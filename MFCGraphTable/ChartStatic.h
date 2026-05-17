#pragma once

#include "LineChart.h"

class CChartStatic : public CStatic
{
public:
	CChartStatic();

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
