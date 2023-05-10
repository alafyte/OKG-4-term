#include "stdafx.h"


double Function1(double x, double y)
{
	return x * x + y * y;
}
//----------------------------------------------
double Function2(double x, double y)
{
	return x * x - y * y;
}
//-----------------------------------------------

double Function3(double x, double y)
{
	double r = sqrt(x * x + y * y);
	double z;
	if (r < 0.001)
		z = 10;
	else z = 10 * sin(r) / r;
	return z;
}


//-------------------------------------------
CPlot3D::CPlot3D()
{

	pFunc = NULL;
	ViewPoint.RedimMatrix(3);
	WinRect.SetRect(0, 0, 200, 200);
	ViewPoint(0) = 10, ViewPoint(1) = 30; ViewPoint(2) = 45;
}

//-------------------------------------------------------------
void CPlot3D::SetFunction(pfunc2 pF, CRectD RS, double dx, double dy)
// Устанавливает функцию f(x,y)
// pFunc - указатель на функцию f(x,y) - поверхность для построения
// RS - область в МСК (xmin,ymax,xmax,ymin) для расчета значений f(x) 
// dx, dy - шаги для расчет значений f(x) по x и y 
// VP - координаты точки наблюдения (r,fi,q) в сферической МСК  
// RW - область в окне для рисования 
{
	pFunc = pF;
	SpaceRect.SetRectD(RS.left, RS.top, RS.right, RS.bottom);
	MatrF.clear();
	MatrView.clear();
	MatrWindow.clear();

	CreateMatrF(dx, dy);
	CreateMatrView();
	CreateMatrWindow();
}
//-------------------------------------------------------------
void CPlot3D::SetViewPoint(double r,double fi,double q)
// Устанавливает положение точки наблюдения в МИРОВОЙ СК - 
// -заполняет матрицу ViewPoint
// r - модуль радиус-вектора
// fi - азимут (от оси OX против часовой стрелки, 0-360), град.
// q  - угол (от оси Z по часовой стрелке, 0-180), град.

{
	ViewPoint(0)=r, ViewPoint(1)=fi; ViewPoint(2)=q;	
	MatrView.clear();
	CreateMatrView();
	MatrWindow.clear();
	CreateMatrWindow();
}

//------------------------------------------------------------
CMatrix CPlot3D::GetViewPoint()
{
	CMatrix P=ViewPoint;
	return P;
}

//-------------------------------------------------------------

void CPlot3D::SetWinRect(CRect Rect)
// Устанавливает область в окне для рисования
// Rect - новая область
{
	WinRect=Rect;
	MatrWindow.clear();
	CreateMatrWindow();
}

//-------------------------------------------------------------

void CPlot3D::CreateMatrF(double dx, double dy)
// Заполняет матрицу MatrF координатами точек поверхности
// dx - шаг для расчета значений функции f(x,y) оси x
// dy - шаг для расчета значений функции f(x,y) оси y
{
	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;
	CVecMatrix VecMatrix;
	CMatrix V(4);
	V(3) = 1;
	for (double x = xL; x <= xH; x += dx)
	{
		VecMatrix.clear();
		for (double y = yL; y <= yH; y += dy)
		{
			V(0) = x;	V(1) = y;	V(2) = pFunc(x, y);
			VecMatrix.push_back(V);
		}
		MatrF.push_back(VecMatrix);
	}
}
//-------------------------------------------------------------------------------
void CPlot3D::SetMatrF(CMasMatrix& Matr)
// Задает значение матрицы MatrF извне
// Matr - новое значение для MatrF
{
	CVecMatrix VecMatrix;
	CMatrix V(4);
	double xmin, xmax, ymin, ymax;
	pFunc = NULL;
	MatrF.clear();
	MatrView.clear();
	MatrWindow.clear();
	V = Matr[0][0];
	xmin = xmax = V(0);
	ymin = ymax = V(1);
	for (int i = 0; i < Matr.size(); i++)
	{
		VecMatrix.clear();
		for (int j = 0; j < Matr[i].size(); j++)
		{
			V = Matr[i][j];
			VecMatrix.push_back(V);
			double x = V(0);
			double y = V(1);
			if (x < xmin)xmin = x;
			if (x > xmax)xmax = x;
			if (y < ymin)ymin = y;
			if (y > ymax)ymax = y;
		}
		MatrF.push_back(VecMatrix);
	}
	SpaceRect.SetRectD(xmin, ymax, xmax, ymin);	// Определяем область
	CreateMatrView();
	CreateMatrWindow();
}


	//-------------------------------------------------------------------------------
