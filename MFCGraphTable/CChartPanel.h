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
	void SetXAxisMinValue(int value);
	void SetXAxisMaxValue(int value);
	void SetXAxisStep(int value);
	int GetXAxisMinValue() const;
	int GetXAxisMaxValue() const;
	int GetXAxisStep() const;

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
	afx_msg void OnEnKillfocusEditXMaxval();
	afx_msg void OnEnKillfocusEditXMinval();
	afx_msg void OnEnKillfocusEditXStep();
	afx_msg void OnDeltaposSpinMaxval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinMinval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStep(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXMaxval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXMinval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXStep(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	void SyncControlsFromChart();
	void ApplyYAxisFromControls();
	void ApplyXAxisFromControls();
	void ApplySpinDelta(UINT editID, NMHDR* pNMHDR, LRESULT* pResult, bool allowNegative);

	CChartStatic m_graphPanel;
	CButton m_checkLock;
	CEdit m_editMaxValue;
	CEdit m_editMinValue;
	CEdit m_editStep;
	CEdit m_editXMaxValue;
	CEdit m_editXMinValue;
	CEdit m_editXStep;
	CSpinButtonCtrl m_spinMaxValue;
	CSpinButtonCtrl m_spinMinValue;
	CSpinButtonCtrl m_spinStep;
	CSpinButtonCtrl m_spinXMaxValue;
	CSpinButtonCtrl m_spinXMinValue;
	CSpinButtonCtrl m_spinXStep;
	CButton m_buttonReset;
};
