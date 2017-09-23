#pragma once


// ImageStretchDlg dialog

class ImageStretchDlg : public CDialog
{
	DECLARE_DYNAMIC(ImageStretchDlg)

public:
	ImageStretchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImageStretchDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_stretch;
	virtual BOOL OnInitDialog();
};
