// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "stdio.h"
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include <io.h>
#include<process.h>
#include <tlhelp32.h>
#include<mutex>
#include<WinSock2.h>
#include<winBase.h>
//#include<string.h>
//#include <Ws2tcpip.h> 
typedef char(*REG_MODE)[256];
//注意这里使用的是x86
#pragma comment(lib,"C:\\Users\\芙茗\\Desktop\\软件安全课设\\Detours-master\\lib.X86\\detours.lib")//链接到库
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
SYSTEMTIME st;

/*dll使用的函数声明区域*/
BOOL GetProcessList();

/*设置各个进程之间的共享缓冲区*/
#pragma data_seg("MySeg")
char seg[300000][256] = {0};//注意这里一定要初始化
int count = 0;
int frequency= 0;//可以定义一个共享的数据
int num = 0;
#pragma data_seg()
#pragma comment(linker,"/section:MySeg,RWS")

//数据操作的互斥操作
std::mutex mtx;
extern "C" __declspec(dllexport)void ChangeSeg(LPCSTR str) {
    mtx.lock();
    count++;
    if (count >= 300000) {
        count--;
    }
    else {
        strcpy_s(seg[count], str);
    }
    mtx.unlock();
}

/*对共享数据的查看函数*/
extern "C" _declspec (dllexport)REG_MODE query()

{
    return seg;
}


/*内部调用函数定义区域*/
BOOL GetProcessList()
{
    int PID = _getpid();
    //printf("当前进程号是：%d\n", PID);
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        //printf("CreateToolhelp32Snapshot (of processes)");
        return(FALSE);
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        //printf("Process32First");  // Show cause of failure
        CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
        return(FALSE);
    }

    // 循环遍历进行寻找
    do
    {
        //printf("\nPROCESS NAME:  %ws %d\n", pe32.szExeFile, pe32.th32ProcessID);
        if (pe32.th32ProcessID == PID) {
           // printf("\n\n=====================================================");
            sprintf_s(seg[count],"%ws", pe32.szExeFile);
            count++;
            //printf("\n-----------------------------------------------------");
            sprintf_s(seg[count], "%d\r\n", pe32.th32ProcessID);
            count++;
            //printf("\n  process ID        = %d", pe32.th32ProcessID);
            sprintf_s(seg[count], "%d\r\n", pe32.cntThreads);
            count++;
           // printf("\n  thread count      = %d", pe32.cntThreads);
            sprintf_s(seg[count], "%d\r\n", pe32.th32ParentProcessID);
            count++;
            //printf("\n  parent process ID = 0x%X", pe32.th32ParentProcessID);
            sprintf_s(seg[count], "%d\r\n", pe32.pcPriClassBase);
            count++;
            //printf("\n  Priority Base     = %d", pe32.pcPriClassBase);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return(TRUE);
}


/*detour函数替换区域*/
//MessageBox函数
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_  LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_  LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_  LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
   // strcpy(lpText,"OMG");
    //printf("MessageBoxW hooked\n");
    //strcpy_s(seg[count], "MessageBoxW Hooked\r\n");
    //count++;
    //OldMessageBoxA(NULL, "W hooked", "SUCCESS", MB_OK);
    //printf("截获到的内容是:%s %s\n", lpText, lpCaption);
    sprintf_s(seg[count], "MessageBoxW(%x,%ls,%ls,%x)\r\n", hWnd,lpText,lpCaption,uType);
    count++;
    return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
}
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_  LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {//将函数声明为导出函数
    //printf("\n******************\n");
    //printf("MessageBoxA Hooked\n");
;
    //OutputDebugString(L"MessageBoxA Hooked");
    //使用共享的缓冲区进行输出
    //strcpy_s(seg[count], "MessageBoxA Hooked\r\n");
    //count++;
    //GetLocalTime(&st);
    //printf("DLL日志输出：%d-%d-%d\n", st.wYear, st.wMonth, st.wDay);
    //printf("截获到的内容是:%s %s\n", lpText, lpCaption);
    //printf("当前的进程号是:%d\n", _getpid());
    //GetProcessList();
    sprintf_s(seg[count], "MessageBoxA(%x,%s,%s,%x)\r\n", hWnd, lpText, lpCaption, uType);
    count++;
    return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
}

