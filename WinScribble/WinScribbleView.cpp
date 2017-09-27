
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
	childMode = -1;//��ʼ��һ��ֵ�����Ϸ�ͼƬ
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

	MemDC.CreateCompatibleDC(NULL);//��ʱ�����ܻ�ͼ����Ϊû�еط��� 
	MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);//����һ������Ļ��ʾ���ݵ�λͼ
	pOldBit = MemDC.SelectObject(&MemBitmap);//��λͼѡ�뵽�ڴ���ʾ�豸��
	MemDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
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
	pDC->BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ

	

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


void CWinScribbleView::OnShapeLine() //ֱ��
{
	// TODO: Add your command handler code here
	m_drawFlag = 0;
	Mode = 0;
}


void CWinScribbleView::OnShapeRectangle() // ����
{
	// TODO: Add your command handler code here
	m_drawFlag = 1;
	Mode = 1;
}


void CWinScribbleView::OnShapeEllipse() //��Բ
{
	// TODO: Add your command handler code here
	m_drawFlag = 2;
	Mode = 2;
}


void CWinScribbleView::OnShapeCircle()  //Բ
{
	// TODO: Add your command handler code here
	m_drawFlag = 3;
	Mode = 3;
}


void CWinScribbleView::OnShapeDot()    //��
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


	//�ؽ�cimage  
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//��������  


	uchar* pucRow;                                  //ָ������������ָ��  
	uchar* pucImage = (uchar*)cimage.GetBits();     //ָ����������ָ��  
	int nStep = cimage.GetPitch();                  //ÿ�е��ֽ���,ע���������ֵ�����и�  


	if (1 == nChannels)                             //���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��  
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


	//�ؽ�mat  
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//��������  


	uchar* pucRow;                                  //ָ������������ָ��  
	uchar* pucImage = (uchar*)cimage.GetBits();     //ָ����������ָ��  
	int nStep = cimage.GetPitch();                  //ÿ�е��ֽ���,ע���������ֵ�����и�  


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
	// ����ת��������ͼ���м�  
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
		// ȡ�� pImage �� DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image ������Լ��� DC, ���Ա��ʹ�� FromHandle ȡ�Ì����� DC




		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// �����µ� CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		// �� Destination ���^С�ĕr��, ������ Destination DC �ϵ� Stretch Blt mode �Q���Ƿ�Ҫ�������h���c���YӍ
		ResultImageDC->SetStretchBltMode(HALFTONE); // ʹ�����Ʒ�|�ķ�ʽ
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // �{�� Brush �����c

														   // �� pImage ���� ResultImage ����
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}
/*
void CWinScribbleView::reateStretchBmp(CBitmap * pBitmap, int StretchHeight, int StretchWidth, CBitmap * pResultBmp)
{
	// �����@ʾΞĻ�� DC
	CDC dcScreen;
	dcScreen.Attach(::GetDC(NULL));

	// ȡ��ԭʼ�D�n�� dc
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldOrgBitmap = dcMemory.SelectObject(pBitmap);

	// �����µĽY���D�� (ָ����С)
	pResultBmp->CreateCompatibleBitmap(&dcScreen, StretchWidth, StretchHeight);

	// ȡ�ýY���D�ε� dc
	CDC dcFixMemory;
	dcFixMemory.CreateCompatibleDC(&dcScreen);
	CBitmap *pOldReslutBitmap = dcFixMemory.SelectObject(pResultBmp); // ׌ dcFixMemory �x�� "�Y��bmp", �κΌ� dcFixMemory �Ą���������ӳ�� pResultBmp

																	  // ��ԭʼ�D�οs�Ů��� Memory DC����
	BITMAP bmpInfo;
	pBitmap->GetBitmap(&bmpInfo); // ȡ�� ԭʼ�D�n�Č����c�߶�
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
		SetCapture(); // ������겶��
		::SetCursor(m_HCross);//��һ���ܷ�ֹ���������ʱ�򣬱��������ʽ����
	}
		
	m_StartPoint = point; //����ʱ�������
	m_StopPoint = m_StartPoint;


	//�϶�ͼƬ
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
	m_startRectangle = FALSE;//���û��ƾ��ο��־  
	::ClipCursor(NULL);//������꣬�������ָ�ΪĬ�Ϲ��  
					   //��������һ������  
	CClientDC dc(this);//��ȡ�豸���  
	CDC *pDC = GetDC();
	dc.SelectStockObject(NULL_BRUSH);  //���û�ˢΪ�ջ�ˢ  




	switch (m_drawFlag)
	{
	case 0:  //Line  

		::ReleaseCapture();//�����������
		::ClipCursor(NULL);//�������
//		dc.SetROP2(R2_NOT);
		if (m_isSolid)
		{
			CPen* newPen = new CPen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, m_lineWidth, m_lineColor);
			CPen* oldPen = dc.SelectObject(newPen);
			dc.MoveTo(m_StartPoint);
			dc.LineTo(m_StopPoint);

			//������һ�ε���ʱ��  
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
		::ReleaseCapture();//�����������
		::ClipCursor(NULL);//�������
//		dc.SetROP2(R2_NOT);

			//���õ�ǰ��ˢ���ƾ��Σ��ڲ��ɵ�ǰ�ջ�ˢ��NULL_BRUSH����䡣  
			//��ȥMouseMove��Ϣ��Ӧ�е���ʱ����  
		dc.Rectangle(CRect(m_StartPoint, m_StopPoint));
		//���ƹ̶�����
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
			
		
		//m_drawType=0;  //���еĿ���ʹ��ÿ��һ�ζ�Ҫѡ����������״  

		break;
	case 2:  //Ellipse  
		::ReleaseCapture();//�����������
		::ClipCursor(NULL);//�������
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
		::ReleaseCapture();//�����������
		::ClipCursor(NULL);//�������
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
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));//�ڶ������������point����ô�������Ĳ���Բ
		}
			
		break;
	case 4:  //Dot  
		::ReleaseCapture();//�����������
		::ClipCursor(NULL);//�������
//		dc.SetROP2(R2_NOT);
		dc.SetPixel(point, m_lineColor);
		break;

	}

	//����϶�ͼƬ
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

	CClientDC dc(this); //��ȡ�豸���  
//	dc.SetROP2(R2_NOT);
	dc.SetROP2(R2_NOT);

	dc.SelectStockObject(NULL_BRUSH);//��ʹ�û�ˢ  
	CBrush brush(m_fillColor);



	if (TRUE == m_startRectangle) //�����Ƿ��е����ж��Ƿ���Ի�����  
	{
		switch (m_drawFlag)
		{

		case 0: //Line  

			::SetCursor(m_HCross);   //���������״Ϊʮ��
			if (m_isSolid)
			{
				CPen* newPen = new CPen(PS_SOLID, m_lineWidth, m_lineColor);
				CPen* oldPen = dc.SelectObject(newPen);
				dc.MoveTo(m_StartPoint);
				dc.LineTo(m_StopPoint);

				//������һ�ε���ʱ��  
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

			//����ʱ�ߵ��յ㸴�Ƹ�m_OldPoint��  
			//ʹ������Ϣѭ���Ĺ����н���ֵ���ݵ�  
			//��ȥ��һ�λ��ߵĹ����У��Ա��ȥ��һ����������  
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

			//��ȥ��һ�λ��Ƶ���ʱԲ  
			//�趨��Բ��y���꣬��ΪҪ��֤�����x֮�����y֮��  
			m_StopPoint.y = m_StopPoint.x - m_StartPoint.x + m_StartPoint.y;
			dc.Ellipse(CRect(m_StartPoint, m_StopPoint));
			//������ʱԲ  
			point.y = point.x - m_StartPoint.x + m_StartPoint.y;
			dc.Ellipse(CRect(m_StartPoint, point));

			m_StopPoint = point;
			break;
		case 4:   //Dot  
			dc.SetPixel(point, m_lineColor);
			break;

		}

	}
	//��ʾ�������
	CString str;
	str.Format(_T(" (%d, %d)"), point.x, point.y);    //��ʽ������ֵ  
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(str);

	CView::OnMouseMove(nFlags, point);
}


//ѡ�в˵���
//��
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

// ���ɫ
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

//��ɫ
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

//��ͼƬ
void CWinScribbleView::OnImageOpenimage()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("λͼ�ļ���*.bmp��|*.bmp | JEPG�ļ���*.jpg�� | *.jpg || "));
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

//����ͼƬ
void CWinScribbleView::OnImageSaveiamge()
{
	// TODO: Add your command handler code here

	LPCTSTR lpszFilter = _T("BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|");
	CFileDialog  dlg(FALSE, lpszFilter, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, NULL);

	CDC* pDC = GetDC();
	CRect rect;
	this->GetClientRect(&rect);
	//ͼƬ�Ĵ�С
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();
	CBitmap MemBitmap1;
	MemBitmap1.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	CDC MemDC1;
	MemDC1.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit1 = MemDC1.SelectObject(&MemBitmap1);
	MemDC1.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);

	if (dlg.DoModal() != IDOK)return;
	CString filepath = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();
	CFile file;
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
	WCHAR* pBuf = T2W((filepath.GetBuffer(filepath.GetLength())));
	Bitmap bm((HBITMAP)MemBitmap1, NULL);//����bitmap
	CLSID Clsid;
	//�ļ�����path�е��йغ�������޹ض�
	if (fileExt == _T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid);
	else if (fileExt == _T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid);
	else if (fileExt == _T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid);
	else if (fileExt == _T("png"))GetEncoderClsid(L"image/png", &Clsid);
	bm.Save(pBuf, &Clsid, NULL);
	filepath.ReleaseBuffer(filepath.GetLength());
	ReleaseDC(pDC);
}



//ͼ��任
void CWinScribbleView::OnImageImagetranslation()
{
	// TODO: Add your command handler code here
	ImageDlg dlg;
	CDC*pDC = GetDC();
	HDC hdc;
	hdc = ::GetDC(m_hWnd);
	if (dlg.DoModal()==IDOK)
	{
		double PA;//��
		PA = asin(0.5) * 6;
		//ƽ��
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
		//��ת
		int temp = _ttoi(dlg.m_turnAngle);
		if (temp != 0)
		{
			CRect rect;
			GetClientRect(&rect);
			CvRect cvrect(0, 0, img.width, img.height);
			
			
			IplImage *dst = 0;
			double degree;//�Ƕ�
			degree = PA*_ttoi(dlg.m_turnAngle) / 180;
			CImageToMat(image, pic);
			img = pic;//Mat to IplImage
			dst=rotateImage(&img, temp, true);
			
			Mat tmp= cv::cvarrToMat(dst);//IplImage * תMat
			MatToCImage(tmp, image);
			image.Draw(pDC->GetSafeHdc(), picX, picY);
			Invalidate();					
			
		}
	}
}

//�һ�
void CWinScribbleView::OnImageGrayimage()
{
	// TODO: Add your command handler code here
	if (pic.channels() == 1) {
		AfxMessageBox(_T("�Ѿ��һ�����"));
		return;
	}
	cvtColor(pic, pic, CV_BGR2GRAY);
	MatToCImage(pic, image);
	Invalidate();
}

//��˹ƽ��
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

//�����߿�
void CWinScribbleView::OnWidthLine()
{
	// TODO: Add your command handler code here
	LineWidthDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_lineWidth = _ttoi(dlg.m_line);
	}
}

//���Ϊ
void CWinScribbleView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	CClientDC dc(this);

	CRect rect;

	BOOL  showMsgTag;                  //�Ƿ�Ҫ������ͼ�񱣴�ɹ��Ի���"   

	GetClientRect(&rect);                  //��ȡ������С  

	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);

	//��������λͼ  

	HDC hdc = CreateCompatibleDC(dc);      //��������DC���Ա㽫ͼ�񱣴�Ϊ��ͬ�ĸ�ʽ  

	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);

	//��λͼѡ��DC�������淵��ֵ   

	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);

	//����ĻDC��ͼ���Ƶ��ڴ�DC��  



	CImage image;

	image.Attach(hbitmap);                //��λͼת��Ϊһ��ͼ��  

	showMsgTag = TRUE;

	CString  strFilter = _T("BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|");

	CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint.bmp"), NULL, strFilter);

	if (dlg.DoModal() != IDOK)

		return;


	CString saveFilePath;
	CString strFileName;          //����û�û��ָ���ļ���չ������Ϊ�����һ��  

	CString strExtension;

	strFileName = dlg.m_ofn.lpstrFile;

	if (dlg.m_ofn.nFileExtension = 0)               //��չ����ĿΪ0  

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

	saveFilePath = strFileName;     //saveFilePathΪ�����е�ȫ�ֱ���,����ΪCString  

									//AfxMessageBox(saveFilePath);               //��ʾͼ�񱣴��ȫ·��(�����ļ���)  

	HRESULT hResult = image.Save(saveFilePath);     //����ͼ��  

	if (FAILED(hResult))

	{

		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));

	}

	else

	{

		if (showMsgTag)

			MessageBox(_T("�ļ�����ɹ���"));

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

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	CDC MemDC1;
	MemDC1.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit1 = MemDC1.SelectObject(&MemBitmap1);
	MemDC1.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);

	if (dlg.DoModal() != IDOK)return;
	CString filepath = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();
	CFile file;
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
	WCHAR* pBuf = T2W((filepath.GetBuffer(filepath.GetLength())));
	Bitmap bm((HBITMAP)MemBitmap1, NULL);//����bitmap
	CLSID Clsid;
	//�ļ�����path�е��йغ�������޹ض�
	if (fileExt == _T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid);
	else if (fileExt == _T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid);
	else if (fileExt == _T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid);
	else if (fileExt == _T("png"))GetEncoderClsid(L"image/png", &Clsid);
	bm.Save(pBuf, &Clsid, NULL);
	filepath.ReleaseBuffer(filepath.GetLength());
	ReleaseDC(pDC);*/

}

//����ͼƬ
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

