// Book.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BookTickets.h"
#include "Book.h"
#include "afxdialogex.h"

#include "ClientSocket.h"
#include "Deal.h"
#include <string>

using std::string;
using std::to_string;

extern Info info;

// CBook �Ի���

IMPLEMENT_DYNAMIC(CBook, CDialogEx)

CBook::CBook(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOK, pParent)
{

}

CBook::~CBook()
{
}

void CBook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_BOOK_YEAR, m_year);
	DDX_Control(pDX, IDC_BOOK_MON, m_mon);
	DDX_Control(pDX, IDC_BOOK_DAY, m_day);
	DDX_Control(pDX, IDC_BOOK_SET, m_setoff);
	DDX_Control(pDX, IDC_BOOK_OFF, m_arrive);
}


BEGIN_MESSAGE_MAP(CBook, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CBook::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BOOK_FIND, &CBook::OnBnClickedBookFind)
//	ON_EN_CHANGE(IDC_BOOK_SET, &CBook::OnEnChangeBookSet)
//	ON_EN_CHANGE(IDC_BOOK_OFF, &CBook::OnEnChangeBookOff)
ON_CBN_SELCHANGE(IDC_BOOK_MON, &CBook::OnCbnSelchangeBookMon)
ON_BN_CLICKED(IDC_BOOK_BOOK, &CBook::OnBnClickedBookBook)
END_MESSAGE_MAP()


// CBook ��Ϣ�������


void CBook::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (listRect.GetCount() > 0)
	{
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = listRect.GetHeadPosition();                                     //��һ��������ǶԻ����Rect

		CRect dlgSaved;
		dlgSaved = listRect.GetNext(pos);
		ScreenToClient(dlgNow);

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();                             //���ݵ�ǰ��֮ǰ����ĶԻ���Ŀ�������
		float y = dlgNow.Height() *1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);

		CRect childSaved;

		CWnd* pWnd = GetWindow(GW_CHILD);

		while (pWnd)
		{
			childSaved = listRect.GetNext(pos);                                        //���λ�ȡ�Ӵ����Rect
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left)*x;       //���ݱ��������ؼ��������Ҿ���Ի���ľ���
			childSaved.right = dlgNow.right + (childSaved.right - dlgSaved.right)*x;
			childSaved.top = dlgNow.top + (childSaved.top - dlgSaved.top)*y;
			childSaved.bottom = dlgNow.bottom + (childSaved.bottom - dlgSaved.bottom)*y;
			ScreenToClient(childSaved);
			pWnd->MoveWindow(childSaved);
			pWnd = pWnd->GetNextWindow();
		}
	}
}


