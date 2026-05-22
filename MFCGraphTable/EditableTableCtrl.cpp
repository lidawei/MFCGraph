#include "pch.h"
#include "MFCGraphTable.h"
#include "EditableTableCtrl.h"

namespace
{
	constexpr UINT_PTR CellValueChangedTimerId = 1;
	constexpr UINT CellValueChangedTimerInterval = 30;
}

IMPLEMENT_DYNAMIC(CEditableTableCtrl, CListCtrl)

CEditableTableCtrl::CEditableTableCtrl()
	: m_editableColumn(-1)
	, m_editRow(-1)
	, m_editColumn(-1)
	, m_isCommittingEdit(false)
	, m_hasPendingCellValueChanged(false)
	, m_pendingRow(-1)
	, m_pendingColumn(-1)
	, m_pendingOldValue(0)
	, m_pendingNewValue(0)
{
}

CEditableTableCtrl::~CEditableTableCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditableTableCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDITABLE_TABLE_CELL_EDIT, &CEditableTableCtrl::OnEnKillfocusEditCell)
	ON_NOTIFY(UDN_DELTAPOS, IDC_EDITABLE_TABLE_CELL_SPIN, &CEditableTableCtrl::OnDeltaposSpinCell)
	ON_NOTIFY(NM_KILLFOCUS, IDC_EDITABLE_TABLE_CELL_SPIN, &CEditableTableCtrl::OnNMKillfocusSpinCell)
END_MESSAGE_MAP()

void CEditableTableCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	EnsureEditControls();
}

void CEditableTableCtrl::SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data)
{
	CommitEditCell();
	m_headers = headers;
	m_data = data;
	RebuildTable();
}

void CEditableTableCtrl::GetTableData(std::vector<CString>& headers, std::vector<std::vector<CString>>& data)
{
	headers = m_headers;
	data = GetData();
}

std::vector<std::vector<CString>> CEditableTableCtrl::GetData()
{
	CommitEditCell();

	std::vector<std::vector<CString>> data;
	const int rowCount = GetItemCount();
	const int columnCount = static_cast<int>(m_headers.size());
	data.resize(rowCount);

	for (int row = 0; row < rowCount; ++row)
	{
		data[row].resize(columnCount);
		for (int column = 0; column < columnCount; ++column)
		{
			data[row][column] = GetItemText(row, column);
		}
	}

	return data;
}

void CEditableTableCtrl::SetEditableColumn(int column)
{
	CommitEditCell();
	m_editableColumn = column;
}

int CEditableTableCtrl::GetEditableColumn() const
{
	return m_editableColumn;
}

void CEditableTableCtrl::SetCellValueChangedCallback(CellValueChangedCallback callback)
{
	m_cellValueChangedCallback = callback;
}

void CEditableTableCtrl::EnsureEditControls()
{
	if (!::IsWindow(GetSafeHwnd()) || ::IsWindow(m_cellEdit.GetSafeHwnd()))
	{
		return;
	}

	m_cellEdit.Create(WS_CHILD | ES_AUTOHSCROLL | ES_NUMBER | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_EDITABLE_TABLE_CELL_EDIT);
	m_cellSpin.Create(WS_CHILD | UDS_ARROWKEYS | UDS_SETBUDDYINT,
		CRect(0, 0, 0, 0), this, IDC_EDITABLE_TABLE_CELL_SPIN);
	m_cellSpin.SetRange32(0, 1000000);
	m_cellSpin.SetBuddy(&m_cellEdit);
	HideEditControls();
}

void CEditableTableCtrl::RebuildTable()
{
	if (!::IsWindow(GetSafeHwnd()))
	{
		return;
	}

	DeleteAllItems();
	while (DeleteColumn(0))
	{
	}

	for (int column = 0; column < static_cast<int>(m_headers.size()); ++column)
	{
		InsertColumn(column, m_headers[column], LVCFMT_LEFT, 100);
	}

	for (int row = 0; row < static_cast<int>(m_data.size()); ++row)
	{
		const CString firstText = m_data[row].empty() ? _T("") : m_data[row][0];
		InsertItem(row, firstText);

		for (int column = 1; column < static_cast<int>(m_headers.size()); ++column)
		{
			CString text;
			if (column < static_cast<int>(m_data[row].size()))
			{
				text = m_data[row][column];
			}
			SetItemText(row, column, text);
		}
	}

	UpdateColumnWidths();
}

void CEditableTableCtrl::UpdateColumnWidths()
{
	if (!::IsWindow(GetSafeHwnd()) || m_headers.empty())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	const int columnWidth = max(80, rect.Width() / static_cast<int>(m_headers.size()));
	for (int column = 0; column < static_cast<int>(m_headers.size()); ++column)
	{
		SetColumnWidth(column, columnWidth);
	}
}

