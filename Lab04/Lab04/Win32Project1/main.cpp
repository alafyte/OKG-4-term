#include "CMatrix.h"
#include "CPyramid.h"
#include <afxwin.h>
#include <windows.h>

#define ID_Dr1 2002
#define ID_Dr 2003


class CMainWin : public CFrameWnd
{
public:
	CMainWin()
	{
		Create(NULL, L"Lab04");
		Index = 0;
		PView.RedimMatrix(3);
	}
	CMenu *menu;
	CRect WinRect;
	CPyramid PIR;
	CMatrix PView;
	int Index;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Dr1();
	void Dr();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp() {};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMainWin();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_Dr1, Dr1)
	ON_COMMAND(ID_Dr, Dr)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

afx_msg void CMainWin::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1) PIR.Draw(dc, PView, WinRect);
	if (Index == 2) PIR.Draw1(dc, PView, WinRect);

}

afx_msg int CMainWin::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	this->menu->AppendMenu(MF_STRING, ID_Dr, _T("Только видимые"));
	this->menu->AppendMenu(MF_STRING, ID_Dr1, _T("Все грани"));
	SetMenu(menu);
	return 0;
}

void CMainWin::Dr1()
{

	PView(0) = 30;
	PView(1) = 30;
	PView(2) = 60;
	Index = 1;
	Invalidate();
}

void CMainWin::Dr()
{

	PView(0) = 30;
	PView(1) = 30;
	PView(2) = 60;
	Index = 2;
	Invalidate();
}
afx_msg void CMainWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Index == 1 || Index == 2)
	{
		switch (nChar)
		{
			case VK_UP:
			{
				double d = PView(2) - 2;
				if (d >= 0)
				{
					PView(2) = d;
				}
				break;
			}
			case VK_DOWN:
			{
				double d = PView(2) + 2;
				if (d <= 180)
				{
					PView(2) = d;
				}
				break;
			}
			case VK_LEFT:		// вращение вокруг оси z
			{
				double d = PView(1) - 2;
				if (d >= 0)
				{
					PView(1) = d;
				}
				else
				{
					PView(1) = 360 + d;
				}
				break;
			}
			case VK_RIGHT:		// вращение вокруг оси z
			{
				double d = PView(1) + 2;
				if (d <= 360)
				{
					PView(1) = d;
				}
				else
				{
					PView(1) = d - 360;
				}
				break;
			}
		}
		Invalidate();
	}
}

afx_msg void CMainWin::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(cx * 0.5, -cy * 0.3, cx * 0.9, cy * 0.1);
}

CMyApp theApp;