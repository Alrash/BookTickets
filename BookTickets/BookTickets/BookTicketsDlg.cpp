
// BookTicketsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BookTickets.h"
#include "BookTicketsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern Info info;

// CBookTicketsDlg �Ի���



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


// CBookTicketsDlg ��Ϣ�������

BOOL CBookTicketsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CRect tabRect;                                    //��ǩ�ؼ��ͻ�����λ�úʹ�С

	m_tab.InsertItem(0, L"Ԥ�ۻ�Ʊ");
	m_tab.InsertItem(1, L"������Ϣ");
	if (info.user == 'e')
		m_tab.InsertItem(2, L"Ʊ�����");
	//m_tab.InsertItem(3, L"�������");
	//m_tab.InsertItem(4, L"��Ա����");

	m_book.Create(IDD_BOOK, &m_tab);
	m_info.Create(IDD_INFO, &m_tab);
	if (info.user == 'e')
		m_manage.Create(IDD_MAN, &m_tab);

	//��ʼ���Ŀͻ�����С
	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN) * 4 / 5;
	cy = GetSystemMetrics(SM_CYSCREEN) * 4 / 5;

	//���ÿͻ�����С������hWmndInsertAfter,x,y
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

	GetClientRect(&m_rect);							//��õ�ǰ������������С

	m_tab.MoveWindow(0, 0, cx, cy);					//�ı�tab�ؼ��Ĵ�С

	m_tab.GetClientRect(&tabRect);					// ��ȡ��ǩ�ؼ��ͻ���Rect 
													// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
	tabRect.left += 1;
	tabRect.right -= 2;
	tabRect.top += 50;
	tabRect.bottom -= 1;

	m_book.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_info.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	if (info.user == 'e')
		m_manage.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);


	//����m_bookΪĬ��ѡ�
	m_tab.SetCurSel(0);

	CRect rect;
	GetWindowRect(&rect);
	listRect.AddTail(rect);                //�Ի��������

	CWnd* pWnd = GetWindow(GW_CHILD);      //��ȡ�Ӵ���

	while (pWnd)
	{
		pWnd->GetWindowRect(rect);         //�Ӵ��������
		listRect.AddTail(rect);            //CList<CRect,CRect> m_listRect��Ա����
		pWnd = pWnd->GetNextWindow();      //ȡ��һ���Ӵ���
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBookTicketsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBookTicketsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBookTicketsDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CBookTicketsDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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