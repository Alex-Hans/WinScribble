// GaussDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinScribble.h"
#include "GaussDlg.h"
#include "afxdialogex.h"


// GaussDlg dialog

IMPLEMENT_DYNAMIC(GaussDlg, CDialog)

GaussDlg::GaussDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGGAUSS, pParent)
	, m_size1(_T(""))
	, m_size2(_T(""))
{

}

GaussDlg::~GaussDlg()
{
}

void GaussDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_size1);
	DDX_Text(pDX, IDC_EDIT2, m_size2);
}


BEGIN_MESSAGE_MAP(GaussDlg, CDialog)
END_MESSAGE_MAP()


// GaussDlg message handlers