void CEditableTableCtrl::BeginEditCell(int row, int column)
{
	if (column != m_editableColumn || row < 0 || column < 0 || column >= static_cast<int>(m_headers.size()))
	{
		return;
	}

	CommitEditCell();
	EnsureEditControls();

	CRect cellRect;
	const UINT rectType = column == 0 ? LVIR_LABEL : LVIR_BOUNDS;
	if (!GetSubItemRect(row, column, rectType, cellRect))
	{
		return;
	}

	m_editRow = row;
	m_editColumn = column;
	m_cellEdit.SetWindowText(GetItemText(row, column));
	PositionEditControls(cellRect);
	m_cellEdit.SetFocus();
	m_cellEdit.SetSel(0, -1);
}

void CEditableTableCtrl::CommitEditCell()
{
	if (m_isCommittingEdit || !IsEditing() || !::IsWindow(m_cellEdit.GetSafeHwnd()))
	{
		return;
	}

	m_isCommittingEdit = true;

	const int row = m_editRow;
	const int column = m_editColumn;
	const CString oldText = GetItemText(row, column);

	CString text;
	m_cellEdit.GetWindowText(text);
	SetItemText(row, column, text);
	HideEditControls();

	m_isCommittingEdit = false;

	if (oldText != text && m_cellValueChangedCallback)
	{
		QueueCellValueChanged(row, column, _ttoi(oldText), _ttoi(text));
	}
}

void CEditableTableCtrl::CancelEditCell()
{
	if (!IsEditing())
	{
		return;
	}

	HideEditControls();
}

void CEditableTableCtrl::HideEditControls()
{
	m_editRow = -1;
	m_editColumn = -1;
	if (::IsWindow(m_cellEdit.GetSafeHwnd()))
	{
		m_cellEdit.ShowWindow(SW_HIDE);
	}
	if (::IsWindow(m_cellSpin.GetSafeHwnd()))
	{
		m_cellSpin.ShowWindow(SW_HIDE);
	}
}

void CEditableTableCtrl::PositionEditControls(const CRect& cellRect)
{
	const int spinWidth = 18;
	CRect editRect(cellRect);
	editRect.DeflateRect(1, 1, 1, 1);

	CRect spinRect(editRect.right - spinWidth, editRect.top, editRect.right, editRect.bottom);
	editRect.right = spinRect.left;

	m_cellEdit.SetWindowPos(&wndTop, editRect.left, editRect.top, editRect.Width(), editRect.Height(),
		SWP_SHOWWINDOW);
	m_cellSpin.SetWindowPos(&wndTop, spinRect.left, spinRect.top, spinRect.Width(), spinRect.Height(),
		SWP_SHOWWINDOW);
}

void CEditableTableCtrl::QueueCellValueChanged(int row, int column, int oldValue, int newValue)
{
	m_hasPendingCellValueChanged = true;
	m_pendingRow = row;
	m_pendingColumn = column;
	m_pendingOldValue = oldValue;
	m_pendingNewValue = newValue;
	SetTimer(CellValueChangedTimerId, CellValueChangedTimerInterval, nullptr);
}

void CEditableTableCtrl::FirePendingCellValueChanged()
{
	if (!m_hasPendingCellValueChanged || !m_cellValueChangedCallback)
	{
		return;
	}

	m_hasPendingCellValueChanged = false;
	m_cellValueChangedCallback(m_pendingRow, m_pendingColumn, m_pendingOldValue, m_pendingNewValue);
}

bool CEditableTableCtrl::IsEditing() const
{
	return m_editRow >= 0 && m_editColumn >= 0;
}

void CEditableTableCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hitInfo = {};
	hitInfo.pt = point;
	const int row = SubItemHitTest(&hitInfo);
	if (row >= 0)
	{
		BeginEditCell(row, hitInfo.iSubItem);
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CEditableTableCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	UpdateColumnWidths();

	if (IsEditing())
	{
		CommitEditCell();
	}
}

void CEditableTableCtrl::OnEnKillfocusEditCell()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd != nullptr && pFocusWnd->GetSafeHwnd() == m_cellSpin.GetSafeHwnd())
	{
		return;
	}

	CommitEditCell();
}

void CEditableTableCtrl::OnDeltaposSpinCell(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString text;
	m_cellEdit.GetWindowText(text);
	int value = _ttoi(text);
	value += pNMUpDown->iDelta > 0 ? 1 : -1;
	if (value < 0)
	{
		value = 0;
	}

	text.Format(_T("%d"), value);
	m_cellEdit.SetWindowText(text);
	m_cellEdit.SetSel(0, -1);
	m_cellEdit.SetFocus();

	*pResult = 1;
}

void CEditableTableCtrl::OnNMKillfocusSpinCell(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd == nullptr || pFocusWnd->GetSafeHwnd() != m_cellEdit.GetSafeHwnd())
	{
		CommitEditCell();
	}

	*pResult = 0;
}

void CEditableTableCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == CellValueChangedTimerId)
	{
		if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
		{
			return;
		}

		KillTimer(CellValueChangedTimerId);
		FirePendingCellValueChanged();
		return;
	}

	CListCtrl::OnTimer(nIDEvent);
}
