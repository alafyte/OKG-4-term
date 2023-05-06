#pragma once

class CChildView : public CWnd
{
public:
	CChildView();
	double MyF1(double x);

public:
	
	CRect RW;
	CPlot2D Graph;
	CMatrix X, Y;
	int N_Bezier;
	CMyPen PenLine, PenAxis;
	int Index;

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CChildView();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBezier();
	afx_msg void OnLagrange();
};

