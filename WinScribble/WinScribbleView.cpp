
// WinScribbleView.cpp : implementation of the CWinScribbleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WinScribble.h"
#endif

#include "WinScribbleDoc.h"
#include "WinScribbleView.h"

#include "Gdiplus.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinScribbleView

IMPLEMENT_DYNCREATE(CWinScribbleView, CView)

BEGIN_MESSAGE_MAP(CWinScribbleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_SHAPE_LINE, &CWinScribbleView::OnShapeLine)
	ON_COMMAND(ID_SHAPE_RECTANGLE, &CWinScribbleView::OnShapeRectangle)
	ON_COMMAND(ID_SHAPE_ELLIPSE, &CWinScribbleView::OnShapeEllipse)
//	ON_COMMAND(ID_SHAPE_CIRCLE, &CWinScribbleView::OnShapeCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SHAPE_DOT, &CWinScribbleView::OnShapeDot)
	ON_COMMAND(ID_SHAPE_CIRCLE, &CWinScribbleView::OnShapeCircle)
//	ON_UPDATE_COMMAND_UI(ID_SHAPE_LINE, &CWinScribbleView::OnUpdateShapeLine)
//	ON_UPDATE_COMMAND_UI(ID_SHAPE_RECTANGLE, &CWinScribbleView::OnUpdateShapeRectangle)
//	ON_UPDATE_COMMAND_UI(ID_SHAPE_ELLIPSE, &CWinScribbleView::OnUpdateShapeEllipse)
//	ON_UPDATE_COMMAND_UI(ID_SHAPE_CIRCLE, &CWinScribbleView::OnUpdateShapeCircle)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_DOT, &CWinScribbleView::OnUpdateShapeDot)
//	ON_COMMAND(ID_IMAGE_READIMAGE, &CWinScribbleView::OnImageReadimage)
//	ON_WM_CREATE()
//	ON_WM_SIZE()
//	ON_WM_TIMER()
ON_COMMAND(ID_RECTANGLE_HOLLOW, &CWinScribbleView::OnRectangleHollow)
ON_COMMAND(ID_RECTANGLE_SOLID, &CWinScribbleView::OnRectangleSolid)
ON_UPDATE_COMMAND_UI(ID_RECTANGLE_HOLLOW, &CWinScribbleView::OnUpdateRectangleHollow)
ON_UPDATE_COMMAND_UI(ID_RECTANGLE_SOLID, &CWinScribbleView::OnUpdateRectangleSolid)
ON_COMMAND(ID_ELLIPSE_HOLLOW, &CWinScribbleView::OnEllipseHollow)
ON_COMMAND(ID_ELLIPSE_SOLID, &CWinScribbleView::OnEllipseSolid)
ON_UPDATE_COMMAND_UI(ID_ELLIPSE_HOLLOW, &CWinScribbleView::OnUpdateEllipseHollow)
ON_UPDATE_COMMAND_UI(ID_ELLIPSE_SOLID, &CWinScribbleView::OnUpdateEllipseSolid)
ON_COMMAND(ID_CIRCLE_HOLLOW, &CWinScribbleView::OnCircleHollow)
ON_COMMAND(ID_CIRCLE_SOLID, &CWinScribbleView::OnCircleSolid)
ON_UPDATE_COMMAND_UI(ID_CIRCLE_HOLLOW, &CWinScribbleView::OnUpdateCircleHollow)
ON_UPDATE_COMMAND_UI(ID_CIRCLE_SOLID, &CWinScribbleView::OnUpdateCircleSolid)
ON_COMMAND(ID_COLOR_FILLCOLOR, &CWinScribbleView::OnColorFillcolor)
ON_COMMAND(ID_COLOR_LINECOLOR, &CWinScribbleView::OnColorLinecolor)
ON_COMMAND(ID_IMAGE_OPENIMAGE, &CWinScribbleView::OnImageOpenimage)
ON_COMMAND(ID_IMAGE_SAVEIAMGE, &CWinScribbleView::OnImageSaveiamge)
ON_COMMAND(ID_IMAGE_IMAGETRANSLATION, &CWinScribbleView::OnImageImagetranslation)
ON_COMMAND(ID_IMAGE_GRAYIMAGE, &CWinScribbleView::OnImageGrayimage)
ON_COMMAND(ID_IMAGE_GAUSSSMOOTH, &CWinScribbleView::OnImageGausssmooth)
ON_COMMAND(ID_WIDTH_LINE, &CWinScribbleView::OnWidthLine)
ON_COMMAND(ID_FILE_SAVE_AS, &CWinScribbleView::OnFileSaveAs)
ON_COMMAND(ID_IMAGE_STRETCHIMAGE, &CWinScribbleView::OnImageStretchimage)
ON_COMMAND(ID_LINE_HOLLOW, &CWinScribbleView::OnLineHollow)
ON_COMMAND(ID_LINE_SOLID, &CWinScribbleView::OnLineSolid)
ON_UPDATE_COMMAND_UI(ID_LINE_HOLLOW, &CWinScribbleView::OnUpdateLineHollow)
ON_UPDATE_COMMAND_UI(ID_LINE_SOLID, &CWinScribbleView::OnUpdateLineSolid)
END_MESSAGE_MAP()

