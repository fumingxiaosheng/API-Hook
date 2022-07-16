// SocketDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication2.h"
#include "SocketDialog.h"
#include "afxdialogex.h"


// SocketDialog 对话框

IMPLEMENT_DYNAMIC(SocketDialog, CDialogEx)

SocketDialog::SocketDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKET, pParent)
	, m_socket(_T(""))
{

}

SocketDialog::~SocketDialog()
{
}

void SocketDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_socket);
}


BEGIN_MESSAGE_MAP(SocketDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &SocketDialog::OnEnChangeEdit1)
	ON_BN_CLICKED(IDCANCEL, &SocketDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// SocketDialog 消息处理程序


void SocketDialog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void SocketDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
