
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
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_pictureControl);
}

BEGIN_MESSAGE_MAP(CMFCGraphTableDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
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
	m_pictureControl.ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW | SS_NOTIFY);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CMFCGraphTableDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == m_pictureControl.GetSafeHwnd())
	{
		CPoint point(pMsg->pt);
		m_pictureControl.ScreenToClient(&point);

		CRect clientRect;
		m_pictureControl.GetClientRect(&clientRect);

		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
			if (m_lineChart.BeginDrag(clientRect, point))
			{
				m_pictureControl.SetCapture();
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				return TRUE;
			}
			break;
		case WM_LBUTTONUP:
			if (m_lineChart.IsDragging())
			{
				m_lineChart.EndDrag();
				ReleaseCapture();
				return TRUE;
			}
			break;
		case WM_MOUSEMOVE:
			if (m_lineChart.IsDragging())
			{
				if (m_lineChart.DragTo(clientRect, point))
				{
					m_pictureControl.Invalidate(FALSE);
				}
				return TRUE;
			}
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
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

void CMFCGraphTableDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_STATIC_PICTURE)
	{
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		{
			CRect rect(lpDrawItemStruct->rcItem);
			CMemDCEx memDC(&dc, rect);
			m_lineChart.Draw(&memDC, rect);
		}

		dc.Detach();
		return;
	}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMFCGraphTableDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCGraphTableDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);

	if (m_lineChart.IsDragging())
	{
		m_lineChart.EndDrag();
		ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCGraphTableDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_lineChart.IsDragging())
	{
		m_pictureControl.Invalidate(FALSE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGraphTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
