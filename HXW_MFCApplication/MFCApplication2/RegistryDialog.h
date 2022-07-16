#pragma once


// RegistryDialog 对话框

class RegistryDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RegistryDialog)

public:
	RegistryDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RegistryDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTRY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_registry;
	afx_msg void OnEnChangeEdit1();
};
