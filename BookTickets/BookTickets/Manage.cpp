// Manage.cpp : 实现文件
//

#include "stdafx.h"
#include "BookTickets.h"
#include "Manage.h"
#include "afxdialogex.h"

#include <regex>
#include <string>
#include "ClientSocket.h"
#include "Deal.h"

using std::regex_match;
using std::string;


// CManage 对话框

IMPLEMENT_DYNAMIC(CManage, CDialogEx)

CManage::CManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAN, pParent)
{
	m_tid = false;
	m_id = false;
	m_age = false;
	m_com = false;
}

CManage::~CManage()
{
}

void CManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CManage, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_MAN_TICKET, &CManage::OnBnClickedManTicket)
	ON_BN_CLICKED(IDC_MAN_PAY, &CManage::OnBnClickedManPay)
	ON_BN_CLICKED(IDC_MAN_GIV, &CManage::OnBnClickedManGiv)
	ON_BN_CLICKED(IDC_MAN_COM, &CManage::OnBnClickedManCom)
	ON_BN_CLICKED(IDC_MAN_AGE, &CManage::OnBnClickedManAge)
	ON_EN_CHANGE(IDC_MAN_ID, &CManage::OnEnChangeManId)
	ON_EN_CHANGE(IDC_MAN_TID, &CManage::OnEnChangeManTid)
	ON_EN_CHANGE(IDC_MAN_COMID, &CManage::OnEnChangeManComid)
	ON_EN_CHANGE(IDC_MAN_AGEID, &CManage::OnEnChangeManAgeid)
END_MESSAGE_MAP()


// CManage 消息处理程序


BOOL CManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

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

	SetDlgItemText(IDC_MAN_ID, L"请输入使用者的证件号");
	SetDlgItemText(IDC_MAN_TID, L"请输入票号");
	SetDlgItemText(IDC_MAN_COMID, L"公司名称,不填显示全部");
	SetDlgItemText(IDC_MAN_AGEID, L"旅行社名称,不填显示全部");
	
	CRect rc;

	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CManage::OnSize(UINT nType, int cx, int cy)
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

