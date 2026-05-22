#include "pch.h"
#include "MFCGraphTable.h"
#include "afxdialogex.h"
#include "CEditableTablePanel.h"

IMPLEMENT_DYNAMIC(CEditableTablePanel, CDialogEx)

CEditableTablePanel::CEditableTablePanel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITABLE_TABLE_PANEL, pParent)
{
}

CEditableTablePanel::~CEditableTablePanel()
{
}

void CEditableTablePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EDITABLE_TABLE, m_tableCtrl);
}

BEGIN_MESSAGE_MAP(CEditableTablePanel, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CEditableTablePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ResizeTable();
	return TRUE;
}

void CEditableTablePanel::SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data)
{
	m_tableCtrl.SetTableData(headers, data);
}

void CEditableTablePanel::GetTableData(std::vector<CString>& headers, std::vector<std::vector<CString>>& data)
{
	m_tableCtrl.GetTableData(headers, data);
}

std::vector<std::vector<CString>> CEditableTablePanel::GetData()
{
	return m_tableCtrl.GetData();
}

void CEditableTablePanel::SetEditableColumn(int column)
{
	m_tableCtrl.SetEditableColumn(column);
}

int CEditableTablePanel::GetEditableColumn() const
{
	return m_tableCtrl.GetEditableColumn();
}

void CEditableTablePanel::SetCellValueChangedCallback(CellValueChangedCallback callback)
{
	m_tableCtrl.SetCellValueChangedCallback(callback);
}

void CEditableTablePanel::ResizeTable()
{
	if (!::IsWindow(m_tableCtrl.GetSafeHwnd()))
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(7, 7, 7, 7);
	m_tableCtrl.SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);
}

void CEditableTablePanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	ResizeTable();
}

void CEditableTablePanel::OnOK()
{
}

void CEditableTablePanel::OnCancel()
{
}
