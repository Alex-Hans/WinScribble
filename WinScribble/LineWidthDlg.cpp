// LineWidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinScribble.h"
#include "LineWidthDlg.h"
#include "afxdialogex.h"


// LineWidthDlg dialog

IMPLEMENT_DYNAMIC(LineWidthDlg, CDialog)

LineWidthDlg::LineWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGLINE, pParent)
	, m_line(_T(""))
{

}

LineWidthDlg::~LineWidthDlg()
{
}

void LineWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_line);
}


BEGIN_MESSAGE_MAP(LineWidthDlg, CDialog)
END_MESSAGE_MAP()


// LineWidthDlg message handlers


BOOL LineWidthDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	::SetWindowTextA(m_hWnd, (LPCSTR)"Ïß¿í");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