// CWinScribbleView construction/destruction

CWinScribbleView::CWinScribbleView()
{
	// TODO: add construction code here
	m_drawFlag = -1;
	
	m_startRectangle = FALSE;
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	m_isSolid = FALSE;
	m_fillColor = RGB(0, 255, 255);
	m_lineColor = RGB(0, 255, 255);
	m_lineWidth = 1;
	picX = 0;
	picY = 0;
	m_drag = FALSE;
	m_isSretch = FALSE;
	m_isGaussianblur = FALSE;
	ksize1 = 3;
	ksize2 = 3;
	childMode = -1;//初始化一个值用来拖放图片
}

CWinScribbleView::~CWinScribbleView()
{
}

BOOL CWinScribbleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWinScribbleView drawing

void CWinScribbleView::OnDraw(CDC* pDC)
{
	CWinScribbleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	GetClientRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();

	MemDC.CreateCompatibleDC(NULL);//这时还不能绘图，因为没有地方画 
	MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);//建立一个与屏幕显示兼容的位图
	pOldBit = MemDC.SelectObject(&MemBitmap);//将位图选入到内存显示设备中
	MemDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));//先用背景色将位图清除干净，这里我用的是白色作为背景
	CBrush brush(m_fillColor);


	switch (m_drawFlag)
	{
	case 4:		//Dot
		MemDC.SetPixel(m_StopPoint, m_lineColor);

		break;
	case 0:		//Line
		if (m_isSolid)
		{
			MemDC.MoveTo(m_StartPoint);
			MemDC.LineTo(m_StopPoint);
		}
		else
		{
			CPen* newPen = new CPen(PS_DOT, m_lineWidth, m_lineColor);
			CPen* oldPen = MemDC.SelectObject(newPen);
			MemDC.MoveTo(m_StartPoint);
			MemDC.LineTo(m_StopPoint);
			MemDC.SelectObject(oldPen);
			delete newPen;
		}
		break;
	case 1:		//Rectangle

		if (m_isSolid)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = MemDC.SelectObject(&pen);
			HBRUSH oldBrush = (HBRUSH)MemDC.SelectObject(&brush);
			MemDC.Rectangle(CRect(m_StartPoint, m_StopPoint));
			MemDC.SelectObject(oldBrush);
			MemDC.Rectangle(CRect(m_StartPoint, m_StopPoint));
			MemDC.SelectObject(pOldPen);
		}
		else
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = MemDC.SelectObject(&pen);
			MemDC.Rectangle(CRect(m_StartPoint, m_StopPoint));
		}
			

		break;
	case 2:		//Ellipse
		if (m_isSolid)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = MemDC.SelectObject(&pen);
//			MemDC.Ellipse(CRect(m_StartPoint, m_StopPoint
			HBRUSH oldBrush = (HBRUSH)MemDC.SelectObject(&brush);
			MemDC.Ellipse(CRect(m_StartPoint, m_StopPoint));
			MemDC.SelectObject(oldBrush);
			MemDC.SelectObject(pOldPen);
			
		}
		else
			MemDC.Ellipse(CRect(m_StartPoint, m_StopPoint));
		break;
	case 3:		//Circle
		m_StopPoint.y = m_StopPoint.x - m_StartPoint.x + m_StartPoint.y;
		if (m_isSolid)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = MemDC.SelectObject(&pen);
			HBRUSH oldBrush = (HBRUSH)MemDC.SelectObject(&brush);
			MemDC.Ellipse(CRect(m_StartPoint, m_StopPoint));
			MemDC.SelectObject(oldBrush);
			MemDC.SelectObject(pOldPen);
		}
		else
			MemDC.Ellipse(CRect(m_StartPoint, m_StopPoint));
		break;
	default:
		break;
	}
	pDC->BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);//将内存中的图拷贝到屏幕上进行显示

	

	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	if (m_isSretch)
	{
		result.Draw(pDC->GetSafeHdc(), picX, picY);
	}
	else
	{
		if (!image.IsNull())
		{
			image.Draw(pDC->GetSafeHdc(), picX, picY);
		}
	}
	if (m_isGaussianblur)
	{
		gausspic.Draw(pDC->GetSafeHdc(), picX, picY);
	}
}


