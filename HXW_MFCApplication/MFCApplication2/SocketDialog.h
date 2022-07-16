#pragma once


// SocketDialog 对话框

class SocketDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SocketDialog)

public:
	SocketDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SocketDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_socket;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCancel();
};
