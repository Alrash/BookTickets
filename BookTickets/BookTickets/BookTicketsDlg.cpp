
// BookTicketsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BookTickets.h"
#include "BookTicketsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern Info info;

// CBookTicketsDlg 对话框



CBookTicketsDlg::CBookTicketsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKTICKETS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBookTicketsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CBookTicketsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CBookTicketsDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBookTicketsDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CBookTicketsDlg 消息处理程序

BOOL CBookTicketsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CRect tabRect;                                    //标签控件客户区的位置和大小

	m_tab.InsertItem(0, L"预售机票");
	m_tab.InsertItem(1, L"个人信息");
	if (info.user == 'e')
		m_tab.InsertItem(2, L"票务管理");
	//m_tab.InsertItem(3, L"航班管理");
	//m_tab.InsertItem(4, L"人员管理");

	m_book.Create(IDD_BOOK, &m_tab);
	m_info.Create(IDD_INFO, &m_tab);
	if (info.user == 'e')
		m_manage.Create(IDD_MAN, &m_tab);

	//开始更改客户区大小
	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN) * 4 / 5;
	cy = GetSystemMetrics(SM_CYSCREEN) * 4 / 5;

	//设置客户区大小并忽略hWmndInsertAfter,x,y
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

	GetClientRect(&m_rect);							//获得当前整个工作区大小

	m_tab.MoveWindow(0, 0, cx, cy);					//改变tab控件的大小

	m_tab.GetClientRect(&tabRect);					// 获取标签控件客户区Rect 
													// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;
	tabRect.right -= 2;
	tabRect.top += 50;
	tabRect.bottom -= 1;

	m_book.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_info.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	if (info.user == 'e')
		m_manage.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);


	//设置m_book为默认选项卡
	m_tab.SetCurSel(0);

	CRect rect;
	GetWindowRect(&rect);
	listRect.AddTail(rect);                //对话框的区域

	CWnd* pWnd = GetWindow(GW_CHILD);      //获取子窗体

	while (pWnd)
	{
		pWnd->GetWindowRect(rect);         //子窗体的区域
		listRect.AddTail(rect);            //CList<CRect,CRect> m_listRect成员变量
		pWnd = pWnd->GetNextWindow();      //取下一个子窗体
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBookTicketsDlg::OnPaint()
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
HCURSOR CBookTicketsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBookTicketsDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CBookTicketsDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nitem = m_tab.GetCurSel();

	switch (nitem)
	{
	case 0:
		m_book.ShowWindow(SW_SHOW);
		m_info.ShowWindow(SW_HIDE);
		if (info.user == 'e')
			m_manage.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_book.ShowWindow(SW_HIDE);
		m_info.ShowWindow(SW_SHOW);
		if (info.user == 'e')
			m_manage.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_book.ShowWindow(SW_HIDE);
		m_info.ShowWindow(SW_HIDE);
		if (info.user == 'e')
			m_manage.ShowWindow(SW_SHOW);
		break;
	}
}