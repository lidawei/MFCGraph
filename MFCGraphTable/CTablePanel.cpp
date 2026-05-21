#include "pch.h"
#include "MFCGraphTable.h"
#include "afxdialogex.h"
#include "CTablePanel.h"

IMPLEMENT_DYNAMIC(CTablePanel, CDialogEx)

CTablePanel::CTablePanel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABLE_PANEL, pParent)
	, m_editRow(-1)
	, m_editColumn(-1)
	, m_readOnly(true)
	, m_isCommittingEdit(false)
{
}

CTablePanel::~CTablePanel()
{
}

void CTablePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TABLE_EDITABLE, m_checkEditable);
	DDX_Control(pDX, IDC_LIST_TABLE, m_tableCtrl);
}

BEGIN_MESSAGE_MAP(CTablePanel, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_TABLE_EDITABLE, &CTablePanel::OnBnClickedCheckTableEditable)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TABLE, &CTablePanel::OnNMDblclkListTable)
	ON_EN_KILLFOCUS(IDC_EDIT_CELL, &CTablePanel::OnEnKillfocusEditCell)
END_MESSAGE_MAP()

BOOL CTablePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_tableCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_checkEditable.SetCheck(m_readOnly ? BST_UNCHECKED : BST_CHECKED);

	m_cellEdit.Create(WS_CHILD | ES_AUTOHSCROLL | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_EDIT_CELL);
	m_cellEdit.ShowWindow(SW_HIDE);

	RebuildTable();
	return TRUE;
}

void CTablePanel::SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data)
{
	CommitEditCell();
	m_headers = headers;
	m_data = data;
	RebuildTable();
}

void CTablePanel::SetReadOnly(bool readOnly)
{
	CommitEditCell();
	m_readOnly = readOnly;
	if (::IsWindow(m_checkEditable.GetSafeHwnd()))
	{
		m_checkEditable.SetCheck(m_readOnly ? BST_UNCHECKED : BST_CHECKED);
	}
}

void CTablePanel::SetEditable(bool editable)
{
	SetReadOnly(!editable);
}

bool CTablePanel::IsReadOnly() const
{
	return m_readOnly;
}

std::vector<std::vector<CString>> CTablePanel::GetData()
{
	CommitEditCell();

	std::vector<std::vector<CString>> data;
	const int rowCount = m_tableCtrl.GetItemCount();
	const int columnCount = static_cast<int>(m_headers.size());
	data.resize(rowCount);

	for (int row = 0; row < rowCount; ++row)
	{
		data[row].resize(columnCount);
		for (int column = 0; column < columnCount; ++column)
		{
			data[row][column] = m_tableCtrl.GetItemText(row, column);
		}
	}

	return data;
}

void CTablePanel::RebuildTable()
{
	if (!::IsWindow(m_tableCtrl.GetSafeHwnd()))
	{
		return;
	}

	m_tableCtrl.DeleteAllItems();
	while (m_tableCtrl.DeleteColumn(0))
	{
	}

	for (int column = 0; column < static_cast<int>(m_headers.size()); ++column)
	{
		m_tableCtrl.InsertColumn(column, m_headers[column], LVCFMT_LEFT, 100);
	}

	for (int row = 0; row < static_cast<int>(m_data.size()); ++row)
	{
		const CString firstText = m_headers.empty() || m_data[row].empty() ? _T("") : m_data[row][0];
		m_tableCtrl.InsertItem(row, firstText);

		for (int column = 1; column < static_cast<int>(m_headers.size()); ++column)
		{
			CString text;
			if (column < static_cast<int>(m_data[row].size()))
			{
				text = m_data[row][column];
			}
			m_tableCtrl.SetItemText(row, column, text);
		}
	}

	UpdateColumnWidths();
}

void CTablePanel::ResizeTable()
{
	if (!::IsWindow(m_tableCtrl.GetSafeHwnd()))
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(7, 40, 7, 7);
	m_tableCtrl.SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
	m_checkEditable.BringWindowToTop();
	UpdateColumnWidths();
}

void CTablePanel::BeginEditCell(int row, int column)
{
	if (m_readOnly || row < 0 || column < 0 || column >= static_cast<int>(m_headers.size()))
	{
		return;
	}

	CommitEditCell();

	CRect cellRect;
	const UINT rectType = column == 0 ? LVIR_LABEL : LVIR_BOUNDS;
	if (!m_tableCtrl.GetSubItemRect(row, column, rectType, cellRect))
	{
		return;
	}

	cellRect.DeflateRect(1, 1, 1, 1);
	m_tableCtrl.ClientToScreen(&cellRect);
	ScreenToClient(&cellRect);

	m_editRow = row;
	m_editColumn = column;
	m_cellEdit.SetWindowText(m_tableCtrl.GetItemText(row, column));
	m_cellEdit.SetWindowPos(&wndTop, cellRect.left, cellRect.top, cellRect.Width(), cellRect.Height(),
		SWP_SHOWWINDOW);
	m_cellEdit.SetFocus();
	m_cellEdit.SetSel(0, -1);
}

void CTablePanel::CommitEditCell()
{
	if (m_isCommittingEdit || m_editRow < 0 || m_editColumn < 0 || !::IsWindow(m_cellEdit.GetSafeHwnd()))
	{
		return;
	}

	m_isCommittingEdit = true;

	CString text;
	m_cellEdit.GetWindowText(text);
	if (::IsWindow(m_tableCtrl.GetSafeHwnd()))
	{
		m_tableCtrl.SetItemText(m_editRow, m_editColumn, text);
	}

	m_editRow = -1;
	m_editColumn = -1;
	m_cellEdit.ShowWindow(SW_HIDE);

	m_isCommittingEdit = false;
}

void CTablePanel::CancelEditCell()
{
	if (!::IsWindow(m_cellEdit.GetSafeHwnd()))
	{
		return;
	}

	m_editRow = -1;
	m_editColumn = -1;
	m_cellEdit.ShowWindow(SW_HIDE);
}

void CTablePanel::UpdateColumnWidths()
{
	if (!::IsWindow(m_tableCtrl.GetSafeHwnd()) || m_headers.empty())
	{
		return;
	}

	CRect rect;
	m_tableCtrl.GetClientRect(&rect);
	const int columnWidth = max(80, rect.Width() / static_cast<int>(m_headers.size()));
	for (int column = 0; column < static_cast<int>(m_headers.size()); ++column)
	{
		m_tableCtrl.SetColumnWidth(column, columnWidth);
	}
}

void CTablePanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	ResizeTable();
}

void CTablePanel::OnNMDblclkListTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	CPoint point;
	GetCursorPos(&point);
	m_tableCtrl.ScreenToClient(&point);

	LVHITTESTINFO hitInfo = {};
	hitInfo.pt = point;
	const int row = m_tableCtrl.SubItemHitTest(&hitInfo);
	if (row >= 0)
	{
		BeginEditCell(row, hitInfo.iSubItem);
	}

	*pResult = 0;
}

void CTablePanel::OnEnKillfocusEditCell()
{
	CommitEditCell();
}

void CTablePanel::OnBnClickedCheckTableEditable()
{
	SetEditable(m_checkEditable.GetCheck() == BST_CHECKED);
}

void CTablePanel::OnOK()
{
	CommitEditCell();
}

void CTablePanel::OnCancel()
{
	CancelEditCell();
}
