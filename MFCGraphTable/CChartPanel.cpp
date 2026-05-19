#include "pch.h"
#include "MFCGraphTable.h"
#include "afxdialogex.h"
#include "CChartPanel.h"

IMPLEMENT_DYNAMIC(CChartPanel, CDialogEx)

CChartPanel::CChartPanel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHART_PANEL, pParent)
{
}

CChartPanel::~CChartPanel()
{
}

void CChartPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPHPANEL, m_graphPanel);
	DDX_Control(pDX, IDC_CHECK_LOCK, m_checkLock);
	DDX_Control(pDX, IDC_EDIT_MAXVAL, m_editMaxValue);
	DDX_Control(pDX, IDC_EDIT_MINVAL, m_editMinValue);
	DDX_Control(pDX, IDC_EDIT_STEP, m_editStep);
	DDX_Control(pDX, IDC_EDIT_X_MAXVAL, m_editXMaxValue);
	DDX_Control(pDX, IDC_EDIT_X_MINVAL, m_editXMinValue);
	DDX_Control(pDX, IDC_EDIT_X_STEP, m_editXStep);
	DDX_Control(pDX, IDC_SPIN_MAXVAL, m_spinMaxValue);
	DDX_Control(pDX, IDC_SPIN_MINVAL, m_spinMinValue);
	DDX_Control(pDX, IDC_SPIN_STEP, m_spinStep);
	DDX_Control(pDX, IDC_SPIN_X_MAXVAL, m_spinXMaxValue);
	DDX_Control(pDX, IDC_SPIN_X_MINVAL, m_spinXMinValue);
	DDX_Control(pDX, IDC_SPIN_X_STEP, m_spinXStep);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_buttonReset);
}

BEGIN_MESSAGE_MAP(CChartPanel, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_LOCK, &CChartPanel::OnBnClickedCheckLock)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CChartPanel::OnBnClickedButtonReset)
	ON_EN_KILLFOCUS(IDC_EDIT_MAXVAL, &CChartPanel::OnEnKillfocusEditMaxval)
	ON_EN_KILLFOCUS(IDC_EDIT_MINVAL, &CChartPanel::OnEnKillfocusEditMinval)
	ON_EN_KILLFOCUS(IDC_EDIT_STEP, &CChartPanel::OnEnKillfocusEditStep)
	ON_EN_KILLFOCUS(IDC_EDIT_X_MAXVAL, &CChartPanel::OnEnKillfocusEditXMaxval)
	ON_EN_KILLFOCUS(IDC_EDIT_X_MINVAL, &CChartPanel::OnEnKillfocusEditXMinval)
	ON_EN_KILLFOCUS(IDC_EDIT_X_STEP, &CChartPanel::OnEnKillfocusEditXStep)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXVAL, &CChartPanel::OnDeltaposSpinMaxval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MINVAL, &CChartPanel::OnDeltaposSpinMinval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP, &CChartPanel::OnDeltaposSpinStep)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_MAXVAL, &CChartPanel::OnDeltaposSpinXMaxval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_MINVAL, &CChartPanel::OnDeltaposSpinXMinval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_STEP, &CChartPanel::OnDeltaposSpinXStep)
END_MESSAGE_MAP()

BOOL CChartPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spinMaxValue.SetRange32(-100000, 100000);
	m_spinMinValue.SetRange32(-100000, 100000);
	m_spinStep.SetRange32(1, 100000);
	m_spinXMaxValue.SetRange32(-100000, 100000);
	m_spinXMinValue.SetRange32(-100000, 100000);
	m_spinXStep.SetRange32(1, 100000);
	SyncControlsFromChart();

	return TRUE;
}

void CChartPanel::OnOK()
{
	ApplyYAxisFromControls();
	ApplyXAxisFromControls();
}

void CChartPanel::OnCancel()
{
}

void CChartPanel::SetData(const std::vector<int>& values)
{
	m_graphPanel.SetData(values);
	SyncControlsFromChart();
}

const std::vector<int>& CChartPanel::GetData() const
{
	return m_graphPanel.GetData();
}

