
// MFCGraphTableDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCGraphTable.h"
#include "MFCGraphTableDlg.h"
#include "afxdialogex.h"
#include <vector>

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

COLORREF LerpColor(COLORREF from, COLORREF to, double t)
{
	const int red = static_cast<int>(GetRValue(from) + (GetRValue(to) - GetRValue(from)) * t);
	const int green = static_cast<int>(GetGValue(from) + (GetGValue(to) - GetGValue(from)) * t);
	const int blue = static_cast<int>(GetBValue(from) + (GetBValue(to) - GetBValue(from)) * t);
	return RGB(red, green, blue);
}

void DrawThreeColorGradient(CDC* pDC, const CRect& rect)
{
	const COLORREF leftColor = RGB(90, 160, 230);
	const COLORREF centerColor = RGB(255, 235, 120);
	const COLORREF rightColor = RGB(95, 190, 120);
	const int width = rect.Width();

	if (width <= 0 || rect.Height() <= 0)
	{
		return;
	}

	for (int x = 0; x < width; ++x)
	{
		const double position = width == 1 ? 0.0 : static_cast<double>(x) / (width - 1);
		const COLORREF color = position <= 0.5
			? LerpColor(leftColor, centerColor, position * 2.0)
			: LerpColor(centerColor, rightColor, (position - 0.5) * 2.0);

		pDC->FillSolidRect(rect.left + x, rect.top, 1, rect.Height(), color);
	}
}

bool GetChartRects(const CRect& clientRect, CRect& outerRect, CRect& chartRect)
{
	outerRect = clientRect;
	outerRect.DeflateRect(24, 24, 24, 24);
	if (outerRect.Width() <= 120 || outerRect.Height() <= 100)
	{
		return false;
	}

	chartRect = outerRect;
	chartRect.DeflateRect(54, 34, 24, 44);
	return chartRect.Width() > 0 && chartRect.Height() > 0;
}

CPoint ValueToPoint(const CRect& chartRect, int index, int value, int valueCount, int maxValue)
{
	const int x = chartRect.left + chartRect.Width() * index / (valueCount - 1);
	const int y = chartRect.bottom - chartRect.Height() * value / maxValue;
	return CPoint(x, y);
}

int PointToValue(const CRect& chartRect, int y, int maxValue)
{
	int value = (chartRect.bottom - y) * maxValue / chartRect.Height();
	if (value < 0)
	{
		value = 0;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}

	return value;
}

int HitTestChartPoint(const CRect& chartRect, const int* values, int valueCount, int maxValue, CPoint point)
{
	const int hitRadius = 8;
	for (int i = 0; i < valueCount; ++i)
	{
		const CPoint valuePoint = ValueToPoint(chartRect, i, values[i], valueCount, maxValue);
		if (abs(point.x - valuePoint.x) <= hitRadius && abs(point.y - valuePoint.y) <= hitRadius)
		{
			return i;
		}
	}

	return -1;
}

void DrawLineChart(CDC* pDC, const CRect& clientRect, const int* values, int valueCount, int maxValue)
{
	CRect rect(clientRect);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	if (valueCount < 2 || maxValue <= 0)
	{
		return;
	}

	CRect chartRect;
	if (!GetChartRects(clientRect, rect, chartRect))
	{
		return;
	}

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
	DrawThreeColorGradient(pDC, chartRect);

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

	std::vector<CPoint> points(valueCount);
	for (int i = 0; i < valueCount; ++i)
	{
		points[i] = ValueToPoint(chartRect, i, values[i], valueCount, maxValue);

		CString label;
		label.Format(_T("%d"), i + 1);
		pDC->DrawText(label, CRect(points[i].x - 16, chartRect.bottom + 8,
			points[i].x + 16, chartRect.bottom + 26),
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	CPen linePen(PS_SOLID, 2, RGB(46, 117, 182));
	pDC->SelectObject(&linePen);
	pDC->Polyline(points.data(), valueCount);

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
	, m_dragIndex(-1)
	, m_isDragging(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	const int initialValues[ChartValueCount] = { 42, 58, 49, 72, 63, 88, 78, 94, 86, 108 };
	for (int i = 0; i < ChartValueCount; ++i)
	{
		m_values[i] = initialValues[i];
	}
}

void CMFCGraphTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCGraphTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
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
		DrawLineChart(&memDC, rect, m_values, ChartValueCount, ChartMaxValue);
	}
}

void CMFCGraphTableDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect clientRect;
	CRect outerRect;
	CRect chartRect;
	GetClientRect(&clientRect);

	if (GetChartRects(clientRect, outerRect, chartRect))
	{
		const int hitIndex = HitTestChartPoint(chartRect, m_values, ChartValueCount, ChartMaxValue, point);
		if (hitIndex >= 0)
		{
			m_dragIndex = hitIndex;
			m_isDragging = true;
			SetCapture();
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCGraphTableDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);

	if (m_isDragging)
	{
		m_isDragging = false;
		m_dragIndex = -1;
		ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCGraphTableDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_isDragging && m_dragIndex >= 0)
	{
		CRect clientRect;
		CRect outerRect;
		CRect chartRect;
		GetClientRect(&clientRect);

		if (GetChartRects(clientRect, outerRect, chartRect))
		{
			m_values[m_dragIndex] = PointToValue(chartRect, point.y, ChartMaxValue);
			Invalidate(FALSE);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGraphTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

