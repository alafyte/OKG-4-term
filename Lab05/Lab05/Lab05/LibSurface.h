
#ifndef LIBSURFACE
#define LIBSURFACE 1


using namespace std;

////////////// === ����������� ����� === /////////////////////////////////


//typedef vector<int> CVecInt;
typedef vector<CMatrix> CVecMatrix;
typedef vector<CVecMatrix> CMasMatrix;

typedef vector<CPoint> CVecPoint;
typedef vector<CVecPoint> CMatrPoint;
// ��� pFunc2 ��������� � ����� LibGraph.h 


//----------------- �������� ������� -----------------------------------

double Function1(double x,double y);
double Function2(double x,double y);
double Function3(double x,double y);

//-------------------- class CPlot3D ----------------------------------
class CPlot3D
{
		pfunc2 pFunc;		    // ��������� �� ������� f(x,y), ����������� �����������
		CRectD SpaceRect;	    // ������������� �������, �� ������� ��������� ����������� � ������� ��
		CMasMatrix MatrF;		// ������� ��� ������� ��������� ����� (x,y,z,1) ����������� � ������� ��
		CMasMatrix MatrView;	// ������� ��� ������� ��������� ����� (x,y,1) �������� ����������� �� ��������� XY ������� ��
		CRectD ViewRect;		// ������������� �������, ������������ �������� ����������� �� ��������� XY ������� ��
		CRect WinRect;		    // ������������� ������� � ������� ������� ��������� ��� ���������		
		CMatrix ViewPoint;   // ������ (3x1) - ���������� ����� ���������� � ������� 										 
		                       // ����������� ������� ��������� (r,fi(����.), q(����.))
		CMatrPoint MatrWindow; // ������� ��� ������� ������� ��������� P(xi,yi) ����� ����������� 

	public:
		CPlot3D();
		~CPlot3D(){pFunc=NULL;};
		void SetFunction(pfunc2 pF,CRectD RS,double dx,double dy);	// ������������� ������� f(x,y)
		void SetViewPoint(double r,double fi,double q);	// ������������� ��������� ����� ���������� � ������� ��
		CMatrix GetViewPoint();				// ���������� ������ ViewPoin
		void SetWinRect(CRect Rect);	// ������������� ������� � ���� ��� ���������
		void CreateMatrF(double dx,double dy); // ��������� ������� MatrF ������������ ����� �����������
		void SetMatrF(CMasMatrix &Matr);	// ������ �������� ������� MatrF �����
		void CreateMatrView(); // ��������� ������� MatrView ������������ ����� �������� ����������� �� ��������� XY ������� ��
		void CreateMatrWindow(); // ��������� ������� MatrWindow 
		int GetNumberRegion(); //���������� ����� �������  ���  ���������.
		void Draw(CDC& dc);	
};

#endif
