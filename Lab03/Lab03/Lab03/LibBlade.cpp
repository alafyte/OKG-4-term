#include "stdafx.h"

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
//------------------------------------------------------------------------------
void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

//------------------------------------------------------------------------------
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// ������ ������������� �������
	cz.cy = fabs(top - bottom);	// ������ ������������� �������
	return cz;
}

//----------------------------------------------------------------------------

CMatrix CreateTranslate2D(double dx, double dy)
// ��������� ������� ��� �������������� ��������� ������� ��� ��� �������� 
// �� dx �� ��� X � �� dy �� ��� Y � ������������� ������� ���������
// --- ��� ---
// ��������� ������� ��� �������������� ��������� ������� ��� �������� ������
// ������� ��������� �� -dx ��� X � �� -dy �� ��� Y ��� ������������� ��������� ������� 
{
	CMatrix TM(3, 3);
	TM(0, 0) = 1; TM(0, 2) = dx;
	TM(1, 1) = 1;  TM(1, 2) = dy;
	TM(2, 2) = 1;
	return TM;
}

//------------------------------------------------------------------------------------
CMatrix CreateRotate2D(double fi)
// ��������� ������� ��� �������������� ��������� ������� ��� ��� ��������
// �� ���� fi (��� fi>0 ������ ������� �������)� ������������� ������� ���������
// --- ��� ---
// ��������� ������� ��� �������������� ��������� ������� ��� �������� ������
// ������� ��������� �� ���� -fi ��� ������������� ��������� ������� 
// fi - ���� � ��������
{
	double fg = fmod(fi, 360.0);
	double ff = (fg / 180.0) * pi; // ������� � �������
	CMatrix RM(3, 3);
	RM(0, 0) = cos(ff); RM(0, 1) = -sin(ff);
	RM(1, 0) = sin(ff);  RM(1, 1) = cos(ff);
	RM(2, 2) = 1;
	return RM;
}


//------------------------------------------------------------------------------

CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
// ���������� ������� ��������� ��������� �� ������� � �������
// RS - ������� � ������� ����������� - double
// RW - ������� � ������� ����������� - int
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	 // ������ ������� � ����
	int dwx = sz.cx;	     // ������
	int dwy = sz.cy;	     // ������
	CSizeD szd = RS.SizeD(); // ������ ������� � ������� �����������

	double dsx = szd.cx;    // ������ � ������� �����������
	double dsy = szd.cy;    // ������ � ������� �����������

	double kx = (double)dwx / dsx;   // ������� �� x
	double ky = (double)dwy / dsy;   // ������� �� y

	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}

//------------------------------------------------------------------------------

void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
// ������������� ����� ����������� MM_ANISOTROPIC � ��� ���������
// dc - ������ �� ����� CDC MFC
// RS -  ������� � ������� ����������� - int
// RW -	 ������� � ������� ����������� - int  
{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
}

CBlade::CBlade()            
{
	double rS = 30;       
	double RoE = 10 * rS;	 
	double d = RoE;		    
	RS.SetRectD(-d, d, d, -d);					// ������� ������� � ������� �����������
	RW.SetRect(0, 0, 690, 640);					// ������� � ����
	MainPoint.SetRect(-rS, rS, rS, -rS);
	FirstTop.SetRect(-5, 5, 5, -5);
	SecondTop.SetRect(-5, 5, 5, -5);
	FirstBootom.SetRect(-5, 5, 5, -5);
	SecondBootom.SetRect(-5, 5, 5, -5);
	WayRotation.SetRect(-RoE, RoE, RoE, -RoE);	
	fiSB = 215;
	fiFB = 205;			
	fiST = 25;			
	fiFT = 35;//���� ��������� ��� FT
	wPoint = -8;
	dt = 0.1;
	FTCoords.RedimMatrix(3);
	STCoords.RedimMatrix(3);
	FBCoords.RedimMatrix(3);
	SBCoords.RedimMatrix(3);
}

