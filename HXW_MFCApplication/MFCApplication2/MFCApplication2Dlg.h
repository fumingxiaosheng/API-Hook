
// MFCApplication2Dlg.h: 头文件
//

#pragma once


// CMFCApplication2Dlg 对话框
class CMFCApplication2Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_STRING;
	afx_msg void OnEnChangeString();
	afx_msg void OnClickedButton();
	char* new_STRING;
	CString second_STRING;
	CString input_STRING;
	CString ANA_String;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit1();
	CString input_STRING2;
	CButton Picture_Button;
	CString m_NAME;
	CString m_PROCESS_ID;
	CString m_THREAD_COUNT;
	CString m_PARENT_PROCESS_ID;
	CString m_PRIORITY_BASE;
	afx_msg void OnBnClickedHeap();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedOk();
};
