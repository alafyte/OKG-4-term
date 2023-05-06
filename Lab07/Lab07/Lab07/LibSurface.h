using namespace std;


typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;


typedef double(*pfunc2)(double, double);// Указатель на функцию


class CPlot3D
{
private:
	pfunc2 pFunc;          // Указатель на функцию f(x,y), описывающая поверхность 
	CRectD SpaceRect;      // Прямоугольник области, на которую опирается поверхность
	CMasMatrix MatrF;      // Матрица для хранения координат точек (x,y,z,1) поверхности
	CMasMatrix MatrView;   // Матрица для хранения координат точек (x,y,1) проекции XY видовой СК
	CRectD ViewRect;       // Прямоугольная область, охватывающая проекцию поверхности видовой СК
	CRect WinRect;         // Прямоугольная область в оконной системе координат для
	CMatrix ViewPoint;     // Вектор (3x1) – координаты точки наблюдения в мировой сферической СК (r,fi,q)
	CMatrPoint MatrWindow; // Матрица для хранения оконных координат

public:
	CPlot3D();
	~CPlot3D() { pFunc = NULL; }
	void SetFunction(pfunc2 pF, CRectD RS, double dx, double dy);
	void SetViewPoint(double r, double fi, double q);

	CMatrix GetViewPoint();                 // Возвращает вектор ViewPoint
	void SetWinRect(CRect Rect);            // Устанавливает область в окне для рисования
	void CreateMatrF(double dx, double dy); // Заполняет матрицу MatrF координатами
	void SetMatrF(CMasMatrix &Matr);        // Задает значение матрицы MatrF извне
	void CreateMatrView();                  // Заполняет матрицу MatrView координатами точек проекции XY видовой СК
	void CreateMatrWindow();                // Заполняет матрицу MatrWindow
	int GetNumberRegion();                  // Определяет номер области для рисования
	void Draw(CDC& dc);
};


double Function1(double x, double y)
{
	return x*x + y*y;
}
double Function2(double x, double y)
{
	return x*x - y*y;
}
double Function3(double x, double y)
{
	//return sqrt(1 - x*x - y*y);
	double r = sqrt(x*x + y*y);
	double z;
	if (r < 0.001)
		z = 10;
	else
		z = 10 * sin(r) / r;
	return z;
}


