#include<stdio.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
int main(void) {
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	/*if (sock == INVALID_SOCKET) {
		char error[100];
		sprintf_s(error, "%d", WSAGetLastError());
		printf("%s", error);
	}
	else {
		printf("success!");
	}*/
	//向服务器发起请求
	
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	int result;
	result = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	Sleep(1000);
	printf("%d", result);
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("Message form server: %s\n", szBuffer);
	//关闭套接字
	closesocket(sock);
	//终止使用 DLL
	WSACleanup();
	return 0;
}