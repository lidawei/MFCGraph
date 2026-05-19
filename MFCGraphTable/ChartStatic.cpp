#include "pch.h"
#include "ChartStatic.h"

namespace
{
class CMemDCEx : public CDC
{
public:
	CMemDCEx(CDC* pDC, const CRect& rect)
		: m_pDC(pDC)
		, m_rect(rect)
		, m_pOldBitmap(nullptr)
	{
		CreateCompatibleDC(pDC);
		m_bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pOldBitmap = SelectObject(&m_bitmap);
		SetWindowOrg(rect.left, rect.top);
	}

	~CMemDCEx()
	{
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);
		SelectObject(m_pOldBitmap);
	}

private:
	CDC* m_pDC;
	CRect m_rect;
	CBitmap m_bitmap;
	CBitmap* m_pOldBitmap;
};
}

BEGIN_MESSAGE_MAP(CChartStatic, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CChartStatic::CChartStatic()
{
}

void CChartStatic::SetData(const std::vector<int>& values)
{
	m_lineChart.SetData(values);
	Invalidate(FALSE);
}

const std::vector<int>& CChartStatic::GetData() const
{
	return m_lineChart.GetData();
}

void CChartStatic::ResetData()
{
	m_lineChart.ResetData();
	Invalidate(FALSE);
}

void CChartStatic::SetMoveAllPointsEnabled(bool enabled)
{
	m_lineChart.SetMoveAllPointsEnabled(enabled);
}

bool CChartStatic::IsMoveAllPointsEnabled() const
{
	return m_lineChart.IsMoveAllPointsEnabled();
}

void CChartStatic::SetYAxisMinValue(int value)
{
	m_lineChart.SetYAxisMinValue(value);
	Invalidate(FALSE);
}

void CChartStatic::SetYAxisMaxValue(int value)
{
	m_lineChart.SetYAxisMaxValue(value);
	Invalidate(FALSE);
}

void CChartStatic::SetYAxisStep(int value)
{
	m_lineChart.SetYAxisStep(value);
	Invalidate(FALSE);
}

int CChartStatic::GetYAxisMinValue() const
{
	return m_lineChart.GetYAxisMinValue();
}

int CChartStatic::GetYAxisMaxValue() const
{
	return m_lineChart.GetYAxisMaxValue();
}

int CChartStatic::GetYAxisStep() const
{
	return m_lineChart.GetYAxisStep();
}

void CChartStatic::SetXAxisMinValue(int value)
{
	m_lineChart.SetXAxisMinValue(value);
	Invalidate(FALSE);
}

void CChartStatic::SetXAxisMaxValue(int value)
{
	m_lineChart.SetXAxisMaxValue(value);
	Invalidate(FALSE);
}

void CChartStatic::SetXAxisStep(int value)
{
	m_lineChart.SetXAxisStep(value);
	Invalidate(FALSE);
}

int CChartStatic::GetXAxisMinValue() const
{
	return m_lineChart.GetXAxisMinValue();
}

int CChartStatic::GetXAxisMaxValue() const
{
	return m_lineChart.GetXAxisMaxValue();
}

int CChartStatic::GetXAxisStep() const
{
	return m_lineChart.GetXAxisStep();
}

void CChartStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

BOOL CChartStatic::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CChartStatic::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CMemDCEx memDC(&dc, rect);
	m_lineChart.Draw(&memDC, rect);
}

void CChartStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	if (m_lineChart.BeginDrag(rect, point))
	{
		SetCapture();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		Invalidate(FALSE);
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CChartStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);

	if (m_lineChart.IsDragging())
	{
		m_lineChart.EndDrag();
		ReleaseCapture();
		Invalidate(FALSE);
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CChartStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_lineChart.IsDragging())
	{
		CRect rect;
		GetClientRect(&rect);

		if (m_lineChart.DragTo(rect, point))
		{
			Invalidate(FALSE);
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}
