#pragma once

#include "afxdialogex.h"
#include "EditableTableCtrl.h"
#include <vector>

class CEditableTablePanel : public CDialogEx
{
	DECLARE_DYNAMIC(CEditableTablePanel)

public:
	using CellValueChangedCallback = CEditableTableCtrl::CellValueChangedCallback;

	CEditableTablePanel(CWnd* pParent = nullptr);
	virtual ~CEditableTablePanel();

	void SetTableData(const std::vector<CString>& headers, const std::vector<std::vector<CString>>& data);
	void GetTableData(std::vector<CString>& headers, std::vector<std::vector<CString>>& data);
	std::vector<std::vector<CString>> GetData();
	void SetEditableColumn(int column);
	int GetEditableColumn() const;
	void SetCellValueChangedCallback(CellValueChangedCallback callback);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITABLE_TABLE_PANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void ResizeTable();

	CEditableTableCtrl m_tableCtrl;
};
