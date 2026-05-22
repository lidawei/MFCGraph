#pragma once

#include <functional>
#include <vector>

class CEditableTableCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditableTableCtrl)

public:
	using CellValueChangedCallback = std::function<void(int row, int column, int oldValue, int newValue)>;

	CEditableTableCtrl();
	virtual ~CEditableTableCtrl();

	void SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data);
	void GetTableData(std::vector<CString>& headers, std::vector<std::vector<CString>>& data);
	std::vector<std::vector<CString>> GetData();
	void SetEditableColumn(int column);
	int GetEditableColumn() const;
	void SetCellValueChangedCallback(CellValueChangedCallback callback);

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnKillfocusEditCell();
	afx_msg void OnDeltaposSpinCell(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMKillfocusSpinCell(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void EnsureEditControls();
	void RebuildTable();
	void UpdateColumnWidths();
	void BeginEditCell(int row, int column);
	void CommitEditCell();
	void CancelEditCell();
	void HideEditControls();
	void PositionEditControls(const CRect& cellRect);
	void QueueCellValueChanged(int row, int column, int oldValue, int newValue);
	void FirePendingCellValueChanged();
	bool IsEditing() const;

	CEdit m_cellEdit;
	CSpinButtonCtrl m_cellSpin;
	std::vector<CString> m_headers;
	std::vector<std::vector<CString>> m_data;
	int m_editableColumn;
	int m_editRow;
	int m_editColumn;
	bool m_isCommittingEdit;
	bool m_hasPendingCellValueChanged;
	int m_pendingRow;
	int m_pendingColumn;
	int m_pendingOldValue;
	int m_pendingNewValue;
	CellValueChangedCallback m_cellValueChangedCallback;
};
