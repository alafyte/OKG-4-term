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
	cz.cx = fabs(right - left);	// Ширина прямоугольной области
	cz.cy = fabs(top - bottom);	// Высота прямоугольной области
	return cz;
}


CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
// Возвращает матрицу пересчета координат из мировых в оконные
// RS - область в мировых координатах - double
// RW - область в оконных координатах - int
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	 // Размер области в окне
	int dwx = sz.cx;	     
	int dwy = sz.cy;	     
	CSizeD szd = RS.SizeD(); // Размер области в МСК

	double dsx = szd.cx;    // Ширина в мск
	double dsy = szd.cy;    // Высота в мск

	double kx = (double)dwx / dsx;   // Масштаб по x
	double ky = (double)dwy / dsy;   // Масштаб по y

	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}


void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)  //MFC
												// Устанавливает режим отображения MM_ANISOTROPIC и его параметры
												// dc - ссылка на класс CDC MFC
												// RS -  область в мировых координатах - int
												// RW -	 Область в оконных координатах - int  
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

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)		// Установка параметров графика-
// XX - вектор данных по X 
// YY - вектор данных по Y 
// RWX - область в окне 
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
	RS.SetRectD(x_min, y_max, x_max, y_min);		// Область в мировой СК
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// Область в окне
	K = SpaceToWindow(RS, RW);			// Матрица пересчета координат
}


void CPlot2D::SetWindowRect(CRect& RWX)		//Установка области в окне для отображения графика
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	// Область в окне
	K = SpaceToWindow(RS, RW);			// Матрица пересчета координат
}


void CPlot2D::GetWindowCoords(double xs, double ys, int& xw, int& yw)		//Пересчет координаты точки из МСК в оконную СК
// Пересчитывает координаты точки из МСК в оконную
// xs - x- кордината точки в МСК
// ys - y- кордината точки в МСК
// xw - x- кордината точки в оконной СК
// yw - y- кордината точки в оконной СК

{
	CMatrix V(3), W(3);
	V(2) = 1;
	V(0) = xs;
	V(1) = ys;
	W = K * V;
	xw = (int)W(0);
	yw = (int)W(1);
}


void CPlot2D::SetPenLine(CMyPen& PLine)		// Перо для рисования графика
// Установка параметров пера для линии графика
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}


void CPlot2D::SetPenAxis(CMyPen& PAxis)		// Перо для осей координат
// Установка параметров пера для линий осей 
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}


void CPlot2D::Draw(CDC& dc, int Ind1, int Ind2)		// Рисование с самостоятельным пересчетом координат
{
	double xs, ys;			// в мск
	int xw, yw;				// в оск

	if (Ind1 == 1)dc.Rectangle(RW);					// рамка

	if (Ind2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0)
		{
			xs =  0;  ys = RS.top;				// Точка (0,y_max) в МСК
			GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -точка (0,y_max) в ОСК		
			dc.MoveTo(xw, yw);						// Перо в точку (0,y_max)

			xs = 0;  ys = RS.bottom;			// Точка (0,y_min) в МСК
			GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -точка (0,y_min) в ОСК
			dc.LineTo(xw, yw);						// Линия (0,y_max) - (0,y_min) - Ось Y
		}
		
		if (RS.top * RS.bottom < 0)
		{
			xs = RS.left;  ys = 0;				// (xs,ys) - точка (x_min,0) в МСК
			GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -точка (x_min,0) в ОСК
			dc.MoveTo(xw, yw);						// Перо в точку (x_min,0)

			xs = RS.right;  ys = 0;			// (xs,ys) - точка (x_max,0) в МСК
			GetWindowCoords(xs, ys, xw, yw);		// (xw,yw) -точка (x_max,0) в ОСК
			dc.LineTo(xw, yw);						// Линия (x_min,0) - (x_max,0) - Ось X
		}
		dc.SelectObject(pOldPen);
	}

	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw);			// координаты начальной точки графика в ОСК

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);								// Перо в начальную точка для рисования графика
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw);			// координаты начальной точки графика с номером i в ОСК
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);
}


void CPlot2D::Draw1(CDC& dc, int Ind1, int Ind2)	// Рисование БЕЗ самостоятельного пересчетa координат


	// Рисует график в режиме MM_ANISOTROPIC - без собственного пересчета координат
	// К моменту вызова функции режим и параметры режима должны быть установлены
	// dc - ссылка на класс CDC MFC
	// Ind1=1/0 - рисовать/не рисовать рамку
	// Ind2=1/0 - рисовать/не рисовать оси координат
{

	CRect IRS(RS.left, RS.top, RS.right, RS.bottom);
	if (Ind1 == 1)dc.Rectangle(IRS);           // Рамка в окне
	if (Ind2 == 1)                     // Если нужны оси...
	{//***
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0)          // Нужна Ось Y
		{
			dc.MoveTo(0, (int)RS.top);    // Перо в точку (0,Ymax)
			dc.LineTo(0, (int)RS.bottom);  // Линия (0,Ymax) - (0,Ymin) - Ось Y
		}

		if (RS.top * RS.bottom < 0)          // Нужна Ось X
		{
			dc.MoveTo((int)RS.left, 0);        // Перо в точку (0,Xmin)
			dc.LineTo((int)RS.right, 0);        //  Линия (0,Xmin) - (0,Xmax) - Ось X
		}
		dc.SelectObject(pOldPen);
	}//***

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo((int)X(0), (int)Y(0));
	for (int i = 1; i < X.rows(); i++)  dc.LineTo((int)X(i), (int)Y(i));
	dc.SelectObject(pOldPen);
}


void CPlot2D::GetRS(CRectD& RS)		// Возвращает область графика в мировой СК
// RS - структура, куда записываются параметры области графика
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}

