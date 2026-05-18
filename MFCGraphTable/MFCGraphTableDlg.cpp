
// MFCGraphTableDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCGraphTable.h"
#include "MFCGraphTableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCGraphTableDlg 对话框



CMFCGraphTableDlg::CMFCGraphTableDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCGRAPHTABLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCGraphTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CHARTS, m_chartTab);
}

BEGIN_MESSAGE_MAP(CMFCGraphTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHARTS, &CMFCGraphTableDlg::OnTcnSelchangeTabCharts)
END_MESSAGE_MAP()


// CMFCGraphTableDlg 消息处理程序

BOOL CMFCGraphTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_chartTab.InsertItem(0, _T("折线图 1"));
	m_chartTab.InsertItem(1, _T("折线图 2"));

	CRect tabWindowRect;
	m_chartTab.GetWindowRect(&tabWindowRect);
	ScreenToClient(&tabWindowRect);

	CRect pageRect;
	m_chartTab.GetClientRect(&pageRect);
	m_chartTab.AdjustRect(FALSE, &pageRect);
	pageRect.OffsetRect(tabWindowRect.left, tabWindowRect.top);

	CreateChartPanel(m_chartPanel, pageRect);
	CreateChartPanel(m_chartPanel2, pageRect);

	m_chartPanel.SetMoveAllPointsEnabled(true);
	m_chartPanel.SetData(std::vector<int>{42, 58, 49, 72, 63, 88, 78, 94, 86, 8, 10, 20});
	ShowChartPanel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCGraphTableDlg::CreateChartPanel(CChartPanel& panel, const CRect& rect)
{
	if (panel.Create(IDD_CHART_PANEL, this))
	{
		panel.SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CMFCGraphTableDlg::ShowChartPanel(int index)
{
	m_chartPanel.ShowWindow(index == 0 ? SW_SHOW : SW_HIDE);
	m_chartPanel2.ShowWindow(index == 1 ? SW_SHOW : SW_HIDE);
}

void CMFCGraphTableDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

BOOL CMFCGraphTableDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialogEx::OnEraseBkgnd(pDC);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCGraphTableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGraphTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCGraphTableDlg::OnTcnSelchangeTabCharts(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	ShowChartPanel(m_chartTab.GetCurSel());
	*pResult = 0;
}
