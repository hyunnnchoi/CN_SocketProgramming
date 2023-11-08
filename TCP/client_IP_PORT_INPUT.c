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
#include <unistd.h>

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clnt_sock;
    int nRcv;
    // domain: IPv4, IPv6.. 여부
    // type: TCP or UDP or ... 어떤 서비스 타입의 소켓을 생성할 지
    // Protocol: 소켓에서 사용할 프로토콜.
    struct sockaddr_in serv_addr, from_addr;
    socklen_t addr_size;
    int str_len;
    char message[1024] = {
        0x00,
    };

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == -1) { // 파일디스크립터 -1 일 경우 소켓생성 에러
        errorHandling("Socket 생성 에러");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(clnt_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
        -1) {
        errorHandling("Connect 에러");
    }

    if (read(clnt_sock, message, sizeof(message) - 1) == -1) {
        errorHandling("Read 에러");
    }
    // gets(message);
    // send(clnt_sock, message, (int)strlen(message), 0);
    // printf("Message from server: %s\n", message);
    // while (1) {
    //     printf("\n메세지를 입력하세요: ");
    //     // fgets(message, sizeof(message), stdin);
    //     gets(message);
    //     if (strcmp(message, "exit") == 0) {
    //         send(clnt_sock, message, (int)strlen(message), 0);
    //         break;
    //     }
    //     send(clnt_sock, message, (int)strlen(message), 0);
    //     printf("메세지 수신 중...\n");

    //     nRcv = recv(clnt_sock, message, sizeof(message) - 1, 0);

    //     if (nRcv == 0) {
    //         errorHandling("EOF, 더 읽을 것 없음");
    //     }
    //     if (nRcv == -1) {
    //         errorHandling("Read() 에러");
    //     }

    //     message[nRcv] = '\0';

    //     if (strcmp(message, "exit") == 0) {
    //         printf("서버 연결 종료..\n");
    //         break;
    //     }
    //     printf("받은 메시지: %s", message);
    // }
    // close(clnt_sock);
    // return 0;
    while (1) {
        printf("\n메시지를 입력하세요: ");
        gets(message);
        sendto(clnt_sock, message, sizeof(message), 0,
               (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        addr_size = sizeof(from_addr);
        str_len = recvfrom(clnt_sock, message, sizeof(message), 0,
                           (struct sockaddr *)&from_addr, &addr_size);
        message[str_len] = 0;
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

/*
    serv_.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(80);
*/
