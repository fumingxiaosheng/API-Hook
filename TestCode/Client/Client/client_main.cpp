#include<stdio.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll
int main(void) {
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	/*if (sock == INVALID_SOCKET) {
		char error[100];
		sprintf_s(error, "%d", WSAGetLastError());
		printf("%s", error);
	}
	else {
		printf("success!");
	}*/
	//���������������
	
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	int result;
	result = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	Sleep(1000);
	printf("%d", result);
	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//������յ�������
	printf("Message form server: %s\n", szBuffer);
	//�ر��׽���
	closesocket(sock);
	//��ֹʹ�� DLL
	WSACleanup();
	return 0;
}