#include "pch.h"
#include "LineChart.h"

CLineChart::CLineChart()
	: m_values{ 42, 58, 49, 72, 63, 88, 78, 94, 86, 108 }
	, m_initialValues(m_values)
	, m_dragIndex(-1)
	, m_isDragging(false)
	, m_moveAllPoints(false)
	, m_yMinValue(0)
	, m_yMaxValue(120)
	, m_yStep(20)
{
}

void CLineChart::Draw(CDC* pDC, const CRect& clientRect)
{
	CRect rect(clientRect);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	if (m_values.size() < 2 || m_yMaxValue <= m_yMinValue || m_yStep <= 0)
	{
		return;
	}

	CRect chartRect;
	if (!GetChartRects(clientRect, rect, chartRect))
	{
		return;
	}

	CFont titleFont;
	titleFont.CreatePointFont(110, _T("Microsoft YaHei"));
	CFont* pOldFont = pDC->SelectObject(&titleFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(45, 45, 45));

	CPen borderPen(PS_SOLID, 1, RGB(210, 210, 210));
	CBrush whiteBrush(RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&borderPen);
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->RoundRect(rect, CPoint(8, 8));
	pDC->DrawText(_T("\u6298\u7ebf\u56fe Demo"), CRect(rect.left, rect.top, rect.right, chartRect.top - 6),
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawThreeColorGradient(pDC, chartRect);

	CPen gridPen(PS_SOLID, 1, RGB(230, 230, 230));
	CPen axisPen(PS_SOLID, 1, RGB(120, 120, 120));
	CFont labelFont;
	labelFont.CreatePointFont(85, _T("Microsoft YaHei"));
	pDC->SelectObject(&labelFont);
	pDC->SetTextColor(RGB(90, 90, 90));

	auto drawYGridLine = [&](int value)
	{
		const int y = chartRect.bottom
			- chartRect.Height() * (value - m_yMinValue) / (m_yMaxValue - m_yMinValue);
		pDC->SelectObject(&gridPen);
		pDC->MoveTo(chartRect.left, y);
		pDC->LineTo(chartRect.right, y);

		CString label;
		label.Format(_T("%d"), value);
		pDC->DrawText(label, CRect(rect.left + 6, y - 8, chartRect.left - 8, y + 8),
			DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	};

	int lastYGridValue = m_yMinValue;
	for (int value = m_yMinValue; value <= m_yMaxValue; value += m_yStep)
	{
		drawYGridLine(value);
		lastYGridValue = value;
		if (value > m_yMaxValue - m_yStep)
		{
			break;
		}
	}
	if (lastYGridValue != m_yMaxValue)
	{
		drawYGridLine(m_yMaxValue);
	}

	pDC->SelectObject(&gridPen);
	for (int i = 0; i < static_cast<int>(m_values.size()); ++i)
	{
		const int x = chartRect.left + chartRect.Width() * i / (static_cast<int>(m_values.size()) - 1);
		pDC->MoveTo(x, chartRect.top);
		pDC->LineTo(x, chartRect.bottom);
	}

	pDC->SelectObject(&axisPen);
	pDC->MoveTo(chartRect.left, chartRect.top);
	pDC->LineTo(chartRect.left, chartRect.bottom);
	pDC->LineTo(chartRect.right, chartRect.bottom);

	std::vector<CPoint> points(m_values.size());
	for (int i = 0; i < static_cast<int>(m_values.size()); ++i)
	{
		points[i] = ValueToPoint(chartRect, i);

		CString label;
		label.Format(_T("%d"), i + 1);
		pDC->DrawText(label, CRect(points[i].x - 16, chartRect.bottom + 8,
			points[i].x + 16, chartRect.bottom + 26),
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	CPen linePen(PS_SOLID, 2, RGB(46, 117, 182));
	pDC->SelectObject(&linePen);
	pDC->Polyline(points.data(), static_cast<int>(points.size()));

	CBrush pointBrush(RGB(46, 117, 182));
	pDC->SelectObject(&pointBrush);
	for (int i = 0; i < static_cast<int>(points.size()); ++i)
	{
		CRect pointRect(points[i].x - 4, points[i].y - 4, points[i].x + 4, points[i].y + 4);
		pDC->Ellipse(pointRect);
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}

bool CLineChart::BeginDrag(const CRect& clientRect, CPoint point)
{
	if (m_values.size() < 2)
	{
		return false;
	}

	CRect outerRect;
	CRect chartRect;
	if (!GetChartRects(clientRect, outerRect, chartRect))
	{
		return false;
	}

	const int hitIndex = HitTestPoint(chartRect, point);
	if (hitIndex < 0)
	{
		return false;
	}

	m_dragIndex = hitIndex;
	m_isDragging = true;
	return true;
}

bool CLineChart::DragTo(const CRect& clientRect, CPoint point)
{
	if (!m_isDragging || m_dragIndex < 0 || m_dragIndex >= static_cast<int>(m_values.size()))
	{
		return false;
	}

	CRect outerRect;
	CRect chartRect;
	if (!GetChartRects(clientRect, outerRect, chartRect))
	{
		return false;
	}

	const int newValue = PointToValue(chartRect, point.y);
	if (m_moveAllPoints)
	{
		int minValue = m_values[0];
		int maxValue = m_values[0];
		for (int value : m_values)
		{
			if (value < minValue)
			{
				minValue = value;
			}
			if (value > maxValue)
			{
				maxValue = value;
			}
		}

		int delta = newValue - m_values[m_dragIndex];
		if (delta > m_yMaxValue - maxValue)
		{
			delta = m_yMaxValue - maxValue;
		}
		else if (delta < m_yMinValue - minValue)
		{
			delta = m_yMinValue - minValue;
		}

		for (int& value : m_values)
		{
			value += delta;
		}
	}
	else
	{
		m_values[m_dragIndex] = newValue;
	}

	return true;
}

void CLineChart::EndDrag()
{
	m_isDragging = false;
	m_dragIndex = -1;
}

bool CLineChart::IsDragging() const
{
	return m_isDragging;
}

void CLineChart::SetData(const std::vector<int>& values)
{
	m_initialValues = values;

	m_values.clear();
	m_values.reserve(values.size());
	for (int value : values)
	{
		m_values.push_back(ClampValue(value));
	}

	EndDrag();
}

const std::vector<int>& CLineChart::GetData() const
{
	return m_values;
}

void CLineChart::ResetData()
{
	m_values = m_initialValues;
	ClampDataToYAxis(m_values);
	EndDrag();
}

void CLineChart::SetMoveAllPointsEnabled(bool enabled)
{
	m_moveAllPoints = enabled;
}

bool CLineChart::IsMoveAllPointsEnabled() const
{
	return m_moveAllPoints;
}

void CLineChart::SetYAxisMinValue(int value)
{
	m_yMinValue = value;
	if (m_yMinValue >= m_yMaxValue)
	{
		m_yMaxValue = m_yMinValue + 1;
	}

	ClampDataToYAxis(m_values);
	EndDrag();
}

void CLineChart::SetYAxisMaxValue(int value)
{
	m_yMaxValue = value;
	if (m_yMaxValue <= m_yMinValue)
	{
		m_yMinValue = m_yMaxValue - 1;
	}

	ClampDataToYAxis(m_values);
	EndDrag();
}

void CLineChart::SetYAxisStep(int value)
{
	if (value > 0)
	{
		m_yStep = value;
	}
}

int CLineChart::GetYAxisMinValue() const
{
	return m_yMinValue;
}

int CLineChart::GetYAxisMaxValue() const
{
	return m_yMaxValue;
}

int CLineChart::GetYAxisStep() const
{
	return m_yStep;
}

bool CLineChart::GetChartRects(const CRect& clientRect, CRect& outerRect, CRect& chartRect) const
{
	outerRect = clientRect;
	outerRect.DeflateRect(24, 24, 24, 24);
	if (outerRect.Width() <= 120 || outerRect.Height() <= 100)
	{
		return false;
	}

	chartRect = outerRect;
	chartRect.DeflateRect(54, 34, 24, 44);
	return chartRect.Width() > 0 && chartRect.Height() > 0;
}

CPoint CLineChart::ValueToPoint(const CRect& chartRect, int index) const
{
	const int x = chartRect.left + chartRect.Width() * index / (static_cast<int>(m_values.size()) - 1);
	const int y = chartRect.bottom
		- chartRect.Height() * (m_values[index] - m_yMinValue) / (m_yMaxValue - m_yMinValue);
	return CPoint(x, y);
}

int CLineChart::PointToValue(const CRect& chartRect, int y) const
{
	const int value = m_yMinValue
		+ (chartRect.bottom - y) * (m_yMaxValue - m_yMinValue) / chartRect.Height();
	return ClampValue(value);
}

int CLineChart::ClampValue(int value) const
{
	if (value < m_yMinValue)
	{
		return m_yMinValue;
	}
	if (value > m_yMaxValue)
	{
		return m_yMaxValue;
	}

	return value;
}

void CLineChart::ClampDataToYAxis(std::vector<int>& values) const
{
	for (int& value : values)
	{
		value = ClampValue(value);
	}
}

int CLineChart::HitTestPoint(const CRect& chartRect, CPoint point) const
{
	for (int i = 0; i < static_cast<int>(m_values.size()); ++i)
	{
		const CPoint valuePoint = ValueToPoint(chartRect, i);
		if (abs(point.x - valuePoint.x) <= HitRadius && abs(point.y - valuePoint.y) <= HitRadius)
		{
			return i;
		}
	}

	return -1;
}

COLORREF CLineChart::LerpColor(COLORREF from, COLORREF to, double t) const
{
	const int red = static_cast<int>(GetRValue(from) + (GetRValue(to) - GetRValue(from)) * t);
	const int green = static_cast<int>(GetGValue(from) + (GetGValue(to) - GetGValue(from)) * t);
	const int blue = static_cast<int>(GetBValue(from) + (GetBValue(to) - GetBValue(from)) * t);
	return RGB(red, green, blue);
}

void CLineChart::DrawThreeColorGradient(CDC* pDC, const CRect& rect) const
{
	const COLORREF leftColor = RGB(90, 160, 230);
	const COLORREF centerColor = RGB(255, 235, 120);
	const COLORREF rightColor = RGB(95, 190, 120);
	const int width = rect.Width();

	if (width <= 0 || rect.Height() <= 0)
	{
		return;
	}

	for (int x = 0; x < width; ++x)
	{
		const double position = width == 1 ? 0.0 : static_cast<double>(x) / (width - 1);
		const COLORREF color = position <= 0.5
			? LerpColor(leftColor, centerColor, position * 2.0)
			: LerpColor(centerColor, rightColor, (position - 0.5) * 2.0);

		pDC->FillSolidRect(rect.left + x, rect.top, 1, rect.Height(), color);
	}
}
