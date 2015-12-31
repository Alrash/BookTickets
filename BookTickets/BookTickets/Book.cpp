// Book.cpp : 实现文件
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

// CBook 对话框

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


// CBook 消息处理程序


void CBook::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (listRect.GetCount() > 0)
	{
		CRect dlgNow;
		GetWindowRect(&dlgNow);
		POSITION pos = listRect.GetHeadPosition();                                     //第一个保存的是对话框的Rect

		CRect dlgSaved;
		dlgSaved = listRect.GetNext(pos);
		ScreenToClient(dlgNow);

		float x = dlgNow.Width() * 1.0 / dlgSaved.Width();                             //根据当前和之前保存的对话框的宽高求比例
		float y = dlgNow.Height() *1.0 / dlgSaved.Height();
		ClientToScreen(dlgNow);

		CRect childSaved;

		CWnd* pWnd = GetWindow(GW_CHILD);

		while (pWnd)
		{
			childSaved = listRect.GetNext(pos);                                        //依次获取子窗体的Rect
			childSaved.left = dlgNow.left + (childSaved.left - dlgSaved.left)*x;       //根据比例调整控件上下左右距离对话框的距离
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

	// TODO:  在此添加额外的初始化
	CRect rc;

	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("航班号"), LVCFMT_CENTER, 150);
	m_list.InsertColumn(1, _T("出发时间"), LVCFMT_CENTER, 240);
	m_list.InsertColumn(2, _T("预计到达时间"), LVCFMT_CENTER, 240);
	m_list.InsertColumn(3, _T("商务票"), LVCFMT_CENTER, 120);
	m_list.InsertColumn(4, _T("价格"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(5, _T("经济票"), LVCFMT_CENTER, 120);
	m_list.InsertColumn(6, _T("价格"), LVCFMT_CENTER, 110);

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
		//MessageBoxA(m_hWnd, "网络连接错误", "Error!", MB_OK | MB_ICONWARNING);
	}

	if (info.user == 'c')
	{
		GetDlgItem(IDC_BOOK_STA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BOOK_NUM)->ShowWindow(SW_SHOW);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBook::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	/*NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		CString strtemp;
		strtemp.Format(L"单击的是第%d行第%d列", pNMListView->iItem, pNMListView->iSubItem);
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
				ticket = "商务";
				this->tickets = 4;
			}
			else
			{
				ticket = "经济";
				this->tickets = 6;
			}
			str = ticket.c_str();
			str.Format(L"%s号航班 %s票 价格￥%s", m_list.GetItemText(lvinfo.iItem, 0), str, m_list.GetItemText(lvinfo.iItem, this->tickets));
			row = lvinfo.iItem;

			SetDlgItemTextW(IDC_BOOK_SHOW,str);
		}
	}

	*pResult = 0;
}


void CBook::OnBnClickedBookFind()
{
	// TODO: 在此添加控件通知处理程序代码
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
			MessageBoxA(m_hWnd, "没有任何记录", "Option", MB_OK | MB_ICONWARNING);

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
		MessageBoxA(m_hWnd, "网络连接错误", "Error!", MB_OK | MB_ICONWARNING);
	}

	m_list.SetRedraw(TRUE);

	delete off;
	delete set;
	delete deal;
	delete client;
}


//void CBook::OnEnChangeBookSet()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	CEdit *pBox = (CEdit *)GetDlgItem(IDC_BOOK_SET);
//	pBox->GetWindowTextW(m_set);
//}


//void CBook::OnEnChangeBookOff()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	CEdit *pBox = (CEdit *)GetDlgItem(IDC_BOOK_OFF);
//	pBox->GetWindowTextW(m_off);
//}


void CBook::OnCbnSelchangeBookMon()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	char in[10000];
	CString note, num;
	string which;

	if (info.user == 'c')
		GetDlgItemText(IDC_BOOK_NUM, num);
	else
		num = "1";

	if (num == "")
	{
		MessageBox(L"还未填写需要机票数", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (num > m_list.GetItemText(row, tickets - 1))
	{
		MessageBox(L"超出能预定的张数", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (tickets == 4)
		which = "Business";
	else
		which = "Normal";

	GetDlgItemText(IDC_BOOK_SHOW, note);
	if (MessageBox(L"即将预定 " + num + L"枚 " +  note, L"Option", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
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
		MessageBox(L"网络连接错误，预定失败", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str == "Fail")
	{
		delete deal;
		delete client;
		MessageBox(L"预定失败", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	span.GetDays();

	if (str != "")
	{//表明有，已被退回的位置
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
					str.substr(0, 3).c_str(), (int)span.GetDays(), info.id.c_str(), str.substr(str.find("￥") + 2).c_str(), 
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
		MessageBox(L"网络连接错误，预定失败", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str.substr(str.find("|") + 1, 2) == "OK")
	{
		CString t;
		t = str.substr(0, 18).c_str();
		MessageBox(L"票号为 " + t, L"预定成功\n票号已复制到剪贴板", MB_OK | MB_ICONASTERISK);

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
		MessageBox(L"预定失败", L"ERROR!", MB_OK | MB_ICONWARNING);

	delete deal;
	delete client;
}