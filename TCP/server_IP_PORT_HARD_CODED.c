/*서버가  해야 할 일
socket()
bind()
listen()
accept()
read(), write() 반복
close()
*/
// Case (3), IP Addr, Port를 Command Line Argument로 받음
#include <arpa/inet.h> // 내부에 sockaddr_in 구조체 저장되어 있음, htonl, htons, INADDR_ANY, sockaddr_in
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> // atoi 사용
#include <string.h> // memset 사용
#include <sys/socket.h>
#include <unistd.h> // sockaddr_in, read, write

#define BUFSIZE 1024

#define IPADDR "10.211.55.4"
#define PORT 3550
void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock;
    int clnt_sock;

    int nRcv;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    struct sockaddr_in myaddr;

    socklen_t clnt_addr_size;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    // IPv4, SOCK_STREAM(TCP), Protocol의 경우 앞서 SOCK_STREAM 자체가 TCP이므로
    // 0(TYPE에서 미리 정해짐)
    if (serv_sock == -1) { // socket 함수의 경우 생성 실패시 -1 반환
        errorHandling("Socket 에러");
    }

    // IP주소와 포트번호를 지정해준다.
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr =
        inet_addr(IPADDR); // 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의
                           // IP주소를 사용하라.
    serv_addr.sin_port = htons(PORT);

    // 소켓과 서버 주소를 바인딩
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
        -1) {
        errorHandling("Bind 에러");
    }
    // 여기선 Backlog Queue의 크기를 5로 잡은 것.
    if (listen(serv_sock, 5) == -1) {
        errorHandling("listen 에러");
    }
    // 클라이언트로부터 요청이 오면 연결 수락
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock =
        accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    // accept 시 소켓을 하나 create 하는데, 이것을 클라이언트 소켓으로 지정
    getpeername(clnt_sock, (struct sockaddr *)&myaddr, &clnt_addr_size);

    // 얻어온 정보를 출력한다.
    printf("Port    : %d\n", ntohs(myaddr.sin_port));
    printf("address : %s\n", inet_ntoa(myaddr.sin_addr));

    if (clnt_sock == -1) {
        errorHandling("accept 에러");
    };

    char msg[] = "Hello this is server! \n";
    send(clnt_sock, msg, (int)strlen(msg), 0);

    while (1) {
        printf("메세지 받는 중...\n");
        nRcv = recv(clnt_sock, msg, sizeof(msg) - 1, 0);

        if (nRcv == 0) {
            errorHandling("EOF, 더 읽을 것 없음");
        }
        if (nRcv == -1) {
            errorHandling("Read() 에러");
        }
        msg[nRcv] = '\0';

        if (strcmp(msg, "exit") == 0) {
            printf("Close Client Connection...\n");
            break;
        }

        printf("받은 메시지: %s \n", msg);
        printf("보낸 메시지: ");
        gets(msg);
        if (strcmp(msg, "exit") == 0) {
            send(clnt_sock, msg, (int)strlen(msg), 0);
            break;
        }
        send(clnt_sock, msg, (int)strlen(msg), 0);
    }
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
