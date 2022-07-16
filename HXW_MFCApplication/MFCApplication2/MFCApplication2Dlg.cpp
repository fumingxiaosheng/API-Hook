
// MFCApplication2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
//以下是dll注入的测试程序头文件
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include<process.h>
#include <tlhelp32.h>
#include <strsafe.h>

#include <iostream>
#include <cstdio>
#include <detours.h>
#include <sys/types.h>
//T2A需要的头文件
#include <atlbase.h>
#include <atlconv.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
#pragma comment(lib,"C:\\Users\\芙茗\\Desktop\\软件安全课设\\Detours-master\\lib.X86\\detours.lib")

//包含的其余的头文件
#include "HeapDialog.h"
#include "FileDialog.h"
#include "RegistryDialog.h"
#include "SocketDialog.h"
char test[10000000];//存储缓冲区的内容
char ana_string[10000000];
char name[100];
char process_id[100];
char thread_count[100];
char parent_process_id[100];
char priority_base[100];
char heap_analysis[100000000];
char registry_analysis[100000000];
char file_analysis[100000000];
char socket_analysis[100000000];

//堆行为分析所需的全局变量
char HeapHandle[1000][100];
int HeapHandle_num = 0;
int HeapHandle_status[1000];


//文件行为分析所需的全局变量
char File_name[1000][100];
char File_handle[1000][100];
int File_num = 0;
int open_write = 0;
char multiple_folders[1000][100];
int multiple_folders_num = 0;
char modified_exe[1000][100];
int modified_exe_num = 0;
char self_exe[1000][100];
int self_exe_num = 0;

//注册表行为分析所需的全局变量
char Subkey[1000][100];//存储键值的名称和路径
char Subkey_handle[1000][100];//存储打开的键值的句柄
int Subkey_num = 0;
int start_up = 0;