// CWinScribbleView printing

BOOL CWinScribbleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWinScribbleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWinScribbleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWinScribbleView diagnostics

#ifdef _DEBUG
void CWinScribbleView::AssertValid() const
{
	CView::AssertValid();
}

void CWinScribbleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinScribbleDoc* CWinScribbleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinScribbleDoc)));
	return (CWinScribbleDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinScribbleView message handlers


void CWinScribbleView::OnShapeLine() //直线
{
	// TODO: Add your command handler code here
	m_drawFlag = 0;
	Mode = 0;
}


void CWinScribbleView::OnShapeRectangle() // 矩形
{
	// TODO: Add your command handler code here
	m_drawFlag = 1;
	Mode = 1;
}


void CWinScribbleView::OnShapeEllipse() //椭圆
{
	// TODO: Add your command handler code here
	m_drawFlag = 2;
	Mode = 2;
}


void CWinScribbleView::OnShapeCircle()  //圆
{
	// TODO: Add your command handler code here
	m_drawFlag = 3;
	Mode = 3;
}


void CWinScribbleView::OnShapeDot()    //点
{
	// TODO: Add your command handler code here
	m_drawFlag = 4;
	Mode = 4;
}



void CWinScribbleView::MatToCImage(Mat & mat, CImage & cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage  
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据  


	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimage.GetBits();     //指向数据区的指针  
	int nStep = cimage.GetPitch();                  //每行的字节数,注意这个返回值有正有负  


	if (1 == nChannels)                             //对于单通道的图像需要初始化调色板  
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}

void CWinScribbleView::CImageToMat(CImage & cimage, Mat & mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//重建mat  
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//拷贝数据  


	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimage.GetBits();     //指向数据区的指针  
	int nStep = cimage.GetPitch();                  //每行的字节数,注意这个返回值有正有负  


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}



IplImage * CWinScribbleView::rotateImage(IplImage * src, int angle, bool clockwise)
{
	angle = abs(angle) % 180;
	if (angle > 90)
	{
		angle = 90 - (angle % 90);
	}
	IplImage* dst = NULL;
	int width =
		(double)(src->height * sin(angle * CV_PI / 180.0)) +
		(double)(src->width * cos(angle * CV_PI / 180.0)) + 1;
	int height =
		(double)(src->height * cos(angle * CV_PI / 180.0)) +
		(double)(src->width * sin(angle * CV_PI / 180.0)) + 1;
	int tempLength = sqrt((double)src->width * src->width + src->height * src->height) + 10;
	int tempX = (tempLength + 1) / 2 - src->width / 2;
	int tempY = (tempLength + 1) / 2 - src->height / 2;
	int flag = -1;

	dst = cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
	cvZero(dst);
	IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), src->depth, src->nChannels);
	cvZero(temp);

	cvSetImageROI(temp, cvRect(tempX, tempY, src->width, src->height));
	cvCopy(src, temp, NULL);
	cvResetImageROI(temp);

	if (clockwise)
		flag = 1;

	float m[6];
	int w = temp->width;
	int h = temp->height;
	m[0] = (float)cos(flag * angle * CV_PI / 180.);
	m[1] = (float)sin(flag * angle * CV_PI / 180.);
	m[3] = -m[1];
	m[4] = m[0];
	// 将旋转中心移至图像中间  
	m[2] = w * 0.5f;
	m[5] = h * 0.5f;
	//  
	CvMat M = cvMat(2, 3, CV_32F, m);
	cvGetQuadrangleSubPix(temp, dst, &M);
	cvReleaseImage(&temp);
	return dst;
}

int CWinScribbleView::GetEncoderClsid(const WCHAR * format, CLSID * pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
}

