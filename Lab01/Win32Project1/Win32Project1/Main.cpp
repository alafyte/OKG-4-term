#include "LibGraph.h"
#include <string>
#define ID_M 2002
#define ID_F 2004

class CMyMainWnd : public CFrameWnd
{
public:
	CMyMainWnd()
	{
		Create(NULL, L"Lab01 \"CMatrix\"");
	}
	CMenu *menu;
	afx_msg int OnCreate(LPCREATESTRUCT);
	void Matrix();
	void Functions();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp(){};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMyMainWnd();
		m_pMainWnd->ShowWindow(SW_SHOW);
		
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMyMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_M, Matrix)
	ON_COMMAND(ID_F, Functions)
END_MESSAGE_MAP()

afx_msg int CMyMainWnd::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu *subMenu = new CMenu();
	subMenu->CreatePopupMenu();
	subMenu->AppendMenu(MF_STRING, ID_M, _T("Matrix"));
	subMenu->AppendMenu(MF_STRING, ID_F, _T("Functions"));
	this->menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)subMenu->m_hMenu, _T("Tests"));
	SetMenu(menu);
	return 0;
}

void CMyMainWnd::Matrix()
{
	InvalidateRect(0);
	UpdateWindow();
	CClientDC dc(this);
	CChildView M;

	// вывод исходных матриц и векторов
	int y_out = 160;
	int x_out = 800;
	dc.TextOut(x_out, y_out + 32, "A = ");
	PrintMatrix(dc, x_out + 35, y_out, *M.A);
	dc.TextOut(x_out + 250, y_out + 32, "B = ");
	PrintMatrix(dc, x_out + 285, y_out, *M.B);

	dc.TextOut(x_out, (y_out += 120) + 32, "V1 = ");
	PrintMatrix(dc, x_out + 35, y_out, *M.V1);
	dc.TextOut(x_out + 250, y_out + 32, "V2 = ");
	PrintMatrix(dc, x_out + 285, y_out, *M.V2);

	// C1 = A + B
	y_out = 30;
	x_out = 10;
	dc.TextOut(x_out + 70, y_out - 25, "C1 = A + B");
	PrintMatrix(dc, x_out, y_out, *M.A);
	dc.TextOut(x_out += 185, y_out + 32, "+");
	PrintMatrix(dc, x_out += 30, y_out, *M.B);
	dc.TextOut(x_out += 185, y_out + 32, "=");
	CMatrix C1 = *M.A + *M.B;
	PrintMatrix(dc, x_out += 30, y_out, C1);

	// C2 = A * B
	x_out = 10;
	y_out += 130;
	dc.TextOut(x_out + 70, y_out - 25, "C2 = A * B");
	PrintMatrix(dc, x_out, y_out, *M.A);
	dc.TextOut(x_out += 185, y_out + 32, "*");
	PrintMatrix(dc, x_out += 30, y_out, *M.B);
	dc.TextOut(x_out += 185, y_out + 32, "=");
	CMatrix C2 = *M.A * *M.B;
	PrintMatrix(dc, x_out += 30, y_out, C2);

	// D = A * V1
	x_out = 10;
	y_out += 130;
	dc.TextOut(x_out + 70, y_out - 25, "D = A * V1");
	PrintMatrix(dc, x_out, y_out, *M.A);
	dc.TextOut(x_out += 185, y_out + 32, "*");
	PrintMatrix(dc, x_out += 30, y_out, *M.V1);
	dc.TextOut(x_out += 70, y_out + 32, "=");
	CMatrix D = (*M.A) * (*M.V1);
	PrintMatrix(dc, x_out += 30, y_out, D);
	CMatrix q, p;

	// q = v1(t) * v2
	x_out = 10;
	y_out += 130;
	dc.TextOut(x_out + 70, y_out - 25, "q = V1(T) * V2");
	PrintMatrix(dc, x_out, y_out + 32, (*M.V1).Transp());
	dc.TextOut(x_out += 185, y_out + 32, "*");
	PrintMatrix(dc, x_out += 30, y_out, *M.V2);
	dc.TextOut(x_out += 70, y_out + 32, "=");
	q = (*M.V1).Transp() * (*M.V2);
	PrintMatrix(dc, x_out += 30, y_out + 32, q);

	// p = v1(t) * A * v2
	x_out = 10;
	y_out += 130;
	dc.TextOut(x_out + 70, y_out - 25, "p = V1(T) * A * V2");
	PrintMatrix(dc, x_out, y_out + 32, (*M.V1).Transp());
	dc.TextOut(x_out += 185, y_out + 32, "*");
	PrintMatrix(dc, x_out += 30, y_out, *M.A);
	dc.TextOut(x_out += 185, y_out + 32, "*");
	PrintMatrix(dc, x_out += 30, y_out, *M.V2);
	dc.TextOut(x_out += 70, y_out + 32, "=");
	p = (*M.V1).Transp() * (*M.A) * (*M.V2);
	PrintMatrix(dc, x_out += 30, y_out + 32, p);

}
void CMyMainWnd::Functions()
{
	char t[10];
	InvalidateRect(0);
	UpdateWindow();
	CClientDC dc(this);
	CChildView M;
	// вывод исходных векторов
	int y_out = 20;
	int x_out = 600;
	dc.TextOut(x_out, (y_out += 120) + 32, "V1 = ");
	PrintMatrix(dc, x_out + 40, y_out, *M.V1);
	dc.TextOut(x_out, (y_out += 120) + 32, "V2 = ");
	PrintMatrix(dc, x_out + 40, y_out, *M.V2);

	y_out = 30;
	x_out = 10;
	dc.TextOut(x_out + 70, y_out - 25, "VectorMult V1 * V2:");
	CMatrix Mult = VectorMult(*M.V1, *M.V2);
	PrintMatrix(dc, x_out, y_out, Mult);
	dc.TextOut(x_out + 70, (y_out += 120) - 25, "ScalarMult V1 * V2:");
	sprintf_s(t, "%3.2f", ScalarMult(*M.V1, *M.V2));
	dc.TextOut(x_out, y_out, t);

	dc.TextOut(x_out + 70, (y_out += 60) - 25, "ModVec V1");
	sprintf_s(t, "%f", ModVec(*M.V1));
	dc.TextOut(x_out, y_out, t);

	dc.TextOut(x_out + 70, (y_out += 60) - 25, "ModVec V2");
	sprintf_s(t, "%f", ModVec(*M.V2));
	dc.TextOut(x_out, y_out, t);

	dc.TextOut(x_out + 70, (y_out += 60) - 25, "Cos V1, V2:");
	sprintf_s(t, "%f", CosV1V2(*M.V1, *M.V2));
	dc.TextOut(x_out, y_out, t);

	dc.TextOut(x_out, (y_out += 60) - 25, "Spherical");
	dc.TextOut(x_out + 150, y_out - 25, "->");
	dc.TextOut(x_out + 200, y_out - 25, "Cartesian");

	CMatrix s(3);
	PrintMatrix(dc, x_out, y_out, s);
	dc.TextOut(x_out + 150, y_out + 32, "->");
	PrintMatrix(dc, x_out + 200, y_out, SphereToCart(s));
}
CMyApp theApp;