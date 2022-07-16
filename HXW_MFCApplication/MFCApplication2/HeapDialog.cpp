// HeapDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication2.h"
#include "HeapDialog.h"
#include "afxdialogex.h"


// HeapDialog 对话框

IMPLEMENT_DYNAMIC(HeapDialog, CDialogEx)

HeapDialog::HeapDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEAP, pParent)
	, m_heap(_T(""))
{

}

HeapDialog::~HeapDialog()
{
}

void HeapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_heap);
}


BEGIN_MESSAGE_MAP(HeapDialog, CDialogEx)
END_MESSAGE_MAP()


// HeapDialog 消息处理程序
