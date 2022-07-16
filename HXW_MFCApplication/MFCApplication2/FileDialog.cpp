// FileDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication2.h"
#include "FileDialog.h"
#include "afxdialogex.h"


// FileDialog 对话框

IMPLEMENT_DYNAMIC(FileDialog, CDialogEx)

FileDialog::FileDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE, pParent)
	, m_file(_T(""))
{

}

FileDialog::~FileDialog()
{
}

void FileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_file);
}


BEGIN_MESSAGE_MAP(FileDialog, CDialogEx)
END_MESSAGE_MAP()


// FileDialog 消息处理程序
