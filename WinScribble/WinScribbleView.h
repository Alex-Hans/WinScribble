
// WinScribbleView.h : interface of the CWinScribbleView class
//

#pragma once
//#include <opencv2/highgui/highgui.hpp>    
//#include <opencv2/imgproc/imgproc.hpp>    
//#include <opencv2/core/core.hpp> 

#include "MainFrm.h"//��ʾ�������
#include <atlimage.h>
//#include "CvvImage.h"
//#include <string>
#include "ImageDlg.h"
#include "LineWidthDlg.h"
#include "ImageStretchDlg.h"
#include "GaussDlg.h"

#include "cv.h"                             //  OpenCV �ļ�ͷ  
#include "highgui.h"  
#include "cvaux.h"  
#include "cxcore.h"  
#include "opencv2/opencv.hpp"  
#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp" 

#include <string>  
using namespace cv;

#include "windowsx.h"
#include <atlbase.h>
#include <gdiplusimagecodec.h>

class CWinScribbleView : public CView
{
protected: // create from serialization only
	CWinScribbleView();
	DECLARE_DYNCREATE(CWinScribbleView)

// Attributes
public:
	CWinScribbleDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWinScribbleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShapeLine();
	afx_msg void OnShapeRectangle();
	afx_msg void OnShapeEllipse();

private:
	int Mode; //�˵���ѡ��
	UINT m_drawFlag; //ͼ������
	CPoint m_StartPoint; //��ʼ����
	CPoint m_StopPoint;  //��������/��һ�ε�����

	BOOL m_startRectangle; //


	HCURSOR m_HCross;  //�����״Ϊʮ��

	CDC MemDC; //���ȶ���һ����ʾ�豸����
	CBitmap MemBitmap;//����һ��λͼ����//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸

	CRect rect;
	CBitmap *pOldBit;

	BOOL m_isSolid;//�ж��Ƿ�ʵ��
	int childMode;//�Ӳ˵���ѡ��

	COLORREF m_fillColor;//���ɫ
	COLORREF m_lineColor;//��ɫ
	int m_lineWidth;//�߿�


	CImage image;//ԭʼͼ
	Mat pic;
	std::string str;
	void MatToCImage(Mat& mat, CImage& cimage);
	void CImageToMat(CImage& cimage, Mat& mat);

	int picX;//ͼ�����Ϻ�����
	int picY;//ͼ������������
	BOOL m_drag;//�Ϸ�
	IplImage  img;
	IplImage* rotateImage(IplImage* src, int angle, bool clockwise);//��תͼƬ�ĺ���
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);//ͼƬ����

	void DisplayImage(IplImage* img, HDC &hDC, CvRect &rDst, CvRect &rSrc);//��ʾ��ת���ͼƬ

	void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);//����
//	void reateStretchBmp(CBitmap* pBitmap, int StretchHeight, int StretchWidth, CBitmap *pResultBmp);//
	BOOL m_isSretch;//�Ƿ�����
	CImage result;//���ź��ͼƬ
	CImage gausspic;//��˹ƽ�����ͼƬ
	BOOL m_isGaussianblur;//�Ƿ��˹ƽ��
	int ksize1, ksize2;//��˹�ں�ֵ
public:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShapeDot();
	afx_msg void OnShapeCircle();
//	afx_msg void OnUpdateShapeLine(CCmdUI *pCmdUI);
//	afx_msg void OnUpdateShapeRectangle(CCmdUI *pCmdUI);
//	afx_msg void OnUpdateShapeEllipse(CCmdUI *pCmdUI);
//	afx_msg void OnUpdateShapeCircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShapeDot(CCmdUI *pCmdUI);

	afx_msg void OnRectangleHollow();
	afx_msg void OnRectangleSolid();
	afx_msg void OnUpdateRectangleHollow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRectangleSolid(CCmdUI *pCmdUI);
	afx_msg void OnEllipseHollow();
	afx_msg void OnEllipseSolid();
	afx_msg void OnUpdateEllipseHollow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEllipseSolid(CCmdUI *pCmdUI);
	afx_msg void OnCircleHollow();
	afx_msg void OnCircleSolid();
	afx_msg void OnUpdateCircleHollow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCircleSolid(CCmdUI *pCmdUI);
	afx_msg void OnColorFillcolor();
	afx_msg void OnColorLinecolor();
	afx_msg void OnImageOpenimage();
	afx_msg void OnImageSaveiamge();
	afx_msg void OnImageImagetranslation();
	afx_msg void OnImageGrayimage();
	afx_msg void OnImageGausssmooth();
	afx_msg void OnWidthLine();
	afx_msg void OnFileSaveAs();
	afx_msg void OnImageStretchimage();
	afx_msg void OnLineHollow();
	afx_msg void OnLineSolid();
	afx_msg void OnUpdateLineHollow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLineSolid(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in WinScribbleView.cpp
inline CWinScribbleDoc* CWinScribbleView::GetDocument() const
   { return reinterpret_cast<CWinScribbleDoc*>(m_pDocument); }
#endif