void CManage::OnBnClickedManTicket()
{
	// TODO: 在此添加控件通知处理程序代码

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str;
	char s[1000];
	SYSTEMTIME st;

	GetLocalTime(&st);
	GetDlgItemText(IDC_MAN_ID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	if (!regex_match(id, compare))
	{
		MessageBox(L"请输入正确的证件号", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	sprintf(s, "1;s;select NUm, SetOff from Ticket_view where SetOff > '%4d-%.2d-%.2d' \
				and Id = (select Id from Info where Proof = '%s');", st.wYear, st.wMonth - 1, st.wDay, id);
	str = s;

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
		MessageBox(L"网络连接错误，修改失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"查询失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str == "")
	{
		MessageBox(L"没有记录", L"Option", MB_OK | MB_ICONASTERISK);
		return;
	}

	int pos, npos;
	string message("");
	CString mesg;

	while ((pos = str.find(";")) != str.npos)
	{
		npos = str.find("|");
		sprintf(s, "航班号：%s, 出发时间：%s\n", str.substr(0, npos).c_str(), str.substr(npos + 1, pos - npos -1).c_str());
		message += s;
		str.erase(0, pos + 1);
	}

	mesg = message.c_str();
	MessageBox(mesg, L"所有机票");

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManPay()
{
	// TODO: 在此添加控件通知处理程序代码
	
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str;
	char s[1000];

	GetDlgItemText(IDC_MAN_TID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	if (!regex_match(id, compare))
	{
		MessageBox(L"请输入正确的机票码", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	sprintf(s, "1;u;update Tickets set Clear = 1 where TicketsId = '%s';", id);
	str = s;

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
		MessageBox(L"网络连接错误，付款失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"付款失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	MessageBox(L"付款成功", L"Option", MB_OK | MB_ICONASTERISK);

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManGiv()
{
	// TODO: 在此添加控件通知处理程序代码

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str, which;
	char s[1000];

	GetDlgItemText(IDC_MAN_TID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	if (!regex_match(id, compare))
	{
		MessageBox(L"请输入正确的机票码", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	str = id;
	sprintf(s, "1;s;select Business from RealNum where Num = '%s'", str.substr(str.size() - 6, 3).c_str());

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
		MessageBox(L"网络连接错误，退票失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"退票失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	string pos = str.substr(0, str.find(";"));
	str = id;

	if (pos > str.substr(str.size() - 3, 3))
		which = "Business";
	else
		which = "Normal";

	sprintf(s, "3;d;delete from Tickets where TicketsId = '%s'; \
				i;insert into GiveUp values('%s', '%s'); \
				u;update RealNum set %s += 1 where Num = '%s'", 
				id, str.substr(str.size() - 6, 3).c_str(), str.substr(str.size() - 3, 3).c_str(), 
				which, str.substr(str.size() - 6, 3).c_str());
	str = s;

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
		MessageBox(L"网络连接错误，退票失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "OK")
		MessageBox(L"退票成功", L"Option", MB_OK | MB_ICONASTERISK);
	else
		MessageBox(L"退票失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManCom()
{
	// TODO: 在此添加控件通知处理程序代码
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str;
	char s[1000];

	GetDlgItemText(IDC_MAN_COMID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	str = id;
	if (id != "" && sid != L"公司名称,不填显示全部")
		sprintf(s, "1;s;select CompanyId, CompanyName, Num, Remark from Company_view \
					 where CompanyId = '%s' or CompanyName = '%s';", id, id);
	else
		sprintf(s, "1;s;select CompanyId, CompanyName, Num, Remark from Company_view;");
	str = s;

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
		MessageBox(L"网络连接错误，付款失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		delete id;
		delete deal;
		delete client;

		MessageBox(L"查询失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);

		return;
	}

	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));

	m_list.InsertColumn(0, _T("公司编号"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(1, _T("公司名"), LVCFMT_CENTER, 300);
	m_list.InsertColumn(2, _T("航班编号"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(3, _T("备注"), LVCFMT_CENTER, 400);

	if (str == "")
	{
		delete id;
		delete deal;
		delete client;

		MessageBox(L"查询无结果", L"Option!", MB_OK | MB_ICONASTERISK);

		return;
	}

	CString cstr;
	int i, pos = 0, npos = 0, line = 0;

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
		}
		cstr = str.substr(0, str.find(";")).c_str();
		str.erase(0, str.find(";") + 1);
		m_list.SetItemText(line, i, cstr);

		line++;
	}

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManAge()
{
	// TODO: 在此添加控件通知处理程序代码

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str;
	char s[1000];

	GetDlgItemText(IDC_MAN_AGEID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	if (id != "" && sid != L"旅行社名称,不填显示全部")
		sprintf(s, "1;s;select AgencyId, CompanyName, FlightId, TotalNum, SetOff, Price, SouName, DesName, \
					Cancel, Remark from Agency_view where AgencyId = '%s' or CompanyName = '%s';", id, id);
	else
		sprintf(s, "1;s;select AgencyId, CompanyName, FlightId, TotalNum, SetOff, Price, SouName, DesName, Cancel, Remark from Agency_view;");
	str = s;

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
		MessageBox(L"网络连接错误，查询失败", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		delete id;
		delete deal;
		delete client;
		
		MessageBox(L"查询失败，请检查输入是否有误！", L"ERROR!", MB_OK | MB_ICONWARNING);
		
		return;
	}

	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));

	m_list.InsertColumn(0, _T("编号"), LVCFMT_CENTER, 70);
	m_list.InsertColumn(1, _T("旅社名"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(2, _T("航班编号"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(3, _T("总人数"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(4, _T("出发时间"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(5, _T("价格"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(6, _T("出发地"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(7, _T("目的地"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(8, _T("付款"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(9, _T("备注"), LVCFMT_CENTER, 110);

	if (str == "")
	{
		delete id;
		delete deal;
		delete client;

		MessageBox(L"查询无结果", L"Option!", MB_OK | MB_ICONASTERISK);

		return;
	}

	CString cstr;
	int i, pos = 0, npos = 0, line = 0;

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
		}
		cstr = m_list.GetItemText(line, i - 1);
		if (cstr == "")
			m_list.SetItemText(line, i - 1, L"否");
		else
			m_list.SetItemText(line, i - 1, L"是");
		cstr = str.substr(0, str.find(";")).c_str();
		str.erase(0, str.find(";") + 1);
		m_list.SetItemText(line, i, cstr);

		line++;
	}

	delete id;
	delete deal;
	delete client;
}


void CManage::OnEnChangeManId()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;

	GetDlgItemText(IDC_MAN_ID, str);
	if (m_id == false && str != "请输入使用者的证件号")
	{
		((CEdit *)GetDlgItem(IDC_MAN_ID))->ModifyStyleEx(0, ES_NUMBER);
		m_id = true;
		SetDlgItemText(IDC_MAN_ID, L"");
	}
}


void CManage::OnEnChangeManTid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;

	GetDlgItemText(IDC_MAN_TID, str);
	if (m_tid == false && str != "请输入票号")
	{
		((CEdit *)GetDlgItem(IDC_MAN_TID))->ModifyStyleEx(0, ES_NUMBER);
		m_tid = true;
		SetDlgItemText(IDC_MAN_TID, L"");
	}
}


void CManage::OnEnChangeManComid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;

	GetDlgItemText(IDC_MAN_COMID, str);
	if (m_com == false && str != "公司名称,不填显示全部")
	{
		m_com = true;
		SetDlgItemText(IDC_MAN_COMID, L"");
	}
}


void CManage::OnEnChangeManAgeid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;

	GetDlgItemText(IDC_MAN_AGEID, str);
	if (m_age == false && str != "旅行社名称,不填显示全部")
	{
		m_age = true;
		SetDlgItemText(IDC_MAN_AGEID, L"");
	}
}