//文件操作
static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName, 
    DWORD dwDesireAccess, 
    DWORD dwShareMode, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, 
    DWORD dwFlagsAndAttributes, 
    HANDLE hTemplateFile) = CreateFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName, DWORD dwDesireAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    int num = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)(lpFileName), -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpFileName, -1, pchar, num, NULL, FALSE);//这是宽字符串的字符转换
    HANDLE ret=OldCreateFile(lpFileName, dwDesireAccess, dwShareMode, lpSecurityAttributes,
        dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (strcmp(pchar, "C:\\Windows\\Fonts\\staticcache.dat") != 0 && strcmp(pchar, "C:\\WINDOWS\\Globalization\\Sorting\\sortdefault.nls") != 0) {
        sprintf_s(seg[count], "(3)CreateFile(%s,%x,%x,%x,%x,%x,%x):%x\r\n", pchar, dwDesireAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile, ret);
        count++;
    }
    
    //std::cout << "dwSharemode:0x" << std::hex << dwShareMode << std::endl;
    //printf("dwDesireAccess:%x,dwShareMode:%x,lpSecurityAttributes:%x,\ndwCreationDisposition:%x,dwFlagsAndAttributes:%x,hTemplateFile:%x\n",
        //dwDesireAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    return ret;
}

static BOOL(WINAPI* OldCopyFile)(
    LPCTSTR lpExistingFileName,
    LPCTSTR lpNewFileName,
    BOOL    bFailIfExists
    )=CopyFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewCopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists) {

    int num = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)(lpExistingFileName), -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpExistingFileName, -1, pchar1, num, NULL, FALSE);//这是宽字符串的字符转换
    num = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)(lpNewFileName), -1, NULL, 0, NULL, FALSE);
    char* pchar2 = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpNewFileName, -1, pchar2, num, NULL, FALSE);//这是宽字符串的字符转换
    sprintf_s(seg[count], "(5)CopyFile(%s,%s,%d)\r\n", pchar1, pchar2, bFailIfExists);
    count++;
    return OldCopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}

/*static HANDLE (WINAPI* OldFindFirstFileA)(
    LPCSTR             lpFileName,
    LPWIN32_FIND_DATAA lpFindFileData
) = FindFirstFileA;
extern "C" __declspec(dllexport)HANDLE WINAPI NewFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
    sprintf_s(seg[count], "(5)FindFirstFileA(%ls,%d)\r\n", lpFileName, lpFindFileData);
    count++;
    return OldFindFirstFileA(lpFileName, lpFindFileData);
}

static BOOL(WINAPI* OldFindNextFileA)(
    HANDLE             hFindFile,
    LPWIN32_FIND_DATAA lpFindFileData
    ) = FindNextFileA;
extern "C" __declspec(dllexport)BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {
    //printf("hook findnextfile\n");
    sprintf_s(seg[count], "(6)FindNextFileA(%ls,%d)\r\n", hFindFile, lpFindFileData);
    count++;
    return OldFindNextFileA(hFindFile, lpFindFileData);
}*/

static BOOL(WINAPI* OldWriteFile)(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
) {
    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        sprintf_s(seg[count], "(4)WriteFile(%x,%x,%x,%x,%x)\r\n", hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
        count++;
    }
    return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

static BOOL(WINAPI* OldReadFile)(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
    ) = ReadFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
)
{
    if (GetFileType(hFile) == FILE_TYPE_DISK) {
        sprintf_s(seg[count], "(d)ReadFile(%X,%X,%X,%X,%X)\r\n", hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
        count++;
    }
    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

static BOOL(WINAPI* OldCloseHandle)(
    HANDLE hObject
    ) = CloseHandle;
extern "C" __declspec(dllexport)BOOL WINAPI NewCloseHandle(HANDLE hObject){
    sprintf_s(seg[count], "CloseHandle(%x)\r\n", hObject);
    count++;
    return OldCloseHandle(hObject);
}




//堆操作函数
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaxinumSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaxinumSize) {
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaxinumSize);
    //使用HeapCreate(0,1000,0)对其进行过滤
    if (fIOoptions == 0x0 && dwInitialSize == 0x1000 && dwMaxinumSize == 0x0) return hHeap;
    sprintf_s(seg[count], "(1)HeapCreate(%x,%x,%x):%x\r\n", fIOoptions, dwInitialSize, dwMaxinumSize,hHeap);
    count++;
    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap) {
    //printf("hook HeapDestroy\n");
    int i = OldHeapDestroy(hHeap);
    sprintf_s(seg[count], "(2)HeapDestroy(%x)%d\r\n",hHeap);
    count++;
    return i;
}

/*static LPVOID (WINAPI* OldHeapAlloc)(
    HANDLE hHeap, 
    DWORD dwFlags,
    DWORD dwBytes)=HeapAlloc;
extern "C" __declspec(dllexport)LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, DWORD dwBytes) {
    LPVOID ret = OldHeapAlloc(hHeap, dwFlags, dwBytes);
    //sprintf_s(seg[count], "(12)HeapAlloc(%x,%x,%x):%x\r\n", hHeap, dwFlags, dwBytes,ret);
    sprintf_s(seg[count], "(12)HeapAlloc\r\n");
    count++;
    return ret;
}


static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) {
    sprintf_s(seg[count], "(15)HeapFree(%x,%x,%x)\r\n", hHeap, dwFlags, lpMem);
    count++;
    return OldHeapFree(hHeap, dwFlags, lpMem);
}*/

//注册表函数
/*static LONG(WINAPI* OldRegCreateKey)(
    HKEY hKey,        // handle to an open key
    LPCTSTR lpSubKey, // subkey name
    PHKEY phkResult   // buffer for key handle
    ) = RegCreateKey;
extern "C" __declspec(dllexport)LONG WINAPI NewRegCreateKey (HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult){
    printf("hook RegCreateKey\n");
    strcpy_s(seg[count], "hook RegCreateKey\n");
    count++;
    return OldRegCreateKey(hKey, lpSubKey, phkResult);
}

static LSTATUS(WINAPI* OldRegOpenKey)(
    HKEY hKey,        // handle to open key
    LPCWSTR lpSubKey, // name of subkey to open
    PHKEY phkResult   // handle to open key
    ) = RegOpenKey;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult) {
    printf("hook RegOpenKey\n");
    strcpy_s(seg[count], "hook RegOpenKey\n");
    count++;
    return OldRegOpenKey(hKey, lpSubKey, phkResult);
}*/




/*DETOUR_TRAMPOLINE(static int WINAPI Real_Messagebox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)), MessageBoxA);;
int WINAPI MessageBox_Mine(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    LPCSTR tmp = lpText;
    tmp += " 被Detour截获";

    return Real_Messagebox(hWnd, tmp, lpCaption, uType);
}*/

//注册表操作API
static LSTATUS(WINAPI* OldRegOpenKeyEx)(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult
    ) = RegOpenKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult)
{
    size_t ret= OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    char temp[70];
    memset(temp, 0, sizeof(temp));
    WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), temp, sizeof(temp), NULL, NULL);
    sprintf_s(seg[count], "(7)RegOpenKeyEx(%x,%s,%x,%x,%x):%x %x\r\n", hKey, temp, ulOptions, samDesired, phkResult,*phkResult,ret);
    count++;
    return ret;
}

static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
    sprintf_s(seg[count], "RegCloseKey(%x)\r\n", hKey);
    count++;
    return OldRegCloseKey(hKey);
}