void CChartPanel::ResetData()
{
	m_graphPanel.ResetData();
}

void CChartPanel::SetMoveAllPointsEnabled(bool enabled)
{
	m_graphPanel.SetMoveAllPointsEnabled(enabled);
	if (m_checkLock.GetSafeHwnd() != nullptr)
	{
		m_checkLock.SetCheck(enabled ? BST_CHECKED : BST_UNCHECKED);
	}
}

bool CChartPanel::IsMoveAllPointsEnabled() const
{
	return m_graphPanel.IsMoveAllPointsEnabled();
}

void CChartPanel::SetYAxisMinValue(int value)
{
	m_graphPanel.SetYAxisMinValue(value);
	SyncControlsFromChart();
}

void CChartPanel::SetYAxisMaxValue(int value)
{
	m_graphPanel.SetYAxisMaxValue(value);
	SyncControlsFromChart();
}

void CChartPanel::SetYAxisStep(int value)
{
	m_graphPanel.SetYAxisStep(value);
	SyncControlsFromChart();
}

int CChartPanel::GetYAxisMinValue() const
{
	return m_graphPanel.GetYAxisMinValue();
}

int CChartPanel::GetYAxisMaxValue() const
{
	return m_graphPanel.GetYAxisMaxValue();
}

int CChartPanel::GetYAxisStep() const
{
	return m_graphPanel.GetYAxisStep();
}

void CChartPanel::SetXAxisMinValue(int value)
{
	m_graphPanel.SetXAxisMinValue(value);
	SyncControlsFromChart();
}

void CChartPanel::SetXAxisMaxValue(int value)
{
	m_graphPanel.SetXAxisMaxValue(value);
	SyncControlsFromChart();
}

void CChartPanel::SetXAxisStep(int value)
{
	m_graphPanel.SetXAxisStep(value);
	SyncControlsFromChart();
}

int CChartPanel::GetXAxisMinValue() const
{
	return m_graphPanel.GetXAxisMinValue();
}

int CChartPanel::GetXAxisMaxValue() const
{
	return m_graphPanel.GetXAxisMaxValue();
}

int CChartPanel::GetXAxisStep() const
{
	return m_graphPanel.GetXAxisStep();
}

void CChartPanel::OnBnClickedCheckLock()
{
	m_graphPanel.SetMoveAllPointsEnabled(m_checkLock.GetCheck() == BST_CHECKED);
}

void CChartPanel::OnBnClickedButtonReset()
{
	m_graphPanel.ResetData();
}

void CChartPanel::OnEnKillfocusEditMaxval()
{
	ApplyYAxisFromControls();
}

void CChartPanel::OnEnKillfocusEditMinval()
{
	ApplyYAxisFromControls();
}

void CChartPanel::OnEnKillfocusEditStep()
{
	ApplyYAxisFromControls();
}

void CChartPanel::OnEnKillfocusEditXMaxval()
{
	ApplyXAxisFromControls();
}

void CChartPanel::OnEnKillfocusEditXMinval()
{
	ApplyXAxisFromControls();
}

void CChartPanel::OnEnKillfocusEditXStep()
{
	ApplyXAxisFromControls();
}

void CChartPanel::OnDeltaposSpinMaxval(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_MAXVAL, pNMHDR, pResult, true);
}

void CChartPanel::OnDeltaposSpinMinval(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_MINVAL, pNMHDR, pResult, true);
}

void CChartPanel::OnDeltaposSpinStep(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_STEP, pNMHDR, pResult, false);
}

void CChartPanel::OnDeltaposSpinXMaxval(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_X_MAXVAL, pNMHDR, pResult, true);
}

void CChartPanel::OnDeltaposSpinXMinval(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_X_MINVAL, pNMHDR, pResult, true);
}

void CChartPanel::OnDeltaposSpinXStep(NMHDR* pNMHDR, LRESULT* pResult)
{
	ApplySpinDelta(IDC_EDIT_X_STEP, pNMHDR, pResult, false);
}