void CWinScribbleView::DisplayImage(IplImage * img, HDC & hDC, CvRect & rDst, CvRect & rSrc)
{
	int bpp = ((img->depth) & 255)*(img->nChannels);

	BYTE buffer[sizeof(BITMAPINFOHEADER) + 1024] = { 0 };
	BITMAPINFO*       pbmi = (BITMAPINFO*)buffer;
	BITMAPINFOHEADER* pbmih = &(pbmi->bmiHeader);

	memset(pbmih, 0, sizeof(BITMAPINFOHEADER));
	pbmih->biSize = sizeof(BITMAPINFOHEADER);
	pbmih->biWidth = img->width;
	pbmih->biHeight = (img->origin ? 1 : -1)*fabs(img->height);
	pbmih->biPlanes = 1;
	pbmih->biBitCount = (unsigned short)bpp;
	pbmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = pbmi->bmiColors;

		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbRed = palette[i].rgbGreen =
				palette[i].rgbBlue = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	SetDIBitsToDevice(hDC, rDst.x, rDst.y, rDst.width, rDst.height,
		rSrc.x, rSrc.y, rSrc.y, rSrc.height,
		img->imageData + rSrc.y*img->widthStep,
		pbmi, DIB_RGB_COLORS);
}

void CWinScribbleView::CreateStretchImage(CImage * pImage, CImage * ResultImage, int StretchHeight, int StretchWidth)
{
	if (pImage->IsDIBSection())
	{
		// 取得 pImage 的 DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image 因為有自己的 DC, 所以必須使用 FromHandle 取得對應的 DC




		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// 建立新的 CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		// 當 Destination 比較小的時候, 會根據 Destination DC 上的 Stretch Blt mode 決定是否要保留被刪除點的資訊
		ResultImageDC->SetStretchBltMode(HALFTONE); // 使用最高品質的方式
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // 調整 Brush 的起點

														   // 把 pImage 畫到 ResultImage 上面
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}
/*
void CWinScribbleView::reateStretchBmp(CBitmap * pBitmap, int StretchHeight, int StretchWidth, CBitmap * pResultBmp)
{
	// 建立顯示螢幕的 DC
	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));

	// 取得原始圖檔的 dc
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldOrgBitmap = dcMemory.SelectObject(pBitmap);

	// 建立新的結果圖形 (指定大小)
	pResultBmp->CreateCompatibleBitmap(&dcScreen, StretchWidth, StretchHeight);

	// 取得結果圖形的 dc
	CDC dcFixMemory;
	dcFixMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldReslutBitmap = dcFixMemory.SelectObject(pResultBmp); // 讓 dcFixMemory 選擇到 "結果bmp", 任何對 dcFixMemory 的動作都會反映到 pResultBmp

																	  // 把原始圖形縮放畫到 Memory DC上面
	BITMAP bmpInfo;
	pBitmap->GetBitmap(&bmpInfo); // 取得 原始圖檔的寬度與高度
	StretchBlt(dcFixMemory, 0, 0, StretchWidth, StretchHeight, dcMemory, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

	// Set Back
	dcMemory.SelectObject(pOldOrgBitmap);
	dcFixMemory.SelectObject(pOldReslutBitmap);
}
*/

void CWinScribbleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_startRectangle = TRUE;
//	m_isButtonDown = TRUE;
	
	if (m_drawFlag != -1)
	{
		SetCapture(); // 设置鼠标捕获
		::SetCursor(m_HCross);//这一句能防止左键单击的时候，保持鼠标样式不变
	}
		
	m_StartPoint = point; //鼠标此时的坐标点
	m_StopPoint = m_StartPoint;


	//拖动图片
	if (!image.IsNull())
	{
		CPoint move;
		move.x = point.x - m_StartPoint.x;
		move.y = point.y - m_StartPoint.y;

		int height = image.GetHeight() + move.y;
		int width = image.GetWidth() + move.x;
		if (move.x<=m_StartPoint.x &&m_StartPoint.x <= width&&move.y<=m_StartPoint.y&& m_StartPoint.y <= height&&childMode==-1)
			m_drag = TRUE;
		else
			m_drag = FALSE;

	}

	CView::OnLButtonDown(nFlags, point);
}


