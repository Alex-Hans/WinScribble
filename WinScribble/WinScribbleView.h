
// WinScribbleView.h : interface of the CWinScribbleView class
//

#pragma once
//#include <opencv2/highgui/highgui.hpp>    
//#include <opencv2/imgproc/imgproc.hpp>    
//#include <opencv2/core/core.hpp> 

#include "MainFrm.h"//显示鼠标坐标
#include <atlimage.h>
//#include "CvvImage.h"
//#include <string>
#include "ImageDlg.h"
#include "LineWidthDlg.h"
#include "ImageStretchDlg.h"
#include "GaussDlg.h"

#include "cv.h"                             //  OpenCV 文件头  
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
	int Mode; //菜单项选中
	UINT m_drawFlag; //图形类型
	CPoint m_StartPoint; //起始坐标
	CPoint m_StopPoint;  //结束坐标/上一次的坐标

	BOOL m_startRectangle; //


	HCURSOR m_HCross;  //鼠标形状为十字

	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象　//随后建立与屏幕显示兼容的内存显示设备

	CRect rect;
	CBitmap *pOldBit;

	BOOL m_isSolid;//判断是否实心
	int childMode;//子菜单项选中

	COLORREF m_fillColor;//填充色
	COLORREF m_lineColor;//线色
	int m_lineWidth;//线宽


	CImage image;//原始图
	Mat pic;
	std::string str;
	void MatToCImage(Mat& mat, CImage& cimage);
	void CImageToMat(CImage& cimage, Mat& mat);

	int picX;//图像左上横坐标
	int picY;//图像左上纵坐标
	BOOL m_drag;//拖放
	IplImage  img;
	IplImage* rotateImage(IplImage* src, int angle, bool clockwise);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);//图片编码

	void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);//缩放
//	void reateStretchBmp(CBitmap* pBitmap, int StretchHeight, int StretchWidth, CBitmap *pResultBmp);//
	BOOL m_isSretch;//是否缩放
	CImage result;//缩放后的图片
	CImage gausspic;//高斯平滑后的图片
	BOOL m_isGaussianblur;//是否高斯平滑
	int ksize1, ksize2;//高斯内核值
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

