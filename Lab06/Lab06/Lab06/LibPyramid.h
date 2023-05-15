#pragma once
double cosv1v2(CMatrix& one, CMatrix& two);
class CPyramid
{
private:
	CMatrix Vertices; // ���������� ������
	void GetRect(CMatrix& Vert, CRectD& RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
	void ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color);
};

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 6);	// ABC    � ������ ���������
								// A'B'C' � ������� ���������
	/*       A                   B                    C                   A'                  B'                  C'      */
	Vertices(0, 0) = 0;	Vertices(0, 1) = 0;  Vertices(0, 2) = 6; Vertices(0, 3) = 0; Vertices(0, 4) = 0; Vertices(0, 5) = 3;
	Vertices(1, 0) = 6; Vertices(1, 1) = 0;  Vertices(1, 2) = 0; Vertices(1, 3) = 3; Vertices(1, 4) = 0; Vertices(1, 5) = 0;
	Vertices(2, 0) = 0; Vertices(2, 1) = 0;  Vertices(2, 2) = 0; Vertices(2, 3) = 6; Vertices(2, 4) = 6; Vertices(2, 5) = 6;
	Vertices(3, 0) = 1; Vertices(3, 1) = 1;  Vertices(3, 2) = 1; Vertices(3, 3) = 1; Vertices(3, 4) = 1; Vertices(3, 5) = 1;
}

void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView) //����� ����-��, ����� �������� �������� �� XY ���
{
	CMatrix V = Vert.GetRow(0);               // x - ����������
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1);                       // y - ����������
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}

