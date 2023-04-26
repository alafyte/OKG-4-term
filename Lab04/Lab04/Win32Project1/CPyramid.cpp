#include "CPyramid.h"
#include <iostream>
#define pi 3.14159265;

CPyramid::CPyramid()
{
	this->Vertices.RedimMatrix(4, 6);	// создание матрицы с координатами точек пирамиды
	//	A	A1	B	B1	C	C1	
	//	3	1	0	0	0	0
	//	0	0	0	0	3	1
	//	0	3	0	3	0	3
	//	1	1	1	1	1	1
	this->Vertices(0, 0) = 3;		// A (ось х)
	this->Vertices(0, 1) = 1;		// A1 (ось х)
	this->Vertices(2, 1) = 3;		// A1 (ось z)
	this->Vertices(2, 3) = 3;		// B1 (ось z)
	this->Vertices(1, 4) = 3;		// C (ось y)
	this->Vertices(1, 5) = 1;		// C1 (ось у)
	this->Vertices(2, 5) = 3;		// C1 (ось z)
	for (int i = 0; i < 6; i++)
	{
		this->Vertices(3, i) = 1;
	}
}

CMatrix CreateViewCoord(double r, double fi, double q)			// МСК -> ВСК
{
	// сферические координаты точки наблюдения
	double fi_sphere = (fi / 180) * pi;		// азимут
	double q_sphere = (q / 180) * pi;		// зенит

	CMatrix K(4, 4);
	K(0, 0) = -sin(fi_sphere);
	K(0, 1) = cos(fi_sphere);
	K(1, 0) = -cos(q_sphere) * cos(fi_sphere);
	K(1, 1) = -cos(q_sphere) * sin(fi_sphere);
	K(1, 2) = sin(q_sphere);
	K(2, 0) = -sin(q_sphere) * cos(fi_sphere);
	K(2, 1) = -sin(q_sphere) * sin(fi_sphere);
	K(2, 2) = -cos(q_sphere);
	K(2, 3) = r;
	K(3, 3) = 1;
	return K;
}


void CPyramid::Draw1(CDC &dc, CMatrix&PView, CRect &RW)			// с удалением ребер
{
	CMatrix XV = CreateViewCoord(PView(0), PView(1), PView(2));		// получаем матрицу пересчета
	CMatrix ViewVert = XV * this->Vertices;							// Kview * PIR
	CRectD RectView;
	GetRect(this->Vertices, RectView);								// задание области

	CMatrix MW = SpaceToWindow(RectView, RW);
	CPoint MasVert[6], A1B1C1[3], ABC[3];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}
	ABC[0] = MasVert[0];
	ABC[1] = MasVert[2];
	ABC[2] = MasVert[4];
	A1B1C1[0] = MasVert[1];
	A1B1C1[1] = MasVert[3];
	A1B1C1[2] = MasVert[5];

	//CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	//CPen *pOldPen = dc.SelectObject(&Pen);
	//CBrush BottopBrush(RGB(34, 245, 206));				// цвет нижней грани
	//CBrush TopBrush(RGB(5, 192, 34));					// цвет верхней грани
	//CBrush BaseBrush(RGB(251, 67, 105));				// цвет остальных граней
	//CBrush *pOldBrush = dc.SelectObject(&BottopBrush);
	//dc.SelectObject(&BaseBrush);
	CPoint ABB1A1[4];
	ABB1A1[0] = ABC[0];
	ABB1A1[1] = ABC[1];
	ABB1A1[2] = A1B1C1[1];
	ABB1A1[3] = A1B1C1[0];

	CPoint BCB1C1[4];
	BCB1C1[0] = ABC[1];
	BCB1C1[1] = ABC[2];
	BCB1C1[2] = A1B1C1[2];
	BCB1C1[3] = A1B1C1[1];

	CPoint ACC1A1[4];
	ACC1A1[0] = ABC[2];
	ACC1A1[1] = ABC[0];
	ACC1A1[2] = A1B1C1[0];
	ACC1A1[3] = A1B1C1[2];

	if (PView(1) < 90)				// если азимут < 90
	{
		dc.Polygon(ABB1A1, 4);
		dc.Polygon(ACC1A1, 4);
	}
	else
	{
		if (PView(1) < 180)			// если азимут 90 < х < 180
		{
			dc.Polygon(ACC1A1, 4);
			dc.Polygon(BCB1C1, 4);
		}
		else
			if (PView(1) < 270)		// если азимут 180 < х < 270
			{
				dc.Polygon(ACC1A1, 4);
				dc.Polygon(ABB1A1, 4);
				dc.Polygon(BCB1C1, 4);
			}
			else					// если азимут 270 < х < 360
			{
				dc.Polygon(ACC1A1, 4);
				dc.Polygon(ABB1A1, 4);
			}
	}
	if (PView(2) <= 10 && PView(1) > 180 && PView(1) < 270)
	{
		dc.Polygon(ACC1A1, 4);
		dc.Polygon(ABB1A1, 4);
		dc.Polygon(BCB1C1, 4);
	}

	if (PView(2) <= 90)					// если зенит <= 90
	{
		/*dc.SelectObject(&TopBrush);*/
		dc.Polygon(A1B1C1, 3);				// рисуется грань A1B1C1
	}
	else								// если зенит > 90
	{
		/*dc.SelectObject(&BottopBrush);*/
		dc.Polygon(ABC, 3);					// рисуется грань ABC
	}

	/*dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);*/
}