void CWinScribbleView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	CBrush brush(m_fillColor);
	m_startRectangle = FALSE;//重置绘制矩形框标志  
	::ClipCursor(NULL);//解锁光标，即将光标恢复为默认光标  
					   //消隐最后的一个矩形  
	CClientDC dc(this);//获取设备句柄  
	CDC *pDC = GetDC();
	dc.SelectStockObject(NULL_BRUSH);  //设置画刷为空画刷  




	switch (m_drawFlag)
	{
	case 0:  //Line  

		::ReleaseCapture();//结束捕获鼠标
		::ClipCursor(NULL);//解锁鼠标
//		dc.SetROP2(R2_NOT);
		if (m_isSolid)
		{
			CPen* newPen = new CPen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, m_lineWidth, m_lineColor);
			CPen* oldPen = dc.SelectObject(newPen);
			dc.MoveTo(m_StartPoint);
			dc.LineTo(m_StopPoint);

			//绘制这一次的临时线  
			dc.MoveTo(m_StartPoint);
			dc.LineTo(point);
			dc.SelectObject(oldPen);
			delete newPen;
		}
		else
		{
			CPen* newPen = new CPen(PS_DOT, m_lineWidth,m_lineColor);
			CPen* oldPen = dc.SelectObject(newPen);
			dc.MoveTo(m_StartPoint);
			dc.LineTo(m_StopPoint);
			dc.SelectObject(oldPen);
			delete newPen;
		}
		break;
	case 1: //Retangle  
		::ReleaseCapture();//结束捕获鼠标
		::ClipCursor(NULL);//解锁鼠标
//		dc.SetROP2(R2_NOT);

			//利用当前画刷绘制矩形，内部由当前空画刷（NULL_BRUSH）填充。  
			//擦去MouseMove消息响应中的临时矩形  
		dc.Rectangle(CRect(m_StartPoint, m_StopPoint));
		//绘制固定矩形
		if (m_isSolid)
		{

			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
			HBRUSH oldBrush = (HBRUSH)dc.SelectObject(&brush);
			dc.Rectangle(CRect(m_StartPoint, point));
			dc.SelectObject(oldBrush);
			dc.Rectangle(CRect(m_StartPoint, point));
	//		dc.FillRect(CRect(m_StartPoint, point), &brush);
			dc.SelectObject(pOldPen);
		}
		else
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
			dc.Rectangle(CRect(m_StartPoint, point));
		}
			
		
		//m_drawType=0;  //该行的可以使得每画一次都要选择所画的形状  

		break;
	case 2:  //Ellipse  
		::ReleaseCapture();//结束捕获鼠标
		::ClipCursor(NULL);//解锁鼠标
//		dc.SetROP2(R2_NOT);

		dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
		if (m_isSolid)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
//			dc.Ellipse(CRect(m_StartPoint, point));
			HBRUSH oldBrush = (HBRUSH)dc.SelectObject(&brush);
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
			dc.SelectObject(oldBrush);
			dc.SelectObject(pOldPen);
		}
		else
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
			dc.Ellipse(CRect(m_StartPoint, point));
		}
			
		break;
	
	case 3:  //Circle  
		::ReleaseCapture();//结束捕获鼠标
		::ClipCursor(NULL);//解锁鼠标
//		dc.SetROP2(R2_NOT);
		dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
		if (m_isSolid)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
			HBRUSH oldBrush = (HBRUSH)dc.SelectObject(&brush);
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
			dc.SelectObject(oldBrush);
			dc.SelectObject(pOldPen);
		}
		else
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, m_lineWidth, m_lineColor);
			CPen * pOldPen = dc.SelectObject(&pen);
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));//第二个参数如果是point，那么画出来的不是圆
		}
			
		break;
	case 4:  //Dot  
		::ReleaseCapture();//结束捕获鼠标
		::ClipCursor(NULL);//解锁鼠标
//		dc.SetROP2(R2_NOT);
		dc.SetPixel(point, m_lineColor);
		break;

	}

	//鼠标拖动图片
	if (m_drag)
	{
		if (point.x > m_StartPoint.x)
		{
			picX += point.x - m_StartPoint.x;
			picY += point.y - m_StartPoint.y;
		}
		else
		{
			picX = m_StartPoint.x - point.x;
			picY = m_StartPoint.y - point.y;
		}
		image.Draw(pDC->GetSafeHdc(), picX, picY);
		Invalidate();
	}

	CView::OnLButtonUp(nFlags, point);


	
}


