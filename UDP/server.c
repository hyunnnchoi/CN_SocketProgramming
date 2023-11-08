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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> // sockaddr_in, read, write

#define BUFSIZE 1024

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    // UDP: 소켓을 하나만 만들면 됨, 거기서 읽으면 됨.

    int sock; // 서버소켓 파일디스크립터
    // int clnt_sock; // 클라이언트 소켓 파일디스크립터

    int nRcv; // UDP의 recvfrom 할당받을 친구
    char message[BUFSIZE];

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    struct sockaddr_in myaddr;

    // struct data add_data; // types 헤더파일 안에 정의됨

    socklen_t clnt_addr_size;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    // IPv4, SOCK_DGRAM(비연결지향형),
    // 0(TYPE에서 미리 정해짐)
    if (sock == -1) { // socket 함수의 경우 생성 실패시 -1 반환
        errorHandling("Socket 에러");
    }

    // IP주소와 포트번호를 지정해준다.
    memset(&serv_addr, 0, sizeof(serv_addr)); // 메모리 동적할당
    serv_addr.sin_family = AF_INET;           // IPv4
    serv_addr.sin_addr.s_addr =
        htonl(INADDR_ANY); // 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의
                           // IP주소를 사용하라.
    serv_addr.sin_port = htons(atoi(argv[1])); // 포트설정

    // 소켓과 서버 주소를 바인딩
    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        errorHandling("Bind 에러");
    }
    /* ******* Changed: UDP는 listen 과정 필요없음 *******
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
  */

    // 구현할 내용: client의 IP주소와 PORT 번호 어떻게 가져오지?

    char msg[] = "Hello this is server! \n";
    // sendto(sock, msg, (int)strlen(msg), 0);

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        nRcv = recvfrom(sock, (void *)&msg, sizeof(msg), 0,
                        (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        printf("받은 메시지: %s", msg);
        printf("\n보낼 메시지: ");
        gets(msg);
        sendto(sock, (void *)&msg, sizeof(msg), 0,
               (struct sockaddr *)&clnt_addr, clnt_addr_size);
    }
    close(sock);

    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// socat -v tcp-listen:8080 fork system:cat
