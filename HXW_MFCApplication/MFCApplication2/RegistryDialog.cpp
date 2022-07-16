// RegistryDialog.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication2.h"
#include "RegistryDialog.h"
#include "afxdialogex.h"


// RegistryDialog 对话框

IMPLEMENT_DYNAMIC(RegistryDialog, CDialogEx)

RegistryDialog::RegistryDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTRY, pParent)
	, m_registry(_T(""))
{

}

RegistryDialog::~RegistryDialog()
{
}

void RegistryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_registry);
}


BEGIN_MESSAGE_MAP(RegistryDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &RegistryDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// RegistryDialog 消息处理程序


void RegistryDialog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