static LSTATUS(WINAPI* OldRegCreateKeyEx)(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition
) {
    //char temp[70];
    //memset(temp, 0, sizeof(temp));
    //WideCharToMultiByte(CP_ACP, 0, lpSubKey, wcslen(lpSubKey), temp, sizeof(temp), NULL, NULL);
    size_t ret= OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)(lpSubKey), -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpSubKey, -1, pchar1, num, NULL, FALSE);//这是宽字符串的字符转换
    sprintf_s(seg[count], "(6)RegCreateKeyEx(%x,%s,%x,%x,%x,%x,%x,%x,%x):%x %x\r\n", hKey,pchar1,Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition,*(phkResult),ret);
    count++;
    return ret;
}

static LSTATUS(WINAPI* OldRegSetValueEx)(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE* lpData,
    DWORD      cbData
    ) = RegSetValueEx;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE * lpData,
    DWORD      cbData)
{
    size_t ret = OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    char temp1[70];
    char temp2[70];
    memset(temp1, 0, sizeof(temp1));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), temp1, sizeof(temp1), NULL, NULL);
    memset(temp2, 0, sizeof(temp2));
    WideCharToMultiByte(CP_ACP, 0, (LPCWCH)lpData, wcslen((LPCWCH)lpData), temp2, sizeof(temp2), NULL, NULL);
    sprintf_s(seg[count], "(8)RegSetValueEx(%x,%s,%x,%x,%s,%x):%x\r\n", hKey,temp1, Reserved,dwType, temp2, cbData,ret);
    count++;
    return ret;
}

static LSTATUS(WINAPI* OldRegDeleteValue)(
    HKEY    hKey,
    LPCWSTR lpValueName
    ) = RegDeleteValue;

extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
    HKEY    hKey,
    LPCWSTR lpValueName)
{
    size_t ret = OldRegDeleteValue(hKey, lpValueName);
    char temp[70];
    memset(temp, 0, sizeof(temp));
    WideCharToMultiByte(CP_ACP, 0, lpValueName, wcslen(lpValueName), temp, sizeof(temp), NULL, NULL);
    sprintf_s(seg[count], "(9)RegDeleteValue(%x,%s):%x\r\n", hKey,temp,ret);
    count++;
    return ret;
}

//网络通信操作函数

static SOCKET(WINAPI* Oldsocket)(
    int af,
    int type,
    int protocol
    ) = socket;
extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol) {
    SOCKET ret= Oldsocket(af, type, protocol);
    sprintf_s(seg[count], "(a)Socket(%d,%d,%d):%x\r\n", af, type, protocol,ret);
    count++;
    return ret;
}


static int (WINAPI* Oldbind)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = bind;
extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen) {
    
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    //需要读出其端口和IP地址
    sprintf_s(seg[count], "(c)Bind(%x,%x,%x):%s %d\r\n",s,name,namelen, inet_ntoa(sock->sin_addr),port);
    count++;
    return Oldbind(s, name, namelen);
}

static int (WINAPI* Oldconnect)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = connect;
extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    //需要读出其端口和IP地址
    sprintf_s(seg[count], "(b)Connet(%x,%x,%x):%s %d\r\n", s, name, namelen, inet_ntoa(sock->sin_addr), port);
    count++;
    return Oldconnect(s, name, namelen);
}

static int (WINAPI* Oldsend)(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
    ) = send;
extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
    sprintf_s(seg[count], "Send(%x,%s,%d,%d)\r\n" ,s, buf, len,flags);
    count++;
    return Oldsend(s, buf, len, flags);
}



static int (WINAPI* Oldrecv)(
    SOCKET     s,
    char* buf,
    int        len,
    int        flags
    ) = recv;
extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags) {
    int ret;
    ret= Oldrecv(s, buf, len, flags);
    sprintf_s(seg[count], "Recv(%x,%s,%d,%d)\r\n", s, buf, len, flags);
    count++;
    return ret;
}

