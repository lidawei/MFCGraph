#pragma once

#include "afxdialogex.h"
#include <vector>

class CTablePanel : public CDialogEx
{
	DECLARE_DYNAMIC(CTablePanel)

public:
	CTablePanel(CWnd* pParent = nullptr);
	virtual ~CTablePanel();

	void SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data);
	void SetReadOnly(bool readOnly);
	void SetEditable(bool editable);
	bool IsReadOnly() const;
	std::vector<std::vector<CString>> GetData();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_PANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkListTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnKillfocusEditCell();
	afx_msg void OnBnClickedCheckTableEditable();

private:
	void RebuildTable();
	void ResizeTable();
	void BeginEditCell(int row, int column);
	void CommitEditCell();
	void CancelEditCell();
	void UpdateColumnWidths();

	CListCtrl m_tableCtrl;
	CButton m_checkEditable;
	CEdit m_cellEdit;
	std::vector<CString> m_headers;
	std::vector<std::vector<CString>> m_data;
	int m_editRow;
	int m_editColumn;
	bool m_readOnly;
	bool m_isCommittingEdit;
};