typedef char(*REG_MODE)[256];
REG_MODE reg;
//内部函数声明区域
int hxw_inject(char *);
void security_check();
void HeapCreate_case(char* heapcreate);
void HeapDestroy_case(char* heapdestroy);
void CreateFile_case(char* createfile);
int is_multiple_folders(char* filename);
void WriteFile_case(char* writefile);
void CopyFile_case(char* copyfile);
void RegCreateKeyEx_case(char* regcreatekeyex);
void RegOpenKeyEx_case(char* regopenkeyex);
void RegSetValueEx_ex(char* regsetvalueex);
void RegDeleteValue_case(char* regdeletevalue);
void Socket_case(char* socket);
void Connect_case(char* connect);
void Bind_case(char* bind);
void ReadFile_case(char* readfile);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 对话框



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, m_STRING(_T(""))
	, new_STRING(nullptr)
	, second_STRING(_T(""))
	, input_STRING(_T(""))
	, ANA_String(_T(""))
	, input_STRING2(_T(""))
	, m_NAME(_T(""))
	, m_PROCESS_ID(_T(""))
	, m_THREAD_COUNT(_T(""))
	, m_PARENT_PROCESS_ID(_T(""))
	, m_PRIORITY_BASE(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STRING, m_STRING);
	//DDX_Text(pDX, IDC_EDIT1, new_STRING);
	DDX_Text(pDX, IDC_EDIT2, second_STRING);
	//DDX_Text(pDX, IDC_EDIT1, input_STRING);
	//DDX_Text(pDX, IDC_EDIT3, ANA_String);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, input_STRING2);
	DDX_Control(pDX, IDC_BUTTON, Picture_Button);
	DDX_Text(pDX, IDC_NAME, m_NAME);
	DDX_Text(pDX, IDC_PROCESS_ID, m_PROCESS_ID);
	DDX_Text(pDX, IDC_THREAD_COUNT, m_THREAD_COUNT);
	DDX_Text(pDX, IDC_PARENT_PROCESS_ID, m_PARENT_PROCESS_ID);
	DDX_Text(pDX, ID_PRIORITY_BASE, m_PRIORITY_BASE);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_EN_CHANGE(IDC_STRING, &CMFCApplication2Dlg::OnEnChangeString)
	ON_BN_CLICKED(IDC_BUTTON, &CMFCApplication2Dlg::OnClickedButton)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication2Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_HEAP, &CMFCApplication2Dlg::OnBnClickedHeap)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CMFCApplication2Dlg::OnEnChangeMfceditbrowse1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication2Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDOK, &CMFCApplication2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//HICON m_hicn1 = AfxGetApp()->LoadIcon(IDI_ICON5);
	//Picture_Button.SetIcon(m_hicn1);
	//CFont m_editFont;
	//m_editFont.CreatePointFont(180, L"宋体");

	//m_NAME.SetFont(&m_editFont); // 设置新字体


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//设置背景的颜色
		//CRect   rect;
		//CPaintDC   dc(this);
		//GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(205, 202, 206));   //设置为绿色背景  
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnEnChangeString()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication2Dlg::OnClickedButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//为了解决设计的全局变量的重复使用导致的分析错误问题
	HeapHandle_num = 0;
	File_num = 0;
	Subkey_num = 0;
	start_up = 0;
	open_write = 0;
	self_exe_num = 0;
	modified_exe_num = 0;
	multiple_folders_num = 0;
	UpdateData(TRUE);
	strcpy_s(test, "The following are the intercepted APIs in order:\r\n");
	USES_CONVERSION;
	//char* suspect_exe = T2A(input_STRING);
	char* suspect_exe = T2A(input_STRING2);
	sprintf_s(ana_string, "abnormal behavior detection and analysis results:\r\n");//为了解决ana_string的连续赋值问题
	//进行各个区域的初始化
	sprintf_s(heap_analysis, "The result of heap analysis is as follows:\r\n");
	sprintf_s(file_analysis, "The result of file analysis is as follows:\r\n");
	sprintf_s(socket_analysis, "The result of socket analysis is as follows:\r\n");
	sprintf_s(registry_analysis, "The result of registry analysis is as follows:\r\n");
	hxw_inject(suspect_exe);
	security_check();
	//进行输出的显示
	if (start_up) strcat_s(registry_analysis, "Open self-startup item\r\n");
	//if (open_write) strcat_s(file_analysis, "In view of detecting its self-reading and self-opening behavior,note that it may have self replicating behavior!\r\n");
	int m = 0;
	if (modified_exe_num > 0) strcat_s(file_analysis, "It modified executable file ,the list is:\r\n");
	for (m = 0; m < modified_exe_num; m++) {
		strcat_s(file_analysis, modified_exe[m]);
		strcat_s(file_analysis, "\r\n");
	}
	if (multiple_folders_num > 0) strcat_s(file_analysis, "There are multiple folders within the scope of the operation and the list is:\r\n");
	for (m = 0; m < multiple_folders_num; m++) {
		strcat_s(file_analysis, multiple_folders[m]);
		strcat_s(file_analysis, "\r\n");
	}
	if (self_exe_num > 0) strcat_s(file_analysis, "Self replication occurs.The list of destination is:\r\n");
	for (m = 0; m < self_exe_num; m++) {
		strcat_s(file_analysis, self_exe[m]);
		strcat_s(file_analysis, "\r\n");
	}
	if (open_write &&self_exe_num<=0) strcat_s(file_analysis, "In view of detecting its self-reading and self-opening behavior,note that it may have self replicating behavior!\r\n");
	second_STRING = CA2CT(test);
	//ANA_String = CA2CT(ana_string);
	m_NAME = CA2CT(name);
	m_PROCESS_ID = CA2CT(process_id);
	m_THREAD_COUNT = CA2CT(thread_count);
	m_PARENT_PROCESS_ID = CA2CT(parent_process_id);
	m_PRIORITY_BASE = CA2CT(priority_base);
	UpdateData(FALSE);
}