void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
// ������ �������� ��� �������� ��������� �����
// ��������������� �������� ��������� �� ������� � ������� (MM_TEXT)
// dc - ������ �� ����� CDC MFC
// P - ���������� ����� ���������� � ������� ����������� �������
// ���������
// (r,fi(����.), q(����.))
// RW - ������� � ���� ��� �����������	
{
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // ������� (4x4)
	CMatrix ViewVert = MV * Vertices; // ���������� ������ �������� � ������� ��
	CRectD RectView; // �������� - ������������ �������������
	GetRect(ViewVert, RectView); // �������� ������������ ������������� �� �-��� ������
	CMatrix MW = SpaceToWindow(RectView, RW); // ������� (3x3) ��� ��������� ��������� �� ������� � �������

	CPoint MasVert[6]; // ����� ������� ��������� ������,
	CMatrix V(3);
	V(2) = 1;
	// ���� �� ���������� ������ � ��������� ������� ���������
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x � �������
		V(1) = ViewVert(1, i); // y � �������
		V = MW * V;            // ������� ���������� �����
		MasVert[i].x = (int)V(0); // ������� �-�� �
		MasVert[i].y = (int)V(1); // ������� �-�� �
	}

	// ������
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(MasVert[3]);
	// ����� ������ �����
	for (int i = 0; i < 3; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	dc.LineTo(MasVert[0]);
	dc.MoveTo(MasVert[5]);
	// ����� ������� �����
	for (int i = 3; i < 6; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	// ����� ������� ������
	for (int i = 0; i < 3; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 3]);
	}
	dc.SelectObject(pOldPen);
}
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
// ������ �������� � ��������� ��������� �����
{
	CMatrix ViewCart = SphereToCart(PView); // ��������� ���������� ����� ����������
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // ������� (4x4) ��������� ����� �� ������� ������� ��������� � �������
	CMatrix ViewVert = MV * Vertices; // ���������� ������ �������� � ������� ��
	CRectD RectView;
	GetRect(ViewVert, RectView); // �������� ������������ �������������
	CMatrix MW = SpaceToWindow(RectView, RW); // ������� (3x3) ��� ���������

	CPoint MasVert[6]; // ����� ������� ��������� ������,
	CMatrix V(3);
	V(2) = 1;
	// ���� �� ���������� ������ � ��������� ������� ���������
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // ������� ���������� �����
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// ������
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 3; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 3, 0, 2); // ������� E
		int k;
		if (i == 2)
			k = 0;
		else
			k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // ������� ����� ���������
		R2 = Vertices.GetCol(k, 0, 2); // ��������� ����� ���������
		CMatrix V1 = R2 - R1;          // ������ � ����� � ���������
		CMatrix V2 = VE - R1;          // ������ � ����� � �������
		VN = VectorMult(V2, V1);       // ������ ������� �������
		sm = ScalarMult(VN, ViewCart); // ��������� ������������ 

		if (sm >= 0) // ����� ������ � ������ ������� �����
		{
			dc.MoveTo(MasVert[i]);
			dc.LineTo(MasVert[k]);
			dc.LineTo(MasVert[k + 3]);
			dc.LineTo(MasVert[i + 3]);
			dc.LineTo(MasVert[i]);
		}
	}

	VN = VectorMult(R1, R2);
	sm = ScalarMult(VN, ViewCart);
	if (sm >= 0) // ���������
	{
		dc.Polygon(MasVert, 3);
	}
	else
	{
		CBrush* topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 3, 3);	// ������� ���������
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color)
{
	BYTE Red = GetRValue(color);
	BYTE Green = GetGValue(color);
	BYTE Blue = GetBValue(color);

	CMatrix ViewCart = SphereToCart(PView);
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));	//�-�� ���->���
	CMatrix ViewVert = MV * Vertices;
	CRectD RectView;
	GetRect(ViewVert, RectView); //����-�, ����� �������� �������� �� XY ���

	CMatrix MW = SpaceToWindow(RectView, RW); // �-�� ���->���
	CPoint MasVert[6];
	CMatrix V(3);
	V(2) = 1;
	for (int i = 0; i < 6; i++)	///���� � ���
	{
		V(0) = ViewVert(0, i); ///x
		V(1) = ViewVert(1, i); ///y
		V = MW * V;
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	CMatrix R1(3),	///���. ����� ���������
		R2(3),		///��������� �.���������
		VN(3);		///������ �������
	double sm;
	for (int i = 0; i < 3; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 3, 0, 2);	// ������� �
		int k;
		if (i == 2) k = 0;
		else k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2);
		R2 = Vertices.GetCol(k, 0, 2);
		CMatrix V1 = R2 - R1;          // ������ � ����� � ���������
		CMatrix V2 = VE - R1;          // ������ � ����� � �������
		VN = VectorMult(V2, V1);
		sm = cosv1v2(VN, ViewCart);	   // cos (N � �����, ���� ��)

		if (sm >= 0) // ����� ������ � ������ ������� �����
		{
			CPen Pen(PS_SOLID, 2, RGB(sm * sm * Red, sm * sm * Green, sm * sm * Blue));	//������
			CPen* pOldPen = dc.SelectObject(&Pen);
			CBrush Brus(RGB(sm * Red, sm * Green, sm * Blue));
			CBrush* pOldBrush = dc.SelectObject(&Brus);
			CPoint MasVertr[4]{ MasVert[i], MasVert[k], MasVert[k + 3],MasVert[i + 3] };
			dc.Polygon(MasVertr, 4);	///���������
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
		}
	}
	VN = VectorMult(R1, R2);
	sm = cosv1v2(VN, ViewCart);

	if (sm >= 0) // ���������
	{
		CBrush* topBrush = new CBrush(RGB(sm * Red, sm * Green, sm * Blue));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert, 3);	// ���������
	}
	else
	{
		CBrush* topBrush = new CBrush(RGB(sm * sm * Red, sm * sm * Green, sm * sm * Blue));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 3, 3);	// ������� ���������
	}
}

double cosv1v2(CMatrix& V1, CMatrix& V2)
{

	double modV1 = ModVec(V1);
	double modV2 = ModVec(V2);
	int b = (modV1 < 1e-7) || (modV2 < 1e-7);
	if (b)
	{
		TCHAR* error = L"CosV1V2: ������ ������ ��� ����� �������� < 1e-7!";
		MessageBox(NULL, error, L"������", MB_ICONSTOP);
		exit(1);
	}
	int b1 = (V1.cols() == 1) && (V1.rows() == 3);
	int b2 = (V2.cols() == 1) && (V2.rows() == 3);
	b = b1 && b2;
	if (!b)
	{
		TCHAR* error = L"CosV1V2: ������������ ����������� ��������! ";
		MessageBox(NULL, error, L"������", MB_ICONSTOP);
		exit(1);
	}
	double cos_f = ScalarMult(V1, V2) / (modV1 * modV2);
	return cos_f;
}