
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

namespace
{
class CMemDCEx : public CDC
{
public:
	CMemDCEx(CDC* pDC, const CRect& rect)
		: m_pDC(pDC)
		, m_rect(rect)
		, m_pOldBitmap(nullptr)
	{
		CreateCompatibleDC(pDC);
		m_bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pOldBitmap = SelectObject(&m_bitmap);
		SetWindowOrg(rect.left, rect.top);
	}

	~CMemDCEx()
	{
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);
		SelectObject(m_pOldBitmap);
	}

private:
	CDC* m_pDC;
	CRect m_rect;
	CBitmap m_bitmap;
	CBitmap* m_pOldBitmap;
};

void DrawLineChart(CDC* pDC, const CRect& clientRect)
{
	CRect rect(clientRect);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	rect.DeflateRect(24, 24, 24, 24);
	if (rect.Width() <= 120 || rect.Height() <= 100)
	{
		return;
	}

	CRect chartRect(rect);
	chartRect.DeflateRect(54, 34, 24, 44);

	const int values[] = { 42, 58, 49, 72, 63, 88, 78, 94, 86, 108 };
	const int valueCount = static_cast<int>(sizeof(values) / sizeof(values[0]));
	const int maxValue = 120;
	const int ySteps = 6;

	CFont titleFont;
	titleFont.CreatePointFont(110, _T("Microsoft YaHei"));
	CFont* pOldFont = pDC->SelectObject(&titleFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(45, 45, 45));

	CPen borderPen(PS_SOLID, 1, RGB(210, 210, 210));
	CBrush whiteBrush(RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&borderPen);
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->RoundRect(rect, CPoint(8, 8));
	pDC->DrawText(_T("折线图 Demo"), CRect(rect.left, rect.top, rect.right, chartRect.top - 6),
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CPen gridPen(PS_SOLID, 1, RGB(230, 230, 230));
	CPen axisPen(PS_SOLID, 1, RGB(120, 120, 120));
	CFont labelFont;
	labelFont.CreatePointFont(85, _T("Microsoft YaHei"));
	pDC->SelectObject(&labelFont);
	pDC->SetTextColor(RGB(90, 90, 90));

	for (int i = 0; i <= ySteps; ++i)
	{
		const int y = chartRect.bottom - chartRect.Height() * i / ySteps;
		pDC->SelectObject(&gridPen);
		pDC->MoveTo(chartRect.left, y);
		pDC->LineTo(chartRect.right, y);

		CString label;
		label.Format(_T("%d"), maxValue * i / ySteps);
		pDC->DrawText(label, CRect(rect.left + 6, y - 8, chartRect.left - 8, y + 8),
			DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(&axisPen);
	pDC->MoveTo(chartRect.left, chartRect.top);
	pDC->LineTo(chartRect.left, chartRect.bottom);
	pDC->LineTo(chartRect.right, chartRect.bottom);

	CPoint points[valueCount];
	for (int i = 0; i < valueCount; ++i)
	{
		const int x = chartRect.left + chartRect.Width() * i / (valueCount - 1);
		const int y = chartRect.bottom - chartRect.Height() * values[i] / maxValue;
		points[i] = CPoint(x, y);

		CString label;
		label.Format(_T("%d"), i + 1);
		pDC->DrawText(label, CRect(x - 16, chartRect.bottom + 8, x + 16, chartRect.bottom + 26),
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	CPen linePen(PS_SOLID, 2, RGB(46, 117, 182));
	pDC->SelectObject(&linePen);
	pDC->Polyline(points, valueCount);

	CBrush pointBrush(RGB(46, 117, 182));
	pDC->SelectObject(&pointBrush);
	for (int i = 0; i < valueCount; ++i)
	{
		CRect pointRect(points[i].x - 4, points[i].y - 4, points[i].x + 4, points[i].y + 4);
		pDC->Ellipse(pointRect);
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}
}


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
}

BEGIN_MESSAGE_MAP(CMFCGraphTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
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
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);

		CMemDCEx memDC(&dc, rect);
		DrawLineChart(&memDC, rect);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGraphTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

