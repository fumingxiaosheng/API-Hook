#pragma once


// FileDialog 对话框

class FileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FileDialog)

public:
	FileDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FileDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_file;
};