int hxw_inject(char* suspect_exe)
{
	//std::cout << "Hello World!\n";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	//设置DLL文件夹的名称
	WCHAR DirPath[MAX_PATH + 1];
	wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\DETOURS_sample\\Debug");
	char DLLPath[MAX_PATH + 1] = "C:\\Users\\芙茗\\Desktop\\软件安全课设\\DETOURS_sample\\Debug\\DETOURS_sample.dll";
	HINSTANCE hModule = NULL;
	typedef REG_MODE(*Func)();
	hModule = LoadLibraryEx(TEXT("C:\\Users\\芙茗\\Desktop\\软件安全课设\\DETOURS_sample\\Debug\\DETOURS_sample.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	Func fQuery;
	//REG_MODE reg;
	if (hModule != 0) {
		fQuery = (Func)GetProcAddress(hModule, "query");
		reg = fQuery();


		//WCHAR EXE[MAX_PATH + 1];
		//wcscpy_s(EXE, MAX_PATH, suspect_exe);
		//wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\芙茗\\Desktop\\软件安全课设\\test\\Debug\\test.exe");
		USES_CONVERSION;
		WCHAR* EXE = A2W(suspect_exe);
		if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
			CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi,
			DLLPath, NULL))
		{
			ResumeThread(pi.hThread);//启动线程
			WaitForSingleObject(pi.hProcess, INFINITE);//等待某一线程完成
			//printf("缓冲区查找\n");
			int i = 0;
			char c = reg[0][0];
			while (reg[i][0] != 0) {
				if (i == 0) {
					strcpy_s(name, reg[i]);
					i++;
				}
				else if (i == 1) {
					strcpy_s(process_id, reg[i]);
					i++;
				}
				else if (i == 2) {
					strcpy_s(thread_count, reg[i]);
					i++;
				}
				else if (i == 3) {
					strcpy_s(parent_process_id, reg[i]);
					i++;
				}
				else if (i == 4) {
					strcpy_s(priority_base, reg[i]);
					i++;
				}
				else {
				strcat_s(test, reg[i]);
				//进行相应的处理
				switch (reg[i][1]) {
				case '1': {
					HeapCreate_case(reg[i]);
					break;
				}
				case '2': {
					HeapDestroy_case(reg[i]);
					break;
				}
				case '3': {
					CreateFile_case(reg[i]);
					break;
				}
				case '4': {
					WriteFile_case(reg[i]);
					break;
				}
				case '5': {
					CopyFile_case(reg[i]);
					break;
				}
				case '6': {
					RegCreateKeyEx_case(reg[i]);
					break;
				}
				case '7': {
					RegOpenKeyEx_case(reg[i]);
					break;
				}
				case '8': {
					RegSetValueEx_ex(reg[i]);
					break;
				}
				case '9': {
					RegDeleteValue_case(reg[i]);
					break;
				}
				case 'a': {
					Socket_case(reg[i]);
					break;
				}
				case 'b': {
					Connect_case(reg[i]);
					break;
				}
				case 'c': {
					Bind_case(reg[i]);
					break;
				}
				case 'd': {
					ReadFile_case(reg[i]);
					break;
				}
				}
				i++;
				}
			}
		}
		else {
		char error[100];
		sprintf_s(error, "%d", GetLastError());
		}
		FreeLibrary(hModule);
	}
	return 0;
}



//进行最终的检查，包括堆的释放问题
void security_check() {
	int i = 0;
	int inconformity = 0;//标识堆的申请和释放是否一致
	int repeated_release = 0;//标识堆是否存在重复释放的现象
	for (i = 0; i < HeapHandle_num; i++) {
		if (HeapHandle_status[i] != 0) inconformity = 1;
		if (HeapHandle_status[i] < 0) repeated_release = 1;
	}
	if (inconformity) strcat_s(heap_analysis, "The request and release of the heap are inconsistent:lack of release.\r\n");
	if (repeated_release) strcat_s(heap_analysis, "The request and release of the heap are inconsistent:repeated release of the heap.\r\n");
}

//标号1
void HeapCreate_case(char* heapcreate) {
	int i = 3, j = 0;
	char cur_handle[100];
	while (heapcreate[i] != ':') i++;//一直遍历到返回的句柄处
	i++;
	while (heapcreate[i] != '\r') {
		cur_handle[j] = heapcreate[i];
		i++, j++;
	}
	cur_handle[j] = '\0';
	for (i = 0; i < HeapHandle_num; i++) {
		if (strcmp(cur_handle, HeapHandle[i]) == 0) {
			HeapHandle_status[i]++;
			return;
		}
	}
	strcpy_s(HeapHandle[HeapHandle_num], cur_handle);
	HeapHandle_status[HeapHandle_num]++;//更新该句柄状态
	HeapHandle_num++;//指向下一个新的句柄
}