CPlot3D::CPlot3D()
{
	pFunc = NULL;
	ViewPoint.RedimMatrix(3);
	WinRect.SetRect(0, 0, 200, 200);
	ViewPoint(0) = 10, ViewPoint(1) = 30, ViewPoint(2) = 45;
}
void CPlot3D::SetFunction(pfunc2 pF, CRectD RS, double dx, double dy)
// Устанавливает указатель на фукцию f(x,y)
// pFunc – указатель на функцию f(x,y) – поверхность
// RS – область  МСК (xmin, ymax, xmax, ymin) для расчета
// dx, dy – шаги для расчета значений f(x) по x и y
// VP – координаты точки наблюдения (r,fi,q) в сферической СК
// RW – область в окне для рисования
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
void CPlot3D::SetViewPoint(double r, double fi, double q)
// Устанавливает положение точки наблюдения в МИРОВОЙ СК
// –заполняет матрицу ViewPoint
// r – модуль радиус-вектора
// fi – азимут (от оси OX против часовой стрелки)
// q – угол (от оси Z по часовой стрелке, 0-180)
{
	ViewPoint(0) = r, ViewPoint(1) = fi, ViewPoint(2) = q;
	MatrView.clear();
	CreateMatrView();
	MatrWindow.clear();
	CreateMatrWindow();
}
CMatrix CPlot3D::GetViewPoint()
{
	CMatrix P = ViewPoint;
	return P;
}
void CPlot3D::SetWinRect(CRect Rect)
{
	WinRect = Rect;
	MatrWindow.clear();
	CreateMatrWindow();
}
void CPlot3D::CreateMatrF(double dx, double dy)
// Заполняет матрицу MatrF координатами точек поверхности
// dx – шаг для расчета значений функции f(x,y) оси
// dy – шаг для расчета значений функции f(x,y) оси
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
			V(0) = x; V(1) = y; V(2) = pFunc(x, y);
			VecMatrix.push_back(V);
		}
		MatrF.push_back(VecMatrix);
	}
}
void CPlot3D::SetMatrF(CMasMatrix &Matr)
// Задает значение матрицы MatrF извне
// Matr – новове значение для MatrF
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
			if (x < xmin) xmin = x;
			if (x < xmax) xmax = x;
			if (y < ymin) ymin = y;
			if (y < ymax) ymax = y;
		}
		MatrF.push_back(VecMatrix);
	}
	SpaceRect.SetRectD(xmin, ymax, xmax, ymin);
	CreateMatrView();
	CreateMatrWindow();
}
int CPlot3D::GetNumberRegion()
// Определяет номер области для рисования
{
	CMatrix CartPoint = SphereToCart(ViewPoint);
	double xView = CartPoint(0);
	double yView = CartPoint(1);
	double zView = CartPoint(2);

	double xL = SpaceRect.left;
	double xH = SpaceRect.right;
	double yL = SpaceRect.bottom;
	double yH = SpaceRect.top;

	// Определяем где нахоится точка наблюдения относительно
	// получаем уравнение диагонали y1=y1(x) [точки (x)]
	double y1 = yL + (yH - yL)*(xView - xL) / (xH - xL);
	// получаем уравнение диагонали y2=y2(x) [точки (x)]
	double y2 = yH + (yH - yL)*(xView - xL) / (xH - xL);
	if ((yView <= y1) && (yView <= y2)) 
		return 1;
	if ((yView > y2) && (yView < y1)) 
		return 2;
	if ((yView >= y1) && (yView >= y2)) 
		return 3;
	if ((yView > y1) && (yView < y2)) 
		return 4;
}
void CPlot3D::CreateMatrView()
// Заполняет матрицу MatrView координатами точек проекции поверхности f(x,y)
// на плоскость XY видовой СК
// И
// Определяет прямоугольная область ViewRect, охватывающая проекцию
// поверхности на плоскость XY видовой СК
{
	CMatrix MV = CreateViewCoord(ViewPoint(0), ViewPoint(1), ViewPoint(2));
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
		for (int j = 0; j < MatrF[i].size(); j++)
		{
			VX = MatrF[i][j];
			VX = MV*VX;
			V(0) = VX(0); // x – координата проекции
			V(1) = VX(1); // y – координата проекции
			VecMatrix.push_back(V);

			// Для определения области ViewRect
			double x = V(0);
			double y = V(1);
			if (x < xmin) xmin = x;
			if (x > xmax) xmax = x;
			if (y < ymin) ymin = y;
			if (y < ymax) xmin = y;
		}
		MatrView.push_back(VecMatrix);
	}
	ViewRect.SetRectD(xmin, ymax, xmax, ymin);
}
void CPlot3D::CreateMatrWindow()
// Заполняет матрицу MatrWindow оконными координатами точки изображения
{
	CMatrix MW = SpaceToWindow(ViewRect, WinRect);

	CVecPoint VecPoint;
	CMatrix V(3);
	for (int i = 0; i < MatrView.size(); i++)
	{
		VecPoint.clear();
		for (int j = 0; j < MatrView[i].size(); j++)
		{
			V = MatrView[i][j];
			V = MW*V;
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
		TCHAR* error = TEXT("Массив данных для рисования пуст");
		MessageBox(NULL, error, TEXT("Ошибка"), MB_ICONSTOP);
		return;
	}
	CPoint pt[4];
	int kRegion = GetNumberRegion();
	int nRows = MatrWindow.size();
	int nCols = MatrWindow[0].size();

	switch (kRegion)
	{
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
	} break;
	case 2:
	{
		for (int i = 0; i < nRows - 1; i++)
			for (int j = 0; i < nCols - 1; j++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	case 3:
	{
		for (int j = 0; j < nCols; j++)
			for (int i = 0; i < nRows - 1; i++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i + 1][j + 1];
				pt[3] = MatrWindow[i + 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	case 4:
	{
		for (int i = nRows - 1; i > 0; i--)
			for (int j = 0; i < nCols - 1; j++)
			{
				pt[0] = MatrWindow[i][j];
				pt[1] = MatrWindow[i][j + 1];
				pt[2] = MatrWindow[i - 1][j + 1];
				pt[3] = MatrWindow[i - 1][j];
				dc.Polygon(pt, 4);
			}
	} break;
	}
}


void DrawLightSphere(CDC& dc, double Radius, CMatrix& PView, CMatrix& PSourceLight, CRect RW, COLORREF Color, int Index)
// Index=0 –  Диффузионная модель отражения света
// Index=1 –  Зеркальная модель отражения света
{
	BYTE red = GetRValue(Color);
	BYTE green = GetGValue(Color);
	BYTE blue = GetBValue(Color);

	CMatrix VSphere(3), VSphereNorm(3), V0(3), PV(4);
	COLORREF Col;
	double df = 0.54;                              // Шаг по азимуту
	double dq = 0.54;                              // Шаг угла	
	double kLight;
	VSphere(0) = 3;
	CMatrix VR = SphereToCart(PView);				//т.наблюд в декарт
	CMatrix VS = SphereToCart(PSourceLight);		//т.света в декарт
	CRectD  RV(-Radius, Radius, Radius, -Radius); // Обл проекции шара на XY в ВСК
	CMatrix MW = SpaceToWindow(RV, RW);		      // М-ца МСК -> ОСК
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));	//М-ца -> ВСК

	V0(0) = RV.left;	// в ОСК, xL 
	V0(1) = RV.bottom;	// в ОСК, yL
	///V0(2)=0 - в конструкторе	


	// Проходим по точкам сферы
	for (double fi = 0; fi < 360.0; fi += df)		  // Азимут
		for (double q = 0; q <= 180.0; q += dq)		  // q = 90 - угол места
		{
			VSphere(1) = fi;
			VSphere(2) = q;
			CMatrix VCart = SphereToCart(VSphere);	  // т.сферы в декарт                                     
			VSphereNorm = VCart;				      // В-р нормали
			double cos_RN = CosV1V2(VR, VSphereNorm); // cos (в-р т.наблюд. ; в-р N)
			
			if (cos_RN > 0) // Если точка видима наблюдателю
			{
				///дек -вск - оск
				PV(0) = VCart(0);
				PV(1) = VCart(1);
				PV(2) = VCart(2);
				PV(3) = 1;
				PV = MV*PV;         // точка сферы -> ВСК
				VCart(0) = PV(0);   // Xv
				VCart(1) = PV(1);   // Yv
				VCart(2) = 1;
				VCart = VCart - V0; // Xv-xL, Yv-yL, 1
				VCart = MW*VCart;   // точка сферы -> ОСК

				CMatrix VP = VS - VR;       // Напр на источник света отн нормали к т.падения
				double cos_PN = CosV1V2(VP, VSphereNorm); // cos угла падения луча
				
				
				if (cos_PN > 0) // Если точка сферы освещается
				{
					if (Index == 0)				///диффуз
						kLight = cos_PN;
					else if (Index == 1)		///зеркал
						kLight = CosV1V2(VSphereNorm, VS);

					Col = RGB(kLight*red, kLight*green, kLight*blue);
					dc.SetPixel((int)VCart(0), (int)VCart(1), Col);
				}
				else
				{
					Col = RGB(0, 0, 0);
					dc.SetPixel((int)VCart(0), (int)VCart(1), Col);
				}
			}
		}
}