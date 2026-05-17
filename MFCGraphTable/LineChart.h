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
	void SetData(const std::vector<int>& values);
	const std::vector<int>& GetData() const;
	void ResetData();
	void SetMoveAllPointsEnabled(bool enabled);
	bool IsMoveAllPointsEnabled() const;
	void SetYAxisMinValue(int value);
	void SetYAxisMaxValue(int value);
	void SetYAxisStep(int value);
	int GetYAxisMinValue() const;
	int GetYAxisMaxValue() const;
	int GetYAxisStep() const;

private:
	static const int HitRadius = 8;

	bool GetChartRects(const CRect& clientRect, CRect& outerRect, CRect& chartRect) const;
	CPoint ValueToPoint(const CRect& chartRect, int index) const;
	int PointToValue(const CRect& chartRect, int y) const;
	int ClampValue(int value) const;
	void ClampDataToYAxis(std::vector<int>& values) const;
	int HitTestPoint(const CRect& chartRect, CPoint point) const;
	COLORREF LerpColor(COLORREF from, COLORREF to, double t) const;
	void DrawThreeColorGradient(CDC* pDC, const CRect& rect) const;

	std::vector<int> m_values;
	std::vector<int> m_initialValues;
	int m_dragIndex;
	bool m_isDragging;
	bool m_moveAllPoints;
	int m_yMinValue;
	int m_yMaxValue;
	int m_yStep;
};