//标号2
void HeapDestroy_case(char* heapdestroy) {
	int i = 3, j = 0;
	char heapdestroy_new[100] = { 0 };//更新后的只有堆句柄的数组
	while (heapdestroy[i] != '(') i++;
	i++;
	while (heapdestroy[i] != ')') {
		heapdestroy_new[j] = heapdestroy[i];
		i++, j++;
	}
	//下面对已经存在堆句柄进行查找
	i = 0;
	j = 1;
	for(i = 0; i < HeapHandle_num; i++){
		//相同的情况下直接进行句柄的更新
		if (strcmp(heapdestroy_new, HeapHandle[i]) == 0) {
			HeapHandle_status[i]--;
			j = 0;
			break;
		}
}
	//该句柄不存在时，进行复制
	if (j) {
		strcpy_s(HeapHandle[HeapHandle_num], heapdestroy_new);
		HeapHandle_status[HeapHandle_num]--;
		HeapHandle_num++;
	}
}

//标号3
void CreateFile_case(char* createfile) {
	int i = 3, j = 0;
	while (createfile[i] != '(') i++;
	i++;
	while (createfile[i] != ',') {
		File_name[File_num][j] = createfile[i];
		i++, j++;
	}
	File_name[File_num][j] = '\0';
	is_multiple_folders(File_name[File_num]);
	while (createfile[i] != ':') i++;
	i++;
	j = 0;
	while (createfile[i] != '\r') {
		File_handle[File_num][j] = createfile[i];
		i++, j++;
	}
	File_num++;
	
}

int is_multiple_folders(char* filename) {
	//首先割离出路径
	char pathname[100];
	int folder_num = 0;
	int i=0,j=0;
	while (filename[i] != '.') i++;
	while (filename[i] != '\\') i--;
	for (j = 0; j <= i; j++) pathname[j] = filename[j];
	pathname[j] = '*';
	pathname[j + 1] = '.';
	pathname[j + 2] = '*';
	pathname[j + 3] = '\0';
	struct _finddata_t ffblk;
	int done = _findfirst(pathname, &ffblk);
	//printf("%s\n", ffblk.name);
	while (!_findnext(done, &ffblk)) {
		if (strstr(ffblk.name, ".") == NULL) {
			folder_num++;
		}
	}
	if(folder_num >= 1) {
		//strcat_s(file_analysis, "Multiple folders and the path is ");
		//strcat_s(file_analysis, pathname);
		//strcat_s(file_analysis, "\r\n");
		int y = 0;
		for (; y < multiple_folders_num; y++) {
			if (strcmp(multiple_folders[y], pathname) == 0) return 0;
		}
		strcpy_s(multiple_folders[multiple_folders_num], pathname);
		multiple_folders_num++;
	}
	return folder_num;
 }

//标号4
void WriteFile_case(char* writefile) {
	char file_handle[100];//获取待操作的文件句柄
	char file_type[100];
	int i = 3, j = 0;
	while (writefile[i] != '(') i++;
	i++;
	while (writefile[i] != ',') {
		file_handle[j] = writefile[i];
		i++, j++;
	}
	file_handle[j] = '\0';
	for(i= File_num-1;i>-1;i--){
		if (strcmp(file_handle, File_handle[i]) == 0) {
			j = 0;
			while (File_name[i][j] != '.') j++;
			j++;
			int p = 0;
			while (File_name[i][j] != 0) {
				file_type[p] = File_name[i][j];
				p++, j++;
			}
			file_type[p] = '\0';
			if (strcmp(file_type, "exe") == 0 || strcmp(file_type, "dll") == 0 || strcmp(file_type, "ocx")==0) {
				//strcat_s(file_analysis, "Modify executable file:");
				//strcat_s(file_analysis, File_name[i]);
				//strcat_s(file_analysis, "\r\n");
				strcpy_s(modified_exe[modified_exe_num], File_name[i]);
				strcat_s(modified_exe[modified_exe_num], "\0");
				modified_exe_num++;
			}
			break;
		}
	}
}

