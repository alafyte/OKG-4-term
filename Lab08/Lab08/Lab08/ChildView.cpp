#include "stdafx.h"
#include "Lab08.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Bezier.h"
#include "Lagrange.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF Color = RGB(255, 40, 40);
CChildView::CChildView()
{
	Index = 0;
}
CChildView::~CChildView()
{
}
BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_32780, &CChildView::OnBezier)
	ON_COMMAND(ID_32781, &CChildView::OnLagrange)
END_MESSAGE_MAP()
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1)
	{
		PenLine.Set(PS_SOLID, 0, RGB(0, 255, 0));
		Graph.SetPenLine(PenLine);
		Graph.Draw(dc, 1, 1);

		// Вторая линия линия		
		PenLine.Set(PS_SOLID, 2, RGB(255, 0, 0));
		Graph.SetPenLine(PenLine);
		Graph.DrawBezier(dc, N_Bezier);
	}
	if (Index == 2)
	{
		Graph.Draw(dc, 1, 1);
	}
}

void CChildView::OnBezier()
{
	Index = 1;
	double dt = pi / 4;
	int N = 4;
	X.RedimMatrix(N);
	Y.RedimMatrix(N);
	for (int i = 0; i < N; i++)
	{
		X(i) = i * dt;
		Y(i) = pow(2 + cos(i * dt), sin(2 * i * dt));
	}
	N_Bezier = 50;
	RW.SetRect(100, 50, 500, 350);
	Graph.SetParams(X, Y, RW);
	this->Invalidate();
}

double CChildView::MyF1(double x)
{
	double y = pow(2 + cos(x), sin(2 * x));
	return y;
}

void CChildView::OnLagrange()
{
	double dx = 0.2;
	double xL = 0;
	double xH = 10;
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 1, RGB(0, 255, 0));
	RW.SetRect(100, 50, 500, 450);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 2;
	this->Invalidate();
}
