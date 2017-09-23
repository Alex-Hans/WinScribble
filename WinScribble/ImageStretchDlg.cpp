// ImageStretchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinScribble.h"
#include "ImageStretchDlg.h"
#include "afxdialogex.h"


// ImageStretchDlg dialog

IMPLEMENT_DYNAMIC(ImageStretchDlg, CDialog)

ImageStretchDlg::ImageStretchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOGROTATE, pParent)
	, m_stretch(_T("1"))
{

}

ImageStretchDlg::~ImageStretchDlg()
{
}

void ImageStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_stretch);
}


BEGIN_MESSAGE_MAP(ImageStretchDlg, CDialog)
END_MESSAGE_MAP()


// ImageStretchDlg message handlers


BOOL ImageStretchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	::SetWindowTextA(m_hWnd, (LPCSTR)"Í¼ÏñËõ·Å");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