//标号5
void CopyFile_case(char* copyfile) {
	char filename[100] = { 0 };//去除掉路径后的文件名称
	char destination[100];
	int i = 3;
	int j = 0;
	int k = 0;
	while (copyfile[i] != '.') i++;
	i--;
	while (copyfile[i] != '\\') {
		filename[j] = copyfile[i];
		i--, j++;
	}
	j--;
	for (i = 0; i <= (j / 2); i++) {
		char c = filename[i];
		filename[i] = filename[j - i];
		filename[j - i] = c;
	}
	filename[j + 1] = '.';
	filename[j + 2] = 'e';
	filename[j + 3] = 'x';
	filename[j + 4] = 'e';
	filename[j + 5] = '\0';
	i = 3;
	while (copyfile[i] != ',') i++;
	i++;
	char type[50];
	for (j = 0; copyfile[i] != '.'; i++, j++) destination[j] = copyfile[i];
	destination[j++] = copyfile[i++];
	for (k = 0; copyfile[i] != ','; i++, j++, k++) {
		destination[j] = copyfile[i];
		type[k] = copyfile[i];
	}
	destination[j] = '\0';
	type[k] = '\0';
	if (strcmp(type, "exe") == 0 || strcmp(type, "dll") == 0 || strcmp(type, "ocx")==0) {
		//strcat_s(file_analysis, "Modify executable file:");
		//strcat_s(file_analysis, destination);
		//strcat_s(file_analysis, "\r\n");
		strcpy_s(modified_exe[modified_exe_num], destination);
		strcat_s(modified_exe[modified_exe_num], "\0");
		modified_exe_num++;
	}
	if (strcmp(filename, reg[0])==0) {
		//strcat_s(file_analysis, "Self replication occurs,the name is ");
		//strcat_s(file_analysis, filename);
		//strcat_s(file_analysis, " and the destination is ");
		//strcat_s(file_analysis, destination);
		//strcat_s(file_analysis, "\r\n");
		strcpy_s(self_exe[self_exe_num], destination);
		self_exe_num++;
	}
}

//标号6 example:RegCreateKeyEx(80000001,SOFTWARE \Microsoft\Windows\CurrentVersion\Run,0,0,0,f003f,0,defa64,0):13c 0
void RegCreateKeyEx_case(char* regcreatekeyex) {
	int i = 0, j = 0;
	while (regcreatekeyex[i] != ',') i++;
	i++;
	for (j = 0; regcreatekeyex[i] != ','; i++, j++) Subkey[Subkey_num][j] = regcreatekeyex[i];
	Subkey[Subkey_num][j] = '\0';
	while (regcreatekeyex[i] != ':') i++;
	i++;
	for (j = 0; regcreatekeyex[i] != ' '; i++, j++) Subkey_handle[Subkey_num][j] = regcreatekeyex[i];
	Subkey_handle[Subkey_num][j] = '\0';
	i++;
	//注意这里应该避免Subkey_num的重复增值问题
	if (strstr(Subkey[Subkey_num], "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run") != NULL && regcreatekeyex[i] == '0') {
		//strcat_s(registry_analysis, "Open self startup item\r\n");
		start_up = 1;
		Subkey_num++;
		return;
	}
	if (regcreatekeyex[i] == '0') {
		strcat_s(registry_analysis, "New registry Subkey: ");
		strcat_s(registry_analysis, Subkey[Subkey_num]);
		strcat_s(registry_analysis, "\r\n");
		Subkey_num++;
	}
	return;
}

