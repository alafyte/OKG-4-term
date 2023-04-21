#include <vector>
#include <Windows.h>
#pragma once
using namespace std;

////////////// === ОПРЕДЕЛЕНИЕ ТИПОВ === /////////////////////////////////

typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;

double Surface1(double x, double y);
double Surface2(double x, double y);
double Surface3(double x, double y);

class CPlot3D
// для изображения поверхности z=f(x,y)
{
	pfunc2 pFunc; // указатеь на функцию f(x,y), описывающую поверхность
	CRectD SpaceRect; // Прямоугольная область, на которую опирается поверхность в мировой СК
	CMasMatrix MatrF; // Матрица для хранения координат точек (x,y,z,1) поверхности в МСК
	CMasMatrix MatrView; // М-ца точек (x,y,1) проекции на ХУ ВСК
	CRectD ViewRect; // Прямоуг область, охват проекцию на ХУ ВСК
	CRect WinRect; // Прямоуг область в ОСК для рисования
	CMatrix ViewPoint; // коорд. т.наблюд. в сферической МСК (r,fi,q)
	CMatrPoint MatrWindow; // М-ца оконных коорд Р(xi,yi) точек изобр

public:
	CPlot3D();
	~CPlot3D(){ pFunc = NULL; };
	void SetFunction(pfunc2 pF, CRectD RS, double dx, double dy); // Устанавливает функцию f(x,y)
	void SetViewPoint(double r, double fi, double q); // Устанавливает положение точки наблюдения в МИРОВОЙ СК
	CMatrix GetViewPoint(); // Возвращает вектор ViewPoin
	void SetWinRect(CRect Rect); // Устанавливает область в окне для рисования
	void CreateMatrF(double dx, double dy);// Заполняет матрицу MatrF координатами точек поверхности
	void CreateMatrView(); // Заполняет матрицу MatrView координатами точек проекции поверхности на плоскость XY видовой СК
	void CreateMatrWindow(); // Заполняет матрицу MatrWindow
	void Draw(CDC& dc);
};
