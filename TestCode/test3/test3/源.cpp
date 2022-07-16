#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include<process.h>
#include <tlhelp32.h>
#include <strsafe.h>
#include<string.h>
#include<wchar.h>
//#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
int main() {
	//CopyFile(L"D:\\test3\\Debug\\test3.exe", L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\test1.exe", FALSE);
	//HANDLE hOpenFile = (HANDLE)CreateFile(L"D:\\test1.exe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	//WriteFile(hOpenFile, "hello!", strlen("hello!"), NULL, NULL);

	//char regname[1000] = "计算机\\HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey2;
	size_t ret = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\HXW", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey2, NULL);
	HKEY hKey;
	ret = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);
	TCHAR Data[254];
	memset(Data, 0, sizeof(Data));
	wcsncpy_s(Data, TEXT("success"), 254);
	size_t iLen = wcslen(Data);
	ret = RegSetValueEx(hKey, L"test5", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	if (ret == ERROR_SUCCESS)
	{
		printf("set value successfully!\n");
	}
	hKey = NULL;
	size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
	if (lRet == ERROR_SUCCESS) {
		printf("open successfully!\n");
	}
	else {
		printf("open failed\n");
	}
	lRet = RegDeleteValue(hKey, L"test5");
	if (lRet == ERROR_SUCCESS) {
		printf("delete success!\n");
	}
	else {
		printf("delete fail!\n");
	}
	//char c = getchar();
	//printf("%c", c);
	RegCloseKey(hKey);
	/*char src[100] = { "This is the src." };
	char dst[100] = { "This is the dst." };
	int n = strlen(src);
	memcpy(dst, src,n*sizeof(char));
	printf("%d", n);
	printf("%s", dst);
	char c = getchar();
	/*
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	int result;
	result = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	Sleep(1000);
	//printf("%d", result);
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("Message form server: %s\n", szBuffer);
	//关闭套接字
	closesocket(sock);
	//终止使用 DLL
	WSACleanup();*/
	return 0;
}
/*
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define COPY_SIZE 512

/*
   MyCopyMemory - A wrapper for CopyMemory

   buf     - destination buffer
   pbData  - source buffer
   cbData  - size of block to copy, in bytes
   bufsize - size of the destination buffer
*/

/*void MyCopyMemory(TCHAR* buf, TCHAR* pbData, SIZE_T cbData, SIZE_T bufsize)
{
	//printf("%d %d\n", cbData, bufsize);
	memcpy(buf, pbData, min(cbData, bufsize));
	
}

void main()
{
	TCHAR buf[BUFFER_SIZE] = TEXT("This is the destination");
	TCHAR pbData[BUFFER_SIZE] = TEXT("This is the source");

	MyCopyMemory(buf, pbData, COPY_SIZE * sizeof(TCHAR), BUFFER_SIZE * sizeof(TCHAR));
	
	_tprintf(TEXT("Destination buffer contents: %s\n"), buf);
	char c = getchar();

}*/