void CPyramid::Draw(CDC &dc, CMatrix&PView, CRect &RW)			// без удаления граней
{
	CMatrix XV = CreateViewCoord(PView(0), PView(1), PView(2));
	CMatrix ViewVert = XV * this->Vertices;
	CRectD RectView;
	GetRect(Vertices, RectView);
	CMatrix MW = SpaceToWindow(RectView, RW);
	CPoint MasVert[6], a1b1c1[3], abc[3];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i);
		V(1) = ViewVert(1, i);
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}
	abc[0] = MasVert[0];
	abc[1] = MasVert[2];
	abc[2] = MasVert[4];
	a1b1c1[0] = MasVert[1];
	a1b1c1[1] = MasVert[3];
	a1b1c1[2] = MasVert[5];
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen *pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(abc[0]);
	dc.LineTo(abc[1]);
	dc.MoveTo(abc[0]);
	dc.LineTo(abc[2]);
	dc.MoveTo(abc[2]);
	dc.LineTo(abc[1]);

	dc.MoveTo(a1b1c1[0]);
	dc.LineTo(a1b1c1[1]);
	dc.MoveTo(a1b1c1[0]);
	dc.LineTo(a1b1c1[2]);
	dc.MoveTo(a1b1c1[2]);
	dc.LineTo(a1b1c1[1]);

	dc.MoveTo(abc[0]);
	dc.LineTo(a1b1c1[0]);
	dc.MoveTo(abc[1]);
	dc.LineTo(a1b1c1[1]);
	dc.MoveTo(abc[2]);
	dc.LineTo(a1b1c1[2]);

	dc.SelectObject(pOldPen);
}

void  CPyramid::GetRect(CMatrix& Vert, CRectD&RectView)
{
	RectView.top = Vert.GetRow(2).MinElement();
	RectView.bottom = Vert.GetRow(2).MaxElement();
	RectView.left = Vert.GetRow(0).MinElement();
	RectView.right = Vert.GetRow(0).MaxElement();
}

CMatrix VectorMult(CMatrix& V1, CMatrix& V2)
{
	if (V1.rows() != V2.rows() || V1.cols() > 1 || V2.cols() > 1) // Число столбцов больше одного 
	{
		char* error = "CMatrix VectorMult(CMatrix& V1, CMatrix& V2) объект не вектор - число столбцов больше 1 ";
		MessageBoxA(NULL, error, "Ошибка", MB_ICONSTOP);
		exit(1);
	}

	CMatrix Temp = V1;
	Temp(0) = V1(1)*V2(2) - V1(2)*V2(1);
	Temp(1) = V1(2)*V2(0) - V1(0)*V2(2);
	Temp(2) = V1(0)*V2(1) - V1(1)*V2(0);

	return Temp;
}

double ScalarMult(CMatrix& V1, CMatrix& V2)
{
	if (V1.rows() != V2.rows() || V1.cols() > 1 || V2.cols() > 1) // Число столбцов больше одного 
	{
		char* error = "double ScalarMult(CMatrix& V1, CMatrix& V2) объект не вектор - число столбцов больше 1 ";
		MessageBoxA(NULL, error, "Ошибка", MB_ICONSTOP);
		exit(1);
	}

	return V1(0)*V2(0) + V1(1)*V2(1) + V1(2)*V2(2);
}