void CChartPanel::SyncControlsFromChart()
{
	if (m_editMinValue.GetSafeHwnd() == nullptr)
	{
		return;
	}

	SetDlgItemInt(IDC_EDIT_MINVAL, m_graphPanel.GetYAxisMinValue(), TRUE);
	SetDlgItemInt(IDC_EDIT_MAXVAL, m_graphPanel.GetYAxisMaxValue(), TRUE);
	SetDlgItemInt(IDC_EDIT_STEP, m_graphPanel.GetYAxisStep(), FALSE);
	SetDlgItemInt(IDC_EDIT_X_MINVAL, m_graphPanel.GetXAxisMinValue(), TRUE);
	SetDlgItemInt(IDC_EDIT_X_MAXVAL, m_graphPanel.GetXAxisMaxValue(), TRUE);
	SetDlgItemInt(IDC_EDIT_X_STEP, m_graphPanel.GetXAxisStep(), FALSE);
	m_checkLock.SetCheck(m_graphPanel.IsMoveAllPointsEnabled() ? BST_CHECKED : BST_UNCHECKED);
}

void CChartPanel::ApplyYAxisFromControls()
{
	BOOL minTranslated = FALSE;
	BOOL maxTranslated = FALSE;
	BOOL stepTranslated = FALSE;

	int minValue = GetDlgItemInt(IDC_EDIT_MINVAL, &minTranslated, TRUE);
	int maxValue = GetDlgItemInt(IDC_EDIT_MAXVAL, &maxTranslated, TRUE);
	const int step = GetDlgItemInt(IDC_EDIT_STEP, &stepTranslated, FALSE);

	if (!minTranslated)
	{
		minValue = m_graphPanel.GetYAxisMinValue();
	}
	if (!maxTranslated)
	{
		maxValue = m_graphPanel.GetYAxisMaxValue();
	}
	if (minValue >= maxValue)
	{
		maxValue = minValue + 1;
	}

	m_graphPanel.SetYAxisMinValue(minValue);
	m_graphPanel.SetYAxisMaxValue(maxValue);
	if (stepTranslated && step > 0)
	{
		m_graphPanel.SetYAxisStep(step);
	}

	SyncControlsFromChart();
}

void CChartPanel::ApplyXAxisFromControls()
{
	BOOL minTranslated = FALSE;
	BOOL maxTranslated = FALSE;
	BOOL stepTranslated = FALSE;

	int minValue = GetDlgItemInt(IDC_EDIT_X_MINVAL, &minTranslated, TRUE);
	int maxValue = GetDlgItemInt(IDC_EDIT_X_MAXVAL, &maxTranslated, TRUE);
	const int step = GetDlgItemInt(IDC_EDIT_X_STEP, &stepTranslated, FALSE);

	if (!minTranslated)
	{
		minValue = m_graphPanel.GetXAxisMinValue();
	}
	if (!maxTranslated)
	{
		maxValue = m_graphPanel.GetXAxisMaxValue();
	}
	if (minValue >= maxValue)
	{
		maxValue = minValue + 1;
	}

	m_graphPanel.SetXAxisMinValue(minValue);
	m_graphPanel.SetXAxisMaxValue(maxValue);
	if (stepTranslated && step > 0)
	{
		m_graphPanel.SetXAxisStep(step);
	}

	SyncControlsFromChart();
}

void CChartPanel::ApplySpinDelta(UINT editID, NMHDR* pNMHDR, LRESULT* pResult, bool allowNegative)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	BOOL translated = FALSE;
	int value = GetDlgItemInt(editID, &translated, allowNegative);
	if (!translated)
	{
		value = 0;
	}

	value += pNMUpDown->iDelta > 0 ? 1 : -1;
	if (!allowNegative && value < 1)
	{
		value = 1;
	}

	SetDlgItemInt(editID, value, allowNegative);
	if (editID == IDC_EDIT_X_MAXVAL || editID == IDC_EDIT_X_MINVAL || editID == IDC_EDIT_X_STEP)
	{
		ApplyXAxisFromControls();
	}
	else
	{
		ApplyYAxisFromControls();
	}

	*pResult = 1;
}
