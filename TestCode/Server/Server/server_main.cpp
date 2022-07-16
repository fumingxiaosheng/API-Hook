#include<stdio.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
int main() {
	//��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//�������״̬
	listen(servSock, 20);
	//���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//��ͻ��˷�������
	char str[32] = "Hello World!";
	send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//�ر��׽���
	closesocket(clntSock);
	closesocket(servSock);
	//��ֹ DLL ��ʹ��
	WSACleanup();
	return 0;
}

/*
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;


void PackageResponse(char http_resp[])
{
    const char* hello = "<h1>hello world</h1>";
    int cur_len = 0;
    //cur_len += sprintf(http_resp,"HTTP/1.1 504 OK\n");
    cur_len += sprintf(http_resp, "HTTP/1.1 200 OK\n");
    // cur_len += sprintf(http_resp,"HTTP/1.1 302 OK\n");
    //cur_len += sprintf(http_resp+cur_len,"Location:https://www.baidu.com\n");
    cur_len += sprintf(http_resp + cur_len, "Content-Length:%d\n", strlen(hello));
    cur_len += sprintf(http_resp + cur_len, "\n");
    cur_len += sprintf(http_resp + cur_len, "%s", hello);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Error Usage");
        return 1;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));

    if (ret < 0) {
        perror("bind");
        return 1;
    }
    ret = listen(fd, 5);
    if (ret < 0) {
        perror("listen");
        return 1;
    }

    while (1) {
        sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int new_fd = accept(fd, (sockaddr*)&peer_addr, &len);
        if (new_fd < 0) {
            perror("accept");
            return 1;
        }
        char buf[100 * 1024] = { 0 };
        ssize_t read_size = read(new_fd, buf, sizeof(buf) - 1);
        if (read_size < 0) {
            perror("read");
            close(new_fd);;
            return 1;
        }
        if (read_size == 0) {
            close(new_fd);
            continue;
        }

        printf("\n%s", buf);

        char http_resp[100 * 1024] = { 0 };
        PackageResponse(http_resp);
        write(new_fd, http_resp, strlen(http_resp));
        close(new_fd);
    }
    return 0;

}*/