//内存拷贝区域
static void* (__cdecl* Oldmemcpy)(void* _Dst, const void* _Src, size_t _Size) = memcpy;
extern "C" __declspec(dllexport)void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size) {
    //printf("the size is:%d", _Size);
    /*char* String1 = (char*)_Src;
    char String2[100] = { 0 };
    int k = _Size;
    int i = 0;
    while (k--) {
        String2[i++] = *String1++;
    }
    num++;
    String2[i] = '\0';
    printf("this is dll\n");
    printf("this is dll %s\n", String2);*/
    sprintf(seg[count],"Memcpy(%x,%x,%d):%d\r\n", _Dst, _Src, _Size,num);
    num++;
    count++;
    return Oldmemcpy(_Dst, _Src, _Size);
}

/*static void (WINAPI* OldCopyMemory)(
    PVOID Destination,
    const VOID* Source,
    SIZE_T Length
    ) = CopyMemory;
extern "C" __declspec(dllexport)void NewCopyMemory(PVOID Destination, const VOID * Source, SIZE_T Length) {
    sprintf(seg[count], "CopyMemory(%x,%x,%d)", Destination, Source, Length);
    count++;
    return OldCopyMemory(Destination, Source, Length);
}*/



/*main函数区域*/
BOOL APIENTRY DllMain(HMODULE hModule,//dll加载的具体位置
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if(frequency==1)GetProcessList();
    frequency++;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();//初始化
        DetourUpdateThread(GetCurrentThread());//更新线程
        DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldCopyFile, NewCopyFile);
        //DetourAttach(&(PVOID&)OldFindFirstFileA, NewFindFirstFileA);
        //DetourAttach(&(PVOID&)OldFindNextFileA, NewFindNextFileA);
        DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
        DetourAttach(&(PVOID&)OldCloseHandle, NewCloseHandle);

        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        //DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
        //DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        //DetourAttach(&(PVOID&)OldRegCreateKey, NewRegCreateKey);
        //DetourAttach(&(PVOID&)OldRegOpenKey, NewRegOpenKey);
        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);

        DetourAttach(&(PVOID&)Oldsocket, Newsocket);
        DetourAttach(&(PVOID&)Oldbind, Newbind);
        DetourAttach(&(PVOID&)Oldsend, Newsend);
        DetourAttach(&(PVOID&)Oldconnect, Newconnect);
        DetourAttach(&(PVOID&)Oldrecv, Newrecv);

        DetourAttach(&(PVOID&)Oldmemcpy, Newmemcpy);
        DetourTransactionCommit();//检查是否完成*/
        break;
    }
    case DLL_THREAD_ATTACH: 
    case DLL_THREAD_DETACH: 
    case DLL_PROCESS_DETACH: {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
        DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

        DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourDetach(&(PVOID&)OldCopyFile, NewCopyFile);
        //DetourDetach(&(PVOID&)OldFindFirstFileA, NewFindFirstFileA);
        //DetourDetach(&(PVOID&)OldFindNextFileA, NewFindNextFileA);
        DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
        DetourDetach(&(PVOID&)OldCloseHandle, NewCloseHandle);

        DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        //DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
        //DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);

        //DetourDetach(&(PVOID&)OldRegCreateKey, NewRegCreateKey);
        //DetourDetach(&(PVOID&)OldRegOpenKey, NewRegOpenKey);
        DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
        DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);

        DetourDetach(&(PVOID&)Oldsocket, Newsocket);
        DetourDetach(&(PVOID&)Oldbind, Newbind);
        DetourDetach(&(PVOID&)Oldsend, Newsend);
        DetourDetach(&(PVOID&)Oldconnect, Newconnect);
        DetourDetach(&(PVOID&)Oldrecv, Newrecv);

        DetourDetach(&(PVOID&)Oldmemcpy, Newmemcpy);

        DetourTransactionCommit();
        break;
    }
    }
    return TRUE;
}

