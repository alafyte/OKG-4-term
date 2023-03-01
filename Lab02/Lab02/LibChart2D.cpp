#include "stdafx.h"

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}


void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}


CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// ������ ������������� �������
	cz.cy = fabs(top - bottom);	// ������ ������������� �������
	return cz;
}


CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
// ���������� ������� ��������� ��������� �� ������� � �������
// RS - ������� � ������� ����������� - double
// RW - ������� � ������� ����������� - int
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	 // ������ ������� � ����
	int dwx = sz.cx;	     
	int dwy = sz.cy;	     
	CSizeD szd = RS.SizeD(); // ������ ������� � ���

	double dsx = szd.cx;    // ������ � ���
	double dsy = szd.cy;    // ������ � ���

	double kx = (double)dwx / dsx;   // ������� �� x
	double ky = (double)dwy / dsy;   // ������� �� y

	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}


void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)  //MFC
												// ������������� ����� ����������� MM_ANISOTROPIC � ��� ���������
												// dc - ������ �� ����� CDC MFC
												// RS -  ������� � ������� ����������� - int
												// RW -	 ������� � ������� ����������� - int  
{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
	dc.SetMapMode(MM_ANISOTROPIC);
}

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)		// ��������� ���������� �������-
// XX - ������ ������ �� X 
// YY - ������ ������ �� Y 
// RWX - ������� � ���� 
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();

	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.SetRectD(x_min, y_max, x_max, y_min);		// ������� � ������� ��
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// ������� � ����
	K = SpaceToWindow(RS, RW);			// ������� ��������� ���������
}


void CPlot2D::SetWindowRect(CRect& RWX)		//��������� ������� � ���� ��� ����������� �������
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// ������� � ����
	K = SpaceToWindow(RS, RW);			// ������� ��������� ���������
}


void CPlot2D::GetWindowCoords(double xs, double ys, int& xw, int& yw)		//�������� ���������� ����� �� ��� � ������� ��
// ������������� ���������� ����� �� ��� � �������
// xs - x- ��������� ����� � ���
// ys - y- ��������� ����� � ���
// xw - x- ��������� ����� � ������� ��
// yw - y- ��������� ����� � ������� ��

{
	CMatrix V(3), W(3);
	V(2) = 1;
	V(0) = xs;
	V(1) = ys;
	W = K * V;
	xw = (int)W(0);
	yw = (int)W(1);
}


void CPlot2D::SetPenLine(CMyPen& PLine)		// ���� ��� ��������� �������
// ��������� ���������� ���� ��� ����� �������
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}


void CPlot2D::SetPenAxis(CMyPen& PAxis)		// ���� ��� ���� ���������
// ��������� ���������� ���� ��� ����� ���� 
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}


void CPlot2D::Draw(CDC& dc, int Ind1, int Ind2)		// ��������� � ��������������� ���������� ���������
{
	double xs, ys;			// � ���
	int xw, yw;				// � ���

	if (Ind1 == 1)dc.Rectangle(RW);					// �����

	if (Ind2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		xs = RS.left;  ys = RS.top;				// ����� (0,y_max) � ���
		GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (0,y_max) � ���		
		dc.MoveTo(xw, yw);						// ���� � ����� (0,y_max)

		xs = RS.left;  ys = RS.bottom;			// ����� (0,y_min) � ���
		GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (0,y_min) � ���
		dc.LineTo(xw, yw);						// ����� (0,y_max) - (0,y_min) - ��� Y
		

		xs = RS.left;  ys = RS.top;				// (xs,ys) - ����� (x_min,0) � ���
		GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (x_min,0) � ���
		dc.MoveTo(xw, yw);						// ���� � ����� (x_min,0)

		xs = RS.right;  ys = RS.top;			// (xs,ys) - ����� (x_max,0) � ���
		GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (x_max,0) � ���
		dc.LineTo(xw, yw);						// ����� (x_min,0) - (x_max,0) - ��� X
		
		dc.SelectObject(pOldPen);
	}

	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw);			// ���������� ��������� ����� ������� � ���

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);								// ���� � ��������� ����� ��� ��������� �������
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw);			// ���������� ��������� ����� ������� � ������� i � ���
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}


void CPlot2D::Draw1(CDC& dc, int Ind1, int Ind2)	// ��������� ��� ���������������� ��������a ���������
{

	double xs, ys; // ������� ���������� �����
	int xw, yw; // ������� ���������� �����
	dc.Rectangle(RW);
	CPen MyPen1(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
	CPen* pOldPen1 = dc.SelectObject(&MyPen1);

	xs = RS.left; ys = RS.bottom;			// ����� (0,y_max) � ���
	GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (0,y_max) � ���
	dc.MoveTo(xw, yw);						// ���� � ����� (0,y_max)

	xs = RS.left; ys = RS.top;				// ����� (0,y_min) � ���
	GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (0,y_min) � ���
	dc.LineTo(xw, yw);						// ����� (0,y_max) - (0,y_min) - ��� Y

	xs = RS.right; ys = RS.top;				// (xs,ys) - ����� (x_min,0) � ���
	GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (x_min,0) � ���
	dc.MoveTo(xw, yw);						// ���� � ����� (x_min,0)

	xs = RS.left; ys = RS.top;				// (xs,ys) - ����� (x_max,0) � ���
	GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -����� (x_max,0) � ���
	dc.LineTo(xw, yw);						// ����� (x_min,0) - (x_max,0) - ��� X

	dc.SelectObject(pOldPen1);
	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw);		// ���������� ��������� ����� ������� � ���
	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);						// ���� � ��������� ����� ��� ��������� �������
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw);	// ���������� ��������� ����� ������� � ������� i � ���
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}


void CPlot2D::GetRS(CRectD& RS)		// ���������� ������� ������� � ������� ��
// RS - ���������, ���� ������������ ��������� ������� �������
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}

