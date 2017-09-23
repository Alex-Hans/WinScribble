#pragma once


// LineWidthDlg dialog

class LineWidthDlg : public CDialog
{
	DECLARE_DYNAMIC(LineWidthDlg)

public:
	LineWidthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LineWidthDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGLINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_line;
	virtual BOOL OnInitDialog();
};
