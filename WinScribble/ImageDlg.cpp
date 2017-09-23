// ImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinScribble.h"
#include "ImageDlg.h"
#include "afxdialogex.h"


// ImageDlg dialog

IMPLEMENT_DYNAMIC(ImageDlg, CDialog)

ImageDlg::ImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGIMAGE, pParent)
	, m_ptX(_T("0"))
	, m_ptY(_T("0"))
	, m_turnAngle(_T("0"))
{

}

ImageDlg::~ImageDlg()
{
}

void ImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ptX);
	DDX_Text(pDX, IDC_EDIT2, m_ptY);
	DDX_Text(pDX, IDC_EDIT3, m_turnAngle);
}


BEGIN_MESSAGE_MAP(ImageDlg, CDialog)
END_MESSAGE_MAP()


// ImageDlg message handlers


BOOL ImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	::SetWindowTextA(m_hWnd, (LPCSTR)"Í¼Ïñ´¦Àí");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