void CWinScribbleView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this); //获取设备句柄  
//	dc.SetROP2(R2_NOT);
	dc.SetROP2(R2_NOT);

	dc.SelectStockObject(NULL_BRUSH);//不使用画刷  
	CBrush brush(m_fillColor);



	if (TRUE == m_startRectangle) //根据是否有单击判断是否可以画矩形  
	{
		switch (m_drawFlag)
		{

		case 0: //Line  

			::SetCursor(m_HCross);   //设置鼠标形状为十字
			if (m_isSolid)
			{
				CPen* newPen = new CPen(PS_SOLID, m_lineWidth, m_lineColor);
				CPen* oldPen = dc.SelectObject(newPen);
				dc.MoveTo(m_StartPoint);
				dc.LineTo(m_StopPoint);

				//绘制这一次的临时线  
				dc.MoveTo(m_StartPoint);
				dc.LineTo(point);
				dc.SelectObject(oldPen);
				delete newPen;
			}
			else
			{
				CPen* newPen = new CPen(PS_DOT, m_lineWidth, m_lineColor);
				CPen* oldPen = dc.SelectObject(newPen);
				dc.MoveTo(m_StartPoint);
				dc.LineTo(m_StopPoint);

				dc.MoveTo(m_StartPoint);
				dc.LineTo(point);

				dc.SelectObject(oldPen);
				delete newPen;
			}

			//将临时线的终点复制给m_OldPoint，  
			//使其在消息循环的过程中将该值传递到  
			//擦去上一次画线的过程中，以便擦去上一次所画的线  
			m_StopPoint = point;
			break;
		case 1://Rectangle  

			::SetCursor(m_HCross);

			dc.Rectangle(CRect(m_StartPoint, m_StopPoint));
			dc.Rectangle(CRect(m_StartPoint, point));
			m_StopPoint = point;
			

			break;
		case 2:  //Ellipse  
			::SetCursor(m_HCross);

			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
			dc.Ellipse(CRect(m_StartPoint, point));

			m_StopPoint = point;
			break;
		case 3:  //Circle  
			::SetCursor(m_HCross);

			//擦去上一次绘制的临时圆  
			//设定该圆的y坐标，因为要保证两点的x之差等于y之差  
			m_StopPoint.y = m_StopPoint.x - m_StartPoint.x + m_StartPoint.y;
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
			//绘制临时圆  
			point.y = point.x - m_StartPoint.x + m_StartPoint.y;
			dc.Ellipse(CRect(m_StartPoint, point));

			m_StopPoint = point;
			break;
		case 4:   //Dot  
			dc.SetPixel(point, m_lineColor);
			break;

		}

	}
	//显示鼠标坐标
	CString str;
	str.Format(_T(" (%d, %d)"), point.x, point.y);    //格式化坐标值  
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(str);

	CView::OnMouseMove(nFlags, point);
}


//选中菜单项
//点
void CWinScribbleView::OnUpdateShapeDot(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(Mode == 4);
}


void CWinScribbleView::OnLineHollow()
{
	// TODO: Add your command handler code here
	m_isSolid = FALSE;
	m_drawFlag = 0;
	Mode = 0;
	childMode = 40;
}


void CWinScribbleView::OnLineSolid()
{
	// TODO: Add your command handler code here
	m_isSolid = TRUE;
	m_drawFlag = 0;
	Mode = 0;
	childMode = 41;
}


void CWinScribbleView::OnUpdateLineHollow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 40);
}


void CWinScribbleView::OnUpdateLineSolid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 41);
}


void CWinScribbleView::OnRectangleHollow()
{
	// TODO: Add your command handler code here
	m_isSolid = FALSE;
	m_drawFlag = 1;
	Mode = 1;
	childMode = 10;
}


void CWinScribbleView::OnRectangleSolid()
{
	// TODO: Add your command handler code here
	m_isSolid = TRUE;
	m_drawFlag = 1;
	Mode = 1;
	childMode = 11;
}


void CWinScribbleView::OnUpdateRectangleHollow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 10);
}


void CWinScribbleView::OnUpdateRectangleSolid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 11);
}


void CWinScribbleView::OnEllipseHollow()
{
	// TODO: Add your command handler code here
	m_isSolid = FALSE;
	m_drawFlag = 2;
	Mode = 2;
	childMode = 20;
}


void CWinScribbleView::OnEllipseSolid()
{
	// TODO: Add your command handler code here
	m_isSolid = TRUE;
	m_drawFlag = 2;
	Mode = 2;
	childMode = 21;
}


void CWinScribbleView::OnUpdateEllipseHollow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 20);
}


void CWinScribbleView::OnUpdateEllipseSolid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 21);
}


void CWinScribbleView::OnCircleHollow()
{
	// TODO: Add your command handler code here
	m_isSolid = FALSE;
	m_drawFlag = 3;
	Mode = 3;
	childMode = 30;
}


void CWinScribbleView::OnCircleSolid()
{
	// TODO: Add your command handler code here
	m_isSolid = TRUE;
	m_drawFlag = 3;
	Mode = 3;
	childMode = 31;
}