int CPlot3D::GetNumberRegion()
//Определяет номер области  для  рисования.
{
	CMatrix CartPoint = SphereToCart(ViewPoint);	// Декартовы координаты точки наблюдения (3x1)
	double xView = CartPoint(0);					// x- координата точки наблюдения
	double yView = CartPoint(1);					// y- координата точки наблюдения
	double zView = CartPoint(2);					// z- координата точки наблюдения

	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;

	//-- Определяем где находится точка наблюдения относительно диагоналей области RectF:
	//-- получаем уравнение диагонали y1=y1(x) [точки (xL,yL)-(xH,yH)]и находим значение y1=y1(xView)  	
	double y1 = yL + (yH - yL) * (xView - xL) / (xH - xL);
	//-- получаем уравнение диагонали y2=y2(x) [точки (xL,yH)-(xH,yL)]и находим значение y2=y2(xView)  	
	double y2 = yH - (yH - yL) * (xView - xL) / (xH - xL);
	if ((yView <= y1) && (yView <= y2))return 1;
	if ((yView > y2) && (yView < y1))return 2;
	if ((yView >= y1) && (yView >= y2))return 3;
	if ((yView > y1) && (yView < y2))return 4;
}

//----------------------------------------------------------------------------------
void CPlot3D::CreateMatrView()
// Заполняет матрицу MatrView координатами точек проекции поверхности f(x,y)
// на плоскость XY видовой СК 
// !!! И !!!
// Определяет прямоугольная область ViewRect, охватывающая проекцию 
// поверхности на плоскость XY видовой СК
{
	CMatrix MV = CreateViewCoord(ViewPoint(0), ViewPoint(1), ViewPoint(2));	//Матрица(4x4) пересчета  
																		//в видовуюсистему координат 
	CVecMatrix VecMatrix;
	CMatrix VX(4), V(3);
	V(2) = 1;
	double xmin = DBL_MAX;	// DBL_MAX - максимальное из double
	double xmax = DBL_MIN;	// DBL_MIN - минимальное из double
	double ymin = DBL_MAX;
	double ymax = DBL_MIN;

	for (int i = 0; i < MatrF.size(); i++)
	{
		VecMatrix.clear();
		for (int j = 0; j < MatrF[i].size(); j++)
		{
			VX = MatrF[i][j];
			VX = MV * VX;
			V(0) = VX(0); // x- координата проекции
			V(1) = VX(1);	// y- координата проекции
			VecMatrix.push_back(V);

			//------- Для определения области ViewRect --------------------------
			double x = V(0);
			double y = V(1);
			if (x < xmin)xmin = x;
			if (x > xmax)xmax = x;
			if (y < ymin)ymin = y;
			if (y > ymax)ymax = y;
			// ------------------------------------------------------------------
		}
		MatrView.push_back(VecMatrix);
	}
	ViewRect.SetRectD(xmin, ymax, xmax, ymin);	// Определяем область
}

//-------------------------------------------------------------------

void CPlot3D::CreateMatrWindow()
// Заполняет матрицу MatrWindow оконными координатами точек изображения
{
	CMatrix MW = SpaceToWindow(ViewRect, WinRect);		// Матрица (3 на 3)пересчета 
													//в оконную систему координат
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

//-------------------------------------------------------------------

void CPlot3D::Draw(CDC& dc)
{
	if (MatrWindow.empty())
	{
		TCHAR* error = _T("Массив данных для рисования в окне пуст! ");
		MessageBox(NULL, error, _T("Ошибка"), MB_ICONSTOP);
		return;
	}
	CPoint pt[4];
	int kRegion = GetNumberRegion();	// kRegion=1,2,3,4
	int nRows = MatrWindow.size();
	int nCols = MatrWindow[0].size();  // Все строки одинаковой длины!!!
	switch (kRegion)
	{//
	case 1:
	{
		for (int j = nCols - 1; j > 0; j--)
			for (int i = 0; i < nRows - 1; i++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j - 1];
				pt[2] = MatrWindow[i + 1][j - 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
		break;
	}
	case 2:
	{
		for (int i = 0; i < nRows - 1; i++)
			for (int j = 0; j < nCols - 1; j++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
		break;
	}
	case 3:
	{
		for (int j = 0; j < nCols - 1; j++)
			for (int i = 0; i < nRows - 1; i++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
		break;
	}
	case 4:
	{
		for (int i = nRows - 1; i > 0; i--)
			for (int j = 0; j < nCols - 1; j++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i - 1][j + 1];
				pt[3] = MatrWindow[i - 1][j];
				dc.Polygon(pt, 4);
			}
		break;
	}
	}//
}