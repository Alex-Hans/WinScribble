#pragma once


// GaussDlg dialog

class GaussDlg : public CDialog
{
	DECLARE_DYNAMIC(GaussDlg)

public:
	GaussDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GaussDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGGAUSS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_size1;
	CString m_size2;
};
