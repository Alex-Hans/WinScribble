#pragma once


// ImageDlg dialog

class ImageDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageDlg)

public:
	ImageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImageDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGIMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_ptX;
	CString m_ptY;
	CString m_turnAngle;
	virtual BOOL OnInitDialog();
};