BOOL CBook::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rc;

	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 150);
	m_list.InsertColumn(1, _T("����ʱ��"), LVCFMT_CENTER, 240);
	m_list.InsertColumn(2, _T("Ԥ�Ƶ���ʱ��"), LVCFMT_CENTER, 240);
	m_list.InsertColumn(3, _T("����Ʊ"), LVCFMT_CENTER, 120);
	m_list.InsertColumn(4, _T("�۸�"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(5, _T("����Ʊ"), LVCFMT_CENTER, 120);
	m_list.InsertColumn(6, _T("�۸�"), LVCFMT_CENTER, 110);

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


	SYSTEMTIME st;
	CString str;

	GetLocalTime(&st);
	str.Format(L"%4d", st.wYear);
	m_year.AddString(str);
	m_year.SetCurSel(0);
	if (st.wMonth > 10)
	{
		str.Format(L"%4d", st.wYear + 1);
		m_year.AddString(str);
	}

	m_mon.SetCurSel(st.wMonth - 1);
	m_day.SetCurSel(st.wDay - 1);

	string area("1;s;select AreaName from Area;");
	int npos;
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;

	try
	{
		client->init();
		client->Connect();
		client->Send(deal->RepaceSpace(area));
		area = client->Recv();
		client->closeclient();

		while ((npos = area.find(":")) != area.npos)
		{
			str = area.substr(0, area.find(";") + 1).c_str();
			area.erase(0, area.find(";") + 1);
			m_setoff.AddString(str);
			m_arrive.AddString(str);
		}
	}
	catch (SocketFail e)
	{
		//MessageBoxA(m_hWnd, "�������Ӵ���", "Error!", MB_OK | MB_ICONWARNING);
	}

	if (info.user == 'c')
	{
		GetDlgItem(IDC_BOOK_STA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_NUM)->ShowWindow(SW_SHOW);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CBook::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	/*NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		CString strtemp;
		strtemp.Format(L"�������ǵ�%d�е�%d��", pNMListView->iItem, pNMListView->iSubItem);
		AfxMessageBox(strtemp);
	}*/

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_list.SubItemHitTest(&lvinfo);
	if (nItem != -1)
	{
		CString str;
		string ticket;
		if (lvinfo.iSubItem > 2)
		{
			if (lvinfo.iSubItem < 5)
			{
				ticket = "����";
				this->tickets = 4;
			}
			else
			{
				ticket = "����";
				this->tickets = 6;
			}
			str = ticket.c_str();
			str.Format(L"%s�ź��� %sƱ �۸�%s", m_list.GetItemText(lvinfo.iItem, 0), str, m_list.GetItemText(lvinfo.iItem, this->tickets));
			row = lvinfo.iItem;

			SetDlgItemTextW(IDC_BOOK_SHOW,str);
		}
	}

	*pResult = 0;
}


void CBook::OnBnClickedBookFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	int pos, npos, i = 0, line = 0;
	CString cstr, time, year, mon, day;
	string str("1;s;select Num, SetOff, Arrive, Business, BusPrice, Normal, Price from Flight_view where Cancel = false and SetOff > ");

	GetDlgItemText(IDC_BOOK_SET, m_set);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_set, -1, NULL, NULL, 0, NULL);
	char *set = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_set, -1, set, dwNum, 0, NULL);

	GetDlgItemText(IDC_BOOK_OFF, m_off);
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_off, -1, NULL, NULL, 0, NULL);
	char *off = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_off, -1, off, dwNum, 0, NULL);

	GetDlgItemText(IDC_BOOK_YEAR, year);
	GetDlgItemText(IDC_BOOK_MON, mon);
	GetDlgItemText(IDC_BOOK_DAY, day);
	time = year + L"-" + mon + L"-" + day;
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, time, -1, NULL, NULL, 0, NULL);
	char *setoff = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, time, -1, setoff, dwNum, 0, NULL);

	str += setoff;
	str += " and AreaName = '";
	str += set;
	str += "' and Expr1 = '";
	str += off;
	str += "';";

	try
	{
		client->init();
		client->Connect();
		client->Send(deal->RepaceSpace(str));
		str = client->Recv();
		client->closeclient();

		if (!str.compare("Fail") || str == "")
			MessageBoxA(m_hWnd, "û���κμ�¼", "Option", MB_OK | MB_ICONWARNING);

		while ((npos = str.find(";")) != str.npos)
		{
			i = 1;

			cstr = str.substr(0, str.find("|")).c_str();
			str.erase(0, str.find("|") + 1);
			m_list.InsertItem(line, cstr);
			while ((pos = str.find("|")) < npos && pos != str.npos)
			{
				cstr = str.substr(0, str.find("|")).c_str();
				str.erase(0, str.find("|") + 1);
				m_list.SetItemText(line, i, cstr);
				i++;
				npos = str.find(";");
			}
			cstr = str.substr(0, str.find(";")).c_str();
			str.erase(0, str.find(";") + 1);
			m_list.SetItemText(line, i, cstr);

			line++;
		}
	}
	catch (SocketFail e)
	{
		MessageBoxA(m_hWnd, "�������Ӵ���", "Error!", MB_OK | MB_ICONWARNING);
	}

	m_list.SetRedraw(TRUE);

	delete off;
	delete set;
	delete deal;
	delete client;
}


//void CBook::OnEnChangeBookSet()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CEdit *pBox = (CEdit *)GetDlgItem(IDC_BOOK_SET);
//	pBox->GetWindowTextW(m_set);
//}


//void CBook::OnEnChangeBookOff()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CEdit *pBox = (CEdit *)GetDlgItem(IDC_BOOK_OFF);
//	pBox->GetWindowTextW(m_off);
//}


void CBook::OnCbnSelchangeBookMon()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int tmp;
	CString year, mon;

	GetDlgItemText(IDC_BOOK_YEAR, year);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, year, -1, NULL, NULL, 0, NULL);
	char *y = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, year, -1, y, dwNum, 0, NULL);
	tmp = atoi(y);

	if (tmp % 4 == 0)
	{
		day[1] = 29;
		if (tmp % 100 == 0 && tmp % 400 != 0)
			day[1] = 28;
	}

	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, mon, -1, NULL, NULL, 0, NULL);
	char *m = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, mon, -1, m, dwNum, 0, NULL);
	tmp = atoi(m) - 1;

	char *d = new char[3];
	CString cday;

	m_day.ResetContent();
	for (int i = 1; i <= day[tmp]; i++)
	{
		itoa(i, d, 10);
		cday = d;
		m_day.AddString(cday);
	}

	m_day.SetCurSel(0);
}

void CBook::OnBnClickedBookBook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char in[10000];
	CString note, num;
	string which;

	if (info.user == 'c')
		GetDlgItemText(IDC_BOOK_NUM, num);
	else
		num = "1";

	if (num == "")
	{
		MessageBox(L"��δ��д��Ҫ��Ʊ��", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (num > m_list.GetItemText(row, tickets - 1))
	{
		MessageBox(L"������Ԥ��������", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (tickets == 4)
		which = "Business";
	else
		which = "Normal";

	GetDlgItemText(IDC_BOOK_SHOW, note);
	if (MessageBox(L"����Ԥ�� " + num + L"ö " +  note, L"Option", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
		return;

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	string str;
	CTime now = CTime::GetCurrentTime();

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, note, -1, NULL, NULL, 0, NULL);
	char *infor = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, note, -1, infor, dwNum, 0, NULL);

	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, num, -1, NULL, NULL, 0, NULL);
	char *n = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, num, -1, n, dwNum, 0, NULL);

	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_list.GetItemText(row, 1), -1, NULL, NULL, 0, NULL);
	char *tm = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_list.GetItemText(row, 1), -1, tm, dwNum, 0, NULL);
	str = tm;

	//int year, mon, day, hour, min, sec;
	//year = strtol(str.substr(0, 4).c_str(), NULL, 10);
	CTime setoff(strtol(str.substr(0, 4).c_str(), NULL, 10), strtol(str.substr(5, 2).c_str(), NULL, 10) + 1, strtol(str.substr(8, 2).c_str(), NULL, 10), 0, 0, 0);
	CTimeSpan span = setoff - now;

	str = infor;
	if (info.user != 'c')
		sprintf(in, "1;s;select Pos from GiveUp where Num = '%s';", str.substr(0, 3).c_str());
	else
		sprintf(in, "2;c;%s;u;update RealNum set %s = %s - %d where Num = '%s';",
			str.substr(0, 3).c_str(), which.c_str(), which.c_str(), atoi(n), str.substr(0, 3).c_str());
	str = in;

	try
	{
		client->init();
		client->Connect();
		client->Send(deal->RepaceSpace(str));
		str = client->Recv();
		client->closeclient();
	}
	catch (SocketFail e)
	{
		delete deal;
		delete client;
		MessageBox(L"�������Ӵ���Ԥ��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str == "Fail")
	{
		delete deal;
		delete client;
		MessageBox(L"Ԥ��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	span.GetDays();

	if (str != "")
	{//�����У��ѱ��˻ص�λ��
		int pos;
		if ((pos = str.find("|")) == str.npos)
			pos = str.find(";");

		string Num(infor, 3);

		sprintf(in, "2;d;delete from GiveUp where Num = '%s' and Pos = '%s';u;update RealNum set %s = %s + 1 where Num = '%s';",
					Num.c_str(), str.substr(0, pos).c_str(), which.c_str(), which.c_str(), Num.c_str());
		str = in;
	}
	else
	{
		str = infor;
		sprintf(in, "3;b;%s|%.2d;i;insert into Tickets values('&r', '%s', 0, %s);u;update RealNum set %s = %s + 1 where Num = '%s';",
					str.substr(0, 3).c_str(), (int)span.GetDays(), info.id.c_str(), str.substr(str.find("��") + 2).c_str(), 
					which.c_str(), which.c_str(), str.substr(0, 3).c_str());
		str = in;
	}

	try
	{
		client->init();
		client->Connect();
		client->Send(deal->RepaceSpace(str));
		str = client->Recv();
		client->closeclient();
	}
	catch (SocketFail e)
	{
		delete deal;
		delete client;
		MessageBox(L"�������Ӵ���Ԥ��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str.substr(str.find("|") + 1, 2) == "OK")
	{
		CString t;
		t = str.substr(0, 18).c_str();
		MessageBox(L"Ʊ��Ϊ " + t, L"Ԥ���ɹ�\nƱ���Ѹ��Ƶ�������", MB_OK | MB_ICONASTERISK);

		if (OpenClipboard())
		{
			HGLOBAL clipbuffer;
			char   *buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, str.substr(0, 18).size() + 1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(str.substr(0, 18).c_str()));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT, clipbuffer);
			CloseClipboard();
		}
	}
	else
		MessageBox(L"Ԥ��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

	delete deal;
	delete client;
}