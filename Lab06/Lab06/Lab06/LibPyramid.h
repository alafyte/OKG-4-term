#pragma once
#include <wingdi.h>
#include <wingdi.h>

class CPyramid
{
	CMatrix Vertices;	// ���������� ������
	CMatrix Nabcd;	// ������ ������� ������� � ��������� ��������

	void GetRect(CMatrix& Vert, CRectD& RectView);
public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
	void Draw2(CDC& dc, CMatrix& P, CRect& RW);
	void ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF Color);

};

