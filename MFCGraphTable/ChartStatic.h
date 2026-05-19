#pragma once

#include "LineChart.h"

class CChartStatic : public CStatic
{
public:
	CChartStatic();
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
	void SetXAxisMinValue(int value);
	void SetXAxisMaxValue(int value);
	void SetXAxisStep(int value);
	int GetXAxisMinValue() const;
	int GetXAxisMaxValue() const;
	int GetXAxisStep() const;

protected:
	virtual void PreSubclassWindow();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	CLineChart m_lineChart;
};
