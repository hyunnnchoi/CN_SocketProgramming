/*클라이언트가 해야 할 일
Socket()
Connect()
Send(), Recv() 반복
Close()
*/

// 10.211.55.4

#include <arpa/inet.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h> // 유닉스 소켓을 위한 헤더파일
#include <unistd.h>

#define FILE_SERVER "/tmp/test_server" // 통신에 사용될 파일 개념

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clnt_sock;
    int nRcv;
    struct sockaddr_un serv_addr; // Unix socket - un
    char message[1024] = {
        0x00,
    };

    clnt_sock = socket(AF_UNIX, SOCK_STREAM, 0); // Unix socket이기 때문에 AF_UNIX 인자 사용
    if (clnt_sock == -1) { // 파일디스크립터 -1 일 경우 소켓생성 에러
        errorHandling("Socket 생성 에러");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, FILE_SERVER); // 기존의 IP번호와 포트설정과는 달리 파일 경로를 지정해 줌.

    if (connect(clnt_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
        -1) {
        errorHandling("Connect 에러");
    }

    if (read(clnt_sock, message, sizeof(message) - 1) == -1) {
        errorHandling("Read 에러");
    }
    gets(message);
    send(clnt_sock, message, (int)strlen(message), 0);
    printf("Message from server: %s\n", message);
    while (1) {
        printf("\n메세지를 입력하세요: ");
        // fgets(message, sizeof(message), stdin);
        gets(message);
        if (strcmp(message, "exit") == 0) {
            send(clnt_sock, message, (int)strlen(message), 0);
            break;
        }
        send(clnt_sock, message, (int)strlen(message), 0);
        printf("메세지 수신 중...\n");

        nRcv = recv(clnt_sock, message, sizeof(message) - 1, 0);

        if (nRcv == 0) {
            errorHandling("EOF, 더 읽을 것 없음");
        }
        if (nRcv == -1) {
            errorHandling("Read() 에러");
        }

        message[nRcv] = '\0';

        if (strcmp(message, "exit") == 0) {
            printf("서버 연결 종료..\n");
            break;
        }
        printf("받은 메시지: %s", message);
    }
    close(clnt_sock);
    return 0;

    // Socket 함수의 결과인 파일디스크립터, 서버의 주소정보, 2번인자의 크기
}
void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
