#include <vector>
#include <Windows.h>
#pragma once
using namespace std;

////////////// === ����������� ����� === /////////////////////////////////

typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;

double Surface1(double x, double y);
double Surface2(double x, double y);
double Surface3(double x, double y);

class CPlot3D
// ��� ����������� ����������� z=f(x,y)
{
	pfunc2 pFunc; // �������� �� ������� f(x,y), ����������� �����������
	CRectD SpaceRect; // ������������� �������, �� ������� ��������� ����������� � ������� ��
	CMasMatrix MatrF; // ������� ��� �������� ��������� ����� (x,y,z,1) ����������� � ���
	CMasMatrix MatrView; // �-�� ����� (x,y,1) �������� �� �� ���
	CRectD ViewRect; // ������� �������, ����� �������� �� �� ���
	CRect WinRect; // ������� ������� � ��� ��� ���������
	CMatrix ViewPoint; // �����. �.������. � ����������� ��� (r,fi,q)
	CMatrPoint MatrWindow; // �-�� ������� ����� �(xi,yi) ����� �����

public:
	CPlot3D();
	~CPlot3D(){ pFunc = NULL; };
	void SetFunction(pfunc2 pF, CRectD RS, double dx, double dy); // ������������� ������� f(x,y)
	void SetViewPoint(double r, double fi, double q); // ������������� ��������� ����� ���������� � ������� ��
	CMatrix GetViewPoint(); // ���������� ������ ViewPoin
	void SetWinRect(CRect Rect); // ������������� ������� � ���� ��� ���������
	void CreateMatrF(double dx, double dy);// ��������� ������� MatrF ������������ ����� �����������
	void CreateMatrView(); // ��������� ������� MatrView ������������ ����� �������� ����������� �� ��������� XY ������� ��
	void CreateMatrWindow(); // ��������� ������� MatrWindow
	void Draw(CDC& dc);
};
