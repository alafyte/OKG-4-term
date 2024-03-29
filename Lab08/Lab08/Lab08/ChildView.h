
// ChildView.h: интерфейс класса CChildView
//


#pragma once


// Окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();

// Атрибуты
public:
	CRect RW;
	CPlot2D Graph;
	CMatrix X, Y;
	int N_Bezier;
	CMyPen PenLine, PenAxis;
	int Index;
// Операции
public:
// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	// действия при выборе пункта меню
	afx_msg void OnBezier();
	afx_msg void OnBezier1();
	afx_msg void OnBezier2();
	afx_msg void OnBezier3();
	afx_msg void OnBezier4();
	afx_msg void OnLagr();
	void Bezier();
	void Bezier2();
	void Bezier3();
	void Bezier4();
};