//标号7 example:RegOpenKeyEx(80000001,SOFTWARE\Microsoft\Windows\CurrentVersion\Run,0,f003f,defa64):140
void RegOpenKeyEx_case(char *regopenkeyex) {
	int i = 0, j = 0;
	while (regopenkeyex[i] != ',') i++;
	i++;
	for (j = 0; regopenkeyex[i] != ','; i++, j++) Subkey[Subkey_num][j] = regopenkeyex[i];
	Subkey[Subkey_num][j] = '\0';
	while (regopenkeyex[i] != ':') i++;
	i++;
	for (j = 0; regopenkeyex[i] != ' '; i++, j++) Subkey_handle[Subkey_num][j] = regopenkeyex[i];
	Subkey_handle[Subkey_num][j] = '\0';
	i++;
	if (strstr(Subkey[Subkey_num], "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run") != NULL && regopenkeyex[i] == '0') {
		//strcat_s(registry_analysis, "attention!open self startup item\r\n");
		start_up = 1;
	}
	Subkey_num++;
	return;
}

//标号8 example:RegSetValueEx(13c,test4,0,1,success,e):0
void RegSetValueEx_ex(char* regsetvalueex) {
	int i = 3, j = 0;
	char cur_handle[100];
	char cur_value[100];
	while (regsetvalueex[i] != '(') i++;
	i++;
	for (j = 0; regsetvalueex[i] != ','; i++, j++) cur_handle[j] = regsetvalueex[i];
	cur_handle[j] = '\0';
	i++;
	for(j=0; regsetvalueex[i] != ','; i++, j++) cur_value[j]= regsetvalueex[i];
	cur_value[j] = '\0';
	while (regsetvalueex[i] != ':') i++;
	i++;
	//当设置成功时，需要显示设置的信息
	if (regsetvalueex[i] == '0') {
		for (j = Subkey_num - 1; j>0&&strcmp(cur_handle, Subkey_handle[j]) != 0; j--);
		strcat_s(registry_analysis, "Set the value of ");
		strcat_s(registry_analysis, Subkey[j]);
		strcat_s(registry_analysis, ":");
		strcat_s(registry_analysis, cur_value);
		strcat_s(registry_analysis, "\r\n");
	}
	return;
}

//标号9 example:RegDeleteValue(140,test3):2
void RegDeleteValue_case(char* regdeletevalue) {
	int i = 3, j = 0;
	char cur_handle[100];
	char cur_value[100];
	while (regdeletevalue[i] != '(') i++;
	i++;
	for (j = 0; regdeletevalue[i] != ','; i++, j++) cur_handle[j] = regdeletevalue[i];
	cur_handle[j] = '\0';
	i++;
	for (j = 0; regdeletevalue[i] != ')'; i++, j++) cur_value[j] = regdeletevalue[i];
	cur_value[j] = '\0';
	i++;
	i++;
	if (regdeletevalue[i] == '0') {
		for (j = Subkey_num - 1; strcmp(cur_handle, Subkey_handle[j]) != 0; j--);
		strcat_s(registry_analysis, "Delete the value of ");
		strcat_s(registry_analysis, Subkey[j]);
		strcat_s(registry_analysis, ":");
		strcat_s(registry_analysis, cur_value);
		strcat_s(registry_analysis, "\r\n");
	}
}

//标号10  example:Socket(2, 1, 6) :144
void Socket_case(char* socket) {
	//根据socket来获取协议的类型
	char protocol[10];
	int i = 0, j = 0;
	while (socket[i] != ',') i++;
	i++;
	while (socket[i] != ',') i++;
	i++;
	for (j = 0; socket[i] != ')'; i++, j++) protocol[j] = socket[i];
	protocol[j] = '\0';
	if (strcmp(protocol, "0")==0) strcat_s(socket_analysis, "The protocol is IP\r\n");
	else if(strcmp(protocol, "1")==0) strcat_s(socket_analysis, "The protocol is ICMP\r\n");
	else if (strcmp(protocol, "2")==0) strcat_s(socket_analysis, "The protocol is IGMP\r\n");
	else if (strcmp(protocol, "3")==0) strcat_s(socket_analysis, "The protocol is GGP\r\n");
	else if (strcmp(protocol, "6")==0) strcat_s(socket_analysis, "The protocol is TCP\r\n");
	else if (strcmp(protocol, "12")==0) strcat_s(socket_analysis, "The protocol is PUP\r\n");
	else if (strcmp(protocol, "17")==0) strcat_s(socket_analysis, "The protocol is UDP\r\n");
	else if (strcmp(protocol, "22")==0) strcat_s(socket_analysis, "The protocol is IDP\r\n");
	else strcat_s(socket_analysis, "The protocol is ND\r\n");
	return;
}