void CBlade::SetNewCoords()
{

	double RoV = (WayRotation.right - WayRotation.left) / 2;
	double ff = (fiFT / 90.0) * pi;
	double x = RoV * cos(ff);
	double y = RoV * sin(ff);
	FTCoords(0) = x;
	FTCoords(1) = y;
	FTCoords(2) = 1;

	fiFT += wPoint * dt;
	CMatrix P = CreateRotate2D(fiFT);
	FTCoords = P * FTCoords;

	RoV = (WayRotation.right - WayRotation.left) / 2;
	ff = (fiST / 90.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	STCoords(0) = x;
	STCoords(1) = y;
	STCoords(2) = 1;

	fiST += wPoint * dt;
	P = CreateRotate2D(fiST);
	STCoords = P * STCoords;

	RoV = (WayRotation.right - WayRotation.left) / 2;
	ff = (fiFB / 90.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	FBCoords(0) = x;
	FBCoords(1) = y;
	FBCoords(2) = 1;

	fiFB += wPoint * dt;
	P = CreateRotate2D(fiFB);
	FBCoords = P * FBCoords;

	RoV = (WayRotation.right - WayRotation.left) / 2;
	ff = (fiSB / 90.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	SBCoords(0) = x;
	SBCoords(1) = y;
	SBCoords(2) = 1;

	fiSB += wPoint * dt;
	P = CreateRotate2D(fiSB);
	SBCoords = P * SBCoords;
}


void CBlade::Draw(CDC& dc)
{
	CBrush SBrush, EBrush, MBrush, VBrush, * pOldBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(0, 0, 255));
	EBrush.CreateSolidBrush(RGB(0, 0, 255));
	MBrush.CreateSolidBrush(RGB(0, 255, 0));
	VBrush.CreateSolidBrush(RGB(128, 64, 64));


	dc.MoveTo(0, 0);
	dc.LineTo(FTCoords(0), FTCoords(1));
	DrawTriangle(FTCoords, STCoords, dc);
	DrawTriangle(FBCoords, SBCoords, dc);
	pOldBrush = dc.SelectObject(&SBrush);	// ���� �������
	dc.Ellipse(MainPoint);  

	dc.SelectObject(pOldBrush);				//��������������� �������� �� pOldBrush 
}
void CBlade::DrawTriangle(CMatrix FTCoords, CMatrix STCoords, CDC& dc) 
{
	CPen Pen(PS_SOLID, 1, RGB(255, 0, 0));
	float dy = max(FTCoords(1), STCoords(1)) - min(FTCoords(1), STCoords(1));
	float dx = max(FTCoords(0), STCoords(0)) - min(FTCoords(0), STCoords(0));
	float y = 0;
	float x1 = 0;
	int x2 = 0;
	dc.SelectObject(&Pen);
	if (FTCoords(0) < STCoords(0) && FTCoords(1) > STCoords(1)) 
	{
		if (dx > dy) 
		{
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) 
				{
					y += deltaY;
				}
			}
		}
		else 
		{
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) 
				{
					x1 += deltaX;
				}
			}
		}

	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) > STCoords(1)) {

		if (dx > dy) 
		{
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) 
				{
					y += deltaY;
				}
			}
		}
		else 
		{
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) 
				{
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) < STCoords(1)) 
	{
		if (dx > dy) 
		{
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) 
				{
					y += deltaY;
				}
			}
		}
		else 
		{
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) 
				{
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) < STCoords(0) && FTCoords(1) < STCoords(1)) 
	{
		if (dx > dy) 
		{
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) 
				{
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) 
			{
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) 
				{
					x1 += deltaX;
				}
			}
		}
	}
}


void CBlade::GetRS(CRectD& RSX)
// RS - ���������, ���� ������������ ��������� ������� �������
{
	RSX.left = RS.left;
	RSX.top = RS.top;
	RSX.right = RS.right;
	RSX.bottom = RS.bottom;
}







