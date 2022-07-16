#pragma once


// HeapDialog 对话框

class HeapDialog : public CDialogEx
{
	DECLARE_DYNAMIC(HeapDialog)

public:
	HeapDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~HeapDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_heap;
};