void CWinScribbleView::OnUpdateCircleHollow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 30);
}


void CWinScribbleView::OnUpdateCircleSolid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(childMode == 31);
}

// 填充色
void CWinScribbleView::OnColorFillcolor()
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_PREVENTFULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_fillColor;
	if (dlg.DoModal() == IDOK)
	{
		m_fillColor = dlg.GetColor();
	}
}

//线色
void CWinScribbleView::OnColorLinecolor()
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_PREVENTFULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_lineColor;
	if (dlg.DoModal() == IDOK)
	{
		m_lineColor = dlg.GetColor();
	}
}

//打开图片
void CWinScribbleView::OnImageOpenimage()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("位图文件（*.bmp）|*.bmp | JEPG文件（*.jpg） | *.jpg || "));
	if (dlg.DoModal() == IDOK)
	{
		if (!image.IsNull())
		{
			image.Destroy();
		}
		image.Load(dlg.GetPathName());
		//CString cstr = dlg.GetPathName();  
		str = CStringA(dlg.GetPathName());
		pic = imread(str, CV_LOAD_IMAGE_COLOR);
		Invalidate();
	}
}

//保存图片
void CWinScribbleView::OnImageSaveiamge()
{
	// TODO: Add your command handler code here

	LPCTSTR lpszFilter = _T("BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|");
	CFileDialog  dlg(FALSE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);

	CDC* pDC = GetDC();
	CRect rect;
	this->GetClientRect(&rect);
	//图片的大小
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();
	CBitmap MemBitmap1;
	MemBitmap1.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	//随后建立与屏幕显示兼容的内存显示设备
	CDC MemDC1;
	MemDC1.CreateCompatibleDC(pDC);
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit1 = MemDC1.SelectObject(&MemBitmap1);
	MemDC1.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);

	if (dlg.DoModal() != IDOK)return;
	CString filepath = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();
	CFile file;
	USES_CONVERSION;      //将cstring转化为const wchar*
	WCHAR* pBuf = T2W((filepath.GetBuffer(filepath.GetLength())));
	Bitmap bm((HBITMAP)MemBitmap1, NULL);//定义bitmap
	CLSID Clsid;
	//文件名和path中的有关和下面的无关额
	if (fileExt == _T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid);
	else if (fileExt == _T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid);
	else if (fileExt == _T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid);
	else if (fileExt == _T("png"))GetEncoderClsid(L"image/png", &Clsid);
	bm.Save(pBuf, &Clsid, NULL);
	filepath.ReleaseBuffer(filepath.GetLength());
	ReleaseDC(pDC);
}



//图像变换
void CWinScribbleView::OnImageImagetranslation()
{
	// TODO: Add your command handler code here
	ImageDlg dlg;
	CDC*pDC = GetDC();
	HDC hdc;
	hdc = ::GetDC(m_hWnd);
	if (dlg.DoModal()==IDOK)
	{
		double PA;//π
		PA = asin(0.5) * 6;
		//平移
		if (!image.IsNull())
		{
			picX = _ttoi(dlg.m_ptX);
			picY = _ttoi(dlg.m_ptY);
			if (!image.IsNull())
			{
				image.Draw(pDC->GetSafeHdc(), picX, picY);
				Invalidate();
			}

		}
		//旋转
		int temp = _ttoi(dlg.m_turnAngle);
		if (temp != 0)
		{
			CRect rect;
			GetClientRect(&rect);
			CvRect cvrect(0, 0, img.width, img.height);
			
			
			IplImage *dst = 0;
			double degree;//角度
			degree = PA*_ttoi(dlg.m_turnAngle) / 180;
			CImageToMat(image, pic);
			img = pic;//Mat to IplImage
			dst=rotateImage(&img, temp, true);
			
			Mat tmp= cv::cvarrToMat(dst);//IplImage * 转Mat
			MatToCImage(tmp, image);
			image.Draw(pDC->GetSafeHdc(), picX, picY);
			Invalidate();					
			
		}
	}
}

//灰化
void CWinScribbleView::OnImageGrayimage()
{
	// TODO: Add your command handler code here
	if (pic.channels() == 1) {
		AfxMessageBox(_T("已经灰化过了"));
		return;
	}
	cvtColor(pic, pic, CV_BGR2GRAY);
	MatToCImage(pic, image);
//	Invalidate();
}

