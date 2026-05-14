#pragma once

#include <vector>

class CLineChart
{
public:
	CLineChart();

	void Draw(CDC* pDC, const CRect& clientRect);
	bool BeginDrag(const CRect& clientRect, CPoint point);
	bool DragTo(const CRect& clientRect, CPoint point);
	void EndDrag();
	bool IsDragging() const;

private:
	static const int MaxValue = 120;
	static const int HitRadius = 8;

	bool GetChartRects(const CRect& clientRect, CRect& outerRect, CRect& chartRect) const;
	CPoint ValueToPoint(const CRect& chartRect, int index) const;
	int PointToValue(const CRect& chartRect, int y) const;
	int HitTestPoint(const CRect& chartRect, CPoint point) const;
	COLORREF LerpColor(COLORREF from, COLORREF to, double t) const;
	void DrawThreeColorGradient(CDC* pDC, const CRect& rect) const;

	std::vector<int> m_values;
	int m_dragIndex;
	bool m_isDragging;
};
