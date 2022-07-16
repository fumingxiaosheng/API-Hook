//做文件测试使用
#include<stdio.h>
#include<string.h>
#include<windows.h>
int main(void) {
	HANDLE hOpenFile = (HANDLE)CreateFile(L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\TestCode\\test1.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hOpenFile == INVALID_HANDLE_VALUE)
	{
		hOpenFile = NULL;
		printf("Can not open the file\n");
		return 0;
	}
	int ret=WriteFile(hOpenFile, "hello!", strlen("hello!"), NULL, NULL);
	if (ret != 1) {
		printf("Can not write into it!");
		return 0;
	}
	LONG lDistance = 0;
	DWORD dwPtr = SetFilePointer(hOpenFile, lDistance, NULL, FILE_BEGIN);
	if (dwPtr == INVALID_SET_FILE_POINTER)
	{
		DWORD dwError = GetLastError();
	}

	char chBuffer1[100] = { 0 };

	DWORD dwReadSize = 2;
	bool bRet = ReadFile(hOpenFile, chBuffer1, 6, &dwReadSize, NULL);
	if (bRet) {
		printf("Text 1: %s\n", chBuffer1);
	}
	CloseHandle(hOpenFile);
	CopyFile(L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\TestCode\\test1.txt", L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\TestCode\\test2.txt", FALSE);
	hOpenFile = (HANDLE)CreateFile(L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\TestCode\\test1.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	dwReadSize = 2;
	bRet = ReadFile(hOpenFile, chBuffer1, 6, &dwReadSize, NULL);
	if (bRet) {
		printf("Text 2: %s\n", chBuffer1);
	}
	char c = getchar();
	CloseHandle(hOpenFile);
	//open_write的验证
	hOpenFile = (HANDLE)CreateFile(L"test2.exe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hOpenFile == INVALID_HANDLE_VALUE) printf("Not Open current File!");
	bRet = ReadFile(hOpenFile, chBuffer1, 6, &dwReadSize, NULL);
	CloseHandle(hOpenFile);
	return 0;
}