//高斯平滑
void CWinScribbleView::OnImageGausssmooth()
{
	// TODO: Add your command handler code here
	m_isGaussianblur = TRUE;
	GaussDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		ksize1 = _ttoi(dlg.m_size1);
		ksize2 = _ttoi(dlg.m_size2);
		CDC*pDC = GetDC();
		CImageToMat(image, pic);
		Mat dst;
		GaussianBlur(pic, dst, cv::Size(ksize1, ksize2), 0, 0);

		MatToCImage(dst, gausspic);
		gausspic.Draw(pDC->GetSafeHdc(), picX, picY);
	}



}

//设置线宽
void CWinScribbleView::OnWidthLine()
{
	// TODO: Add your command handler code here
	LineWidthDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_lineWidth = _ttoi(dlg.m_line);
	}
}

//另存为
void CWinScribbleView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CClientDC dc(this);

	CRect rect;

	BOOL  showMsgTag;                  //是否要弹出”图像保存成功对话框"   

	GetClientRect(&rect);                  //获取画布大小  

	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);

	//创建兼容位图  

	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式  

	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);

	//将位图选入DC，并保存返回值   

	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);

	//将屏幕DC的图像复制到内存DC中  



	CImage image;

	image.Attach(hbitmap);                //将位图转化为一般图像  

	showMsgTag = TRUE;

	CString  strFilter = _T("BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|");

	CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint.bmp"), NULL, strFilter);

	if (dlg.DoModal() != IDOK)

		return;


	CString saveFilePath;
	CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个  

	CString strExtension;

	strFileName = dlg.m_ofn.lpstrFile;

	if (dlg.m_ofn.nFileExtension = 0)               //扩展名项目为0  

	{

		switch (dlg.m_ofn.nFilterIndex)

		{

		case 1:

			strExtension = "bmp"; break;

		case 2:

			strExtension = "jpg"; break;

		case 3:

			strExtension = "gif"; break;

		case 4:

			strExtension = "png"; break;

		default:

			break;

		}

		strFileName = strFileName + "." + strExtension;

	}

	saveFilePath = strFileName;     //saveFilePath为视类中的全局变量,类型为CString  

									//AfxMessageBox(saveFilePath);               //显示图像保存的全路径(包含文件名)  

	HRESULT hResult = image.Save(saveFilePath);     //保存图像  

	if (FAILED(hResult))

	{

		MessageBox(_T("保存图像文件失败！"));

	}

	else

	{

		if (showMsgTag)

			MessageBox(_T("文件保存成功！"));

	}

	image.Detach();

	SelectObject(hdc, hOldMap);


/*	LPCTSTR lpszFilter = _T("BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|");
	CFileDialog  dlg(FALSE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);

	CDC* pDC = GetDC();
	CRect rect;
	this->GetClientRect(&rect);

	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CBitmap MemBitmap1;
	MemBitmap1.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	//随后建立与屏幕显示兼容的内存显示设备
	CDC MemDC1;
	MemDC1.CreateCompatibleDC(pDC);
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit1 = MemDC1.SelectObject(&MemBitmap1);
	MemDC1.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);

	if (dlg.DoModal() != IDOK)return;
	CString filepath = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();
	CFile file;
	USES_CONVERSION;      //将cstring转化为const wchar*
	WCHAR* pBuf = T2W((filepath.GetBuffer(filepath.GetLength())));
	Bitmap bm((HBITMAP)MemBitmap1, NULL);//定义bitmap
	CLSID Clsid;
	//文件名和path中的有关和下面的无关额
	if (fileExt == _T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid);
	else if (fileExt == _T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid);
	else if (fileExt == _T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid);
	else if (fileExt == _T("png"))GetEncoderClsid(L"image/png", &Clsid);
	bm.Save(pBuf, &Clsid, NULL);
	filepath.ReleaseBuffer(filepath.GetLength());
	ReleaseDC(pDC);*/

}

//缩放图片
void CWinScribbleView::OnImageStretchimage()
{
	// TODO: Add your command handler code here
	m_isSretch = TRUE;
	ImageStretchDlg dlg;
	int height = image.GetHeight();
	int width = image.GetWidth();
	int radio = 1;
	if (dlg.DoModal() == IDOK)
	{
		radio = _ttoi(dlg.m_stretch);
	}
	int resultH = height / radio;
	int resultW = width / radio;
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);

	CreateStretchImage(&image, &result, resultH, resultW);
	result.Draw(pDC->GetSafeHdc(), picX, picY);

}

