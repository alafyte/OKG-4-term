#include "stdafx.h"
#include "Bezier.h"

Bezier::Bezier(const double *x, const double *y, unsigned int n) : m_n(n)
{
	::memcpy(m_x, x, n);
	::memcpy(m_y, y, n);
}

Bezier::Bezier(CMatrix & x, CMatrix & y)
{
	m_n = x.cols() * x.rows();

	m_x = new double[m_n];
	m_y = new double[m_n];

	for (unsigned int i = 0; i < m_n; ++i)
	{
		m_x[i] = x(i);
		m_y[i] = y(i);
	}
}

Bezier::~Bezier()
{
	delete[] m_x, m_y;
}