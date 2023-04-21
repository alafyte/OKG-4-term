#include "stdafx.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibSurface.h"
#include "math.h"
#include "float.h"

double Surface1(double x, double y)
{
	return x * x + y * y;
};

double Surface2(double x, double y)
{
	return x * x - y * y;
};

double Surface3(double x, double y)
{
	double r = 1 - x * x - y * y;
	double z;
	if (r < 0) z = 0;
	else z = sqrt(r);
	return z;
};

CPlot3D::CPlot3D()
{
	pFunc = NULL;
	ViewPoint.RedimMatrix(3);			//коорд. т.наблюд.
	WinRect.SetRect(0, 0, 200, 200);	//обл рисования 
	ViewPoint(0) = 10;
	ViewPoint(1) = 30;
	ViewPoint(2) = 45;
};

void CPlot3D::SetFunction(pfunc2 pF, CRectD RS, double dx, double dy)	// Устанавливает ф-ю f(x,y)
{
	pFunc = pF;	///ук на f(x,y)
	SpaceRect.SetRectD(RS.left, RS.top, RS.right, RS.bottom);
	MatrF.clear();
	MatrView.clear();
	MatrWindow.clear();
	CreateMatrF(dx, dy);	//заполн MatrF к-ми т.пов.
	CreateMatrView();		//заполн MatrView к-ми т.проекции на XY в ВСК
	CreateMatrWindow();
}

void CPlot3D::SetViewPoint(double r, double fi, double q)	//уст. т.набл. в МСК
{
	ViewPoint(0) = r;	///модуль радиус-вектора
	ViewPoint(1) = fi;	///азимут (от ОХ против чс, 0-360)
	ViewPoint(2) = q;	///угол (от ОZ по ч.с., 0-180)
	MatrView.clear();
	MatrWindow.clear();
	CreateMatrView();
	CreateMatrWindow();
}

CMatrix CPlot3D::GetViewPoint()
{
	CMatrix P = ViewPoint;
	return P;
}

void CPlot3D::SetWinRect(CRect Rect)	//уст. область в окне для рис
{
	WinRect = Rect;
	MatrWindow.clear();	
	CreateMatrWindow();	///МСК -> ОСК
}

void CPlot3D::CreateMatrF(double dx, double dy)		//заполн MatrF к-ми точек пов
{
	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;
	CVecMatrix VecMatrix;
	CMatrix V(4);
	V(3) = 1;
	double max = 0, maxx = 0, maxy = 0;
	for (double x = xL; x <= xH; x += dx)
	{
		VecMatrix.clear(); //очищаем вектор
		for (double y = yL; y <= yH; y += dy)
		{
			V(0) = x;
			V(1) = y;
			V(2) = pFunc(x, y);
			if (V(2) > max)
			{
				max = V(2);
				maxx = x;
				maxy = y;
			}
			VecMatrix.push_back(V);
		}
		MatrF.push_back(VecMatrix);
	}
	double left = 0, right = 0;
	left = maxx - xL;
	right = maxy - yL;
}

void CPlot3D::CreateMatrView()	// Заполн MatrView к-ми точек проекции f(x,y) на XY ВСК
{
	CMatrix MV = CreateViewCoord(ViewPoint(0), ViewPoint(1), ViewPoint(2)); //М-ца(4x4) пересчета в ВСК
	CVecMatrix VecMatrix;
	CMatrix VX(4), V(3);
	V(2) = 1;
	double xmin = DBL_MAX;
	double xmax = DBL_MIN;
	double ymin = DBL_MAX;
	double ymax = DBL_MIN;

	for (int i = 0; i < MatrF.size(); i++)
	{
		VecMatrix.clear();
		for (int j = 0; j < MatrF[i].size(); j++)    //пересчет оконных координат
		{
			VX = MatrF[i][j];
			VX = MV * VX;
			V(0) = VX(0); // x- координата проекции
			V(1) = VX(1);	// y- координата проекции
			VecMatrix.push_back(V);
			//------- Для определения области ViewRect ----------------
			double x = V(0);
			double y = V(1);
			if (x < xmin)xmin = x;
			if (x > xmax)xmax = x;
			if (y < ymin)ymin = y;
			if (y > ymax)ymax = y;
		}
		MatrView.push_back(VecMatrix);
	}
	ViewRect.SetRectD(xmin, ymax, xmax, ymin);	// Опр область
}

void CPlot3D::CreateMatrWindow()	// Заполн MatrWindow окон. к-ми точек изобр
{
	CMatrix MW = SpaceToWindow(ViewRect, WinRect); // М-ца (3х3) пересчета в ОСК
	CVecPoint VecPoint;
	CMatrix  V(3);
	for (int i = 0; i < MatrView.size(); i++)
	{
		VecPoint.clear();
		for (int j = 0; j < MatrView[i].size(); j++)
		{
			V = MatrView[i][j];
			V = MW * V;
			CPoint P((int)V(0), (int)V(1));
			VecPoint.push_back(P);
		}
		MatrWindow.push_back(VecPoint);
	}
}

void CPlot3D::Draw(CDC& dc)
{
	if (MatrWindow.empty())
	{
		TCHAR* error = _T("Массив данных для рисования в окне пуст! ");
		MessageBox(NULL, error, _T("Ошибка"), MB_ICONSTOP);
		return;
	}
	CPoint pt[4];
	int k, d;
	int nRows = MatrWindow.size();
	int nCols = MatrWindow[0].size(); // Все строки одинаковой длины!!!
	for (int i = 0; i < nRows - 1; i++)
		for (int j = 0; j < nCols - 1; j++)
		{
			pt[0] = MatrWindow[i][j];
			pt[1] = MatrWindow[i][j + 1];
			pt[2] = MatrWindow[i + 1][j + 1];
			pt[3] = MatrWindow[i + 1][j];
			dc.Polygon(pt, 4);
		}
}