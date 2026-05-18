#pragma once

#include "afxdialogex.h"
#include "ChartStatic.h"

class CChartPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CChartPanel)

public:
	CChartPanel(CWnd* pParent = nullptr);
	virtual ~CChartPanel();

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

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHART_PANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedCheckLock();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnEnKillfocusEditMaxval();
	afx_msg void OnEnKillfocusEditMinval();
	afx_msg void OnEnKillfocusEditStep();
	afx_msg void OnDeltaposSpinMaxval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinMinval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStep(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	void SyncControlsFromChart();
	void ApplyYAxisFromControls();
	void ApplySpinDelta(UINT editID, NMHDR* pNMHDR, LRESULT* pResult, bool allowNegative);

	CChartStatic m_graphPanel;
	CButton m_checkLock;
	CEdit m_editMaxValue;
	CEdit m_editMinValue;
	CEdit m_editStep;
	CSpinButtonCtrl m_spinMaxValue;
	CSpinButtonCtrl m_spinMinValue;
	CSpinButtonCtrl m_spinStep;
	CButton m_buttonReset;
};