//标号11 example:Connet(144,113f680,10):127.0.0.1 1234
void Connect_case(char* connect) {
	char ip[100];
	char port[100];
	int i = 3,j=0;
	while (connect[i] != ')') i++;
	i++;
	for (j = 0; connect[i] != ' '; i++, j++) ip[j] = connect[i];
	ip[j] = '\r';
	ip[j + 1] = '\n';
	ip[j + 2] = '\0';
	i++;
	for (j = 0; connect[i] != '\r'; i++, j++) port[j] = connect[i];
	port[j] = '\r';
	port[j + 1] = '\n';
	port[j + 2] = '\0';
	strcat_s(socket_analysis, "The client try to connect with server.\r\nServer IP is ");
	strcat_s(socket_analysis, ip);
	strcat_s(socket_analysis, "Server port is ");
	strcat_s(socket_analysis, port);

}

//标号12 Bind(144,7bf984,10):127.0.0.1 1234
void Bind_case(char* bind) {
	char ip[100];
	char port[100];
	int i = 3, j = 0;
	while (bind[i] != ')') i++;
	i++;
	for (j = 0; bind[i] != ' '; i++, j++) ip[j] = bind[i];
	ip[j] = '\r';
	ip[j + 1] = '\n';
	ip[j + 2] = '\0';
	i++;
	for (j = 0; bind[i] != '\r'; i++, j++) port[j] = bind[i];
	port[j] = '\r';
	port[j + 1] = '\n';
	port[j + 2] = '\0';
	strcat_s(socket_analysis, "The server try to bind socket.\r\nServer IP is ");
	strcat_s(socket_analysis, ip);
	strcat_s(socket_analysis, "Server port is ");
	strcat_s(socket_analysis, port);
}

//标号13 ReadFiee(handle,)
void ReadFile_case(char* readfile) {
	char handle[100];
	char cur_file[100];
	int i = 3, j = 0,k=0,m=0;
	while (readfile[i] != '(') i++;
	i++;
	while (readfile[i] != ',') handle[j++] = readfile[i++];
	handle[j] = '\0';
	//Sleep(10);
	i = File_num - 1;
	//Sleep(10);
	for (; i > 0&& strcmp(handle, File_handle[i]) != 0; i--);

	for (j = 0; i>0 && File_name[i][j] != '.'; j++);
	for (k = j - 1;i>0 &&k>=0&& File_name[i][k] != '\\'; k--);
	k++;
	for (m = 0; i>0 && k < j; k++, m++) cur_file[m] = File_name[i][k];
	cur_file[m] = '.';
	cur_file[m + 1] = 'e';
	cur_file[m + 2] = 'x';
	cur_file[m + 3] = 'e';
	cur_file[m + 4] = '\0';
	if (strcmp(cur_file, reg[0]) == 0) open_write = 1;
}
void CMFCApplication2Dlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication2Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication2Dlg::OnBnClickedHeap()
{
	// TODO: 在此添加控件通知处理程序代码
	HeapDialog heapdialog;
	USES_CONVERSION;
	heapdialog.m_heap = CA2CT(heap_analysis);
	UpdateData(FALSE);
	heapdialog.DoModal();
}


void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	FileDialog filedialog;
	USES_CONVERSION;
	filedialog.m_file = CA2CT(file_analysis);
	UpdateData(FALSE);
	filedialog.DoModal();
}


void CMFCApplication2Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	RegistryDialog registrydialog;
	USES_CONVERSION;
	registrydialog.m_registry = CA2CT(registry_analysis);
	UpdateData(FALSE);
	registrydialog.DoModal();
}


void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	SocketDialog socketdialog;
	USES_CONVERSION;
	socketdialog.m_socket = CA2CT(socket_analysis);
	UpdateData(FALSE);
	socketdialog.DoModal();
}


void CMFCApplication2Dlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication2Dlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication2Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
