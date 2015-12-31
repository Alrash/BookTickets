// Info.cpp : 实现文件
//

#include "stdafx.h"
#include "BookTickets.h"
#include "Info.h"
#include "afxdialogex.h"

#include <cstdio>
#include <regex>
#include <string>
#include "ClientSocket.h"
#include "Deal.h"

using std::regex_match;
using std::string;

extern Info info;

// CInfo 对话框

IMPLEMENT_DYNAMIC(CInfo, CDialogEx)

CInfo::CInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INFO, pParent)
{
	m_again = m_key = false;
}

CInfo::~CInfo()
{
}

void CInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CInfo, CDialogEx)
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_INFO_CHA, &CInfo::OnStnClickedInfoCha)
	ON_EN_CHANGE(IDC_INFO_KEY, &CInfo::OnEnChangeInfoKey)
	ON_EN_CHANGE(IDC_INFO_AGA, &CInfo::OnEnChangeInfoAga)
	ON_BN_CLICKED(IDC_INFO_KCOM, &CInfo::OnBnClickedInfoKcom)
	ON_BN_CLICKED(IDC_INFO_CHAG, &CInfo::OnBnClickedInfoChag)
	ON_BN_CLICKED(IDC_INFO_FIN, &CInfo::OnBnClickedInfoFin)
	ON_BN_CLICKED(IDC_INFO_CAL, &CInfo::OnBnClickedInfoCal)
	ON_BN_CLICKED(IDC_INFO_FINDALL, &CInfo::OnBnClickedInfoFindall)
	ON_BN_CLICKED(IDC_INFO_FIND, &CInfo::OnBnClickedInfoFind)
	ON_BN_CLICKED(IDC_INFO_DEL, &CInfo::OnBnClickedInfoDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CInfo::OnLvnItemchangedList2)
END_MESSAGE_MAP()


// CInfo 消息处理程序


void CInfo::OnSize(UINT nType, int cx, int cy)
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


BOOL CInfo::OnInitDialog()
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

	CRect rc;

	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("航班号"), LVCFMT_CENTER, 70);
	m_list.InsertColumn(1, _T("票号"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(2, _T("出发地"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(3, _T("目的地"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(4, _T("出发时间"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(6, _T("价格"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(6, _T("是否付款"), LVCFMT_CENTER, 110);

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	string str("1;s;select Name, Sex, Nickname, Email, Proof, Oranization, Remark from Info_view where Id = '");

	str += info.id;
	str += "';";

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
		MessageBox(L"网络连接失败", L"ERROR!", MB_OK | MB_ICONWARNING);
		ExitProcess(EOF);
	}

	if (!str.compare("Fail"))
	{
		MessageBox(L"ERROR", L"ERROR!", MB_OK | MB_ICONWARNING);
		return false;
	}

	CString name, nick, sex, com, remark, id, mail;
	int pos, npos;

	pos = 0, npos = str.find("|");
	name = str.substr(pos, npos).c_str();
	pos = npos + 1, npos = str.find("|", pos);
	sex = str.substr(pos, npos - pos).c_str();
	pos = npos + 1, npos = str.find("|", pos);
	nick = str.substr(pos, npos - pos).c_str();
	pos = npos + 1, npos = str.find("|", pos);
	mail = str.substr(pos, npos - pos).c_str();
	pos = npos + 1, npos = str.find("|", pos);
	id = str.substr(pos, npos - pos).c_str();
	pos = npos + 1, npos = str.find("|", pos);
	com = str.substr(pos, npos - pos).c_str();
	pos = npos + 1, npos = str.find(";", pos);
	remark = str.substr(pos, npos - pos).c_str();

	SetDlgItemText(IDC_INFO_NAME, name);
	SetDlgItemText(IDC_INFO_MAIL, mail);
	if (sex == "")
		sex = L"Male";
	else
		sex = L"Female";
	SetDlgItemText(IDC_INFO_NICK, nick);
	SetDlgItemText(IDC_INFO_SEX, sex);
	SetDlgItemText(IDC_INFO_COM, com);
	SetDlgItemText(IDC_INFO_REMA, remark);
	SetDlgItemText(IDC_INFO_ID, id);

	delete deal;
	delete client;

	if (info.user == 'c')
	{
		GetDlgItem(IDC_INFO_CHA)->EnableWindow(false);
		GetDlgItem(IDC_INFO_CHAG)->EnableWindow(false);
		GetDlgItem(IDC_INFO_FINDALL)->EnableWindow(false);
		GetDlgItem(IDC_INFO_FIND)->EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CInfo::OnStnClickedInfoCha()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_KCOM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INFO_KEY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INFO_AGA)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INFO_CAL)->ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_INFO_KEY, L"新的密码");
	SetDlgItemText(IDC_INFO_AGA, L"请再输一遍");
}


void CInfo::OnEnChangeInfoKey()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;
	GetDlgItemText(IDC_INFO_KEY, str);
	//MessageBox(str);
	CEdit *pBox;
	pBox = (CEdit *)GetDlgItem(IDC_INFO_KEY);
	if (m_key == false && str != "新的密码")
	{
		pBox->SetPasswordChar('*');
		pBox->ModifyStyleEx(0, ES_PASSWORD);
		m_key = true;
		SetDlgItemText(IDC_INFO_KEY, L"");
	}
}


void CInfo::OnEnChangeInfoAga()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;
	GetDlgItemText(IDC_INFO_AGA, str);
	CEdit *pBox;
	pBox = (CEdit *)GetDlgItem(IDC_INFO_AGA);
	if (m_again == false && str != "请再输一遍")
	{
		pBox->SetPasswordChar('*');
		pBox->ModifyStyleEx(0, ES_PASSWORD);
		m_again = true;
		SetDlgItemText(IDC_INFO_AGA, L"");
	}
}


void CInfo::OnBnClickedInfoKcom()
{
	// TODO: 在此添加控件通知处理程序代码
	CString key, again;
	string str;
	char s[2000];

	GetDlgItemText(IDC_INFO_KEY, key);
	GetDlgItemText(IDC_INFO_AGA, again);

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, key, -1, NULL, NULL, 0, NULL);
	char *passwd = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, key, -1, passwd, dwNum, 0, NULL);

	if (!regex_match(passwd, ppasswd))
	{
		MessageBoxA(m_hWnd, "需要六位以上的数组和字母的组合", "密码不规范!", MB_OK | MB_ICONWARNING);
		return;
	}
	if (key != again)
	{
		MessageBoxA(m_hWnd, "两次密码输入不一致", "ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	sprintf(s, "1;u;update Passinfo set Password = '%s' where Id = '%s';", passwd, info.id.c_str());
	str = s;

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;

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
		delete client;
		delete deal;
		MessageBoxA(m_hWnd, "网络错误， 修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
	}

	if (str == "Fail")
	{
		MessageBoxA(m_hWnd, "修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
	}

	GetDlgItem(IDC_INFO_KCOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_KEY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_AGA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_CAL)->ShowWindow(SW_HIDE);

	MessageBoxA(m_hWnd, "修改密码成功", "OK", MB_OK | MB_ICONASTERISK);

	delete client;
	delete deal;
}


void CInfo::OnBnClickedInfoChag()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_NICK)->EnableWindow(true);
	GetDlgItem(IDC_INFO_MAIL)->EnableWindow(true);
	GetDlgItem(IDC_INFO_COM)->EnableWindow(true);
	GetDlgItem(IDC_INFO_REMA)->EnableWindow(true);
	GetDlgItem(IDC_INFO_FIN)->ShowWindow(SW_SHOW);

	GetDlgItemText(IDC_INFO_NICK, m_nick);
	GetDlgItemText(IDC_INFO_MAIL, m_mail);
	GetDlgItemText(IDC_INFO_COM, m_com);
	GetDlgItemText(IDC_INFO_REMA, m_remark);
}


void CInfo::OnBnClickedInfoFin()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_NICK)->EnableWindow(false);
	GetDlgItem(IDC_INFO_MAIL)->EnableWindow(false);
	GetDlgItem(IDC_INFO_COM)->EnableWindow(false);
	GetDlgItem(IDC_INFO_REMA)->EnableWindow(false);
	GetDlgItem(IDC_INFO_FIN)->ShowWindow(SW_HIDE);

	CString nick, mail, com, remark;

	GetDlgItemText(IDC_INFO_NICK, nick);
	GetDlgItemText(IDC_INFO_MAIL, mail);
	GetDlgItemText(IDC_INFO_COM, com);
	GetDlgItemText(IDC_INFO_REMA, remark);

	if (nick != m_nick || mail != m_mail || com != m_com || remark != m_remark)
	{
		ClientSocket *client = new ClientSocket;
		Deal *deal = new Deal;
		string str;
		char s[10000];

		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, nick, -1, NULL, NULL, 0, NULL);
		char *cnick = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, nick, -1, cnick, dwNum, 0, NULL);

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, mail, -1, NULL, NULL, 0, NULL);
		char *cmail = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, mail, -1, cmail, dwNum, 0, NULL);

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, com, -1, NULL, NULL, 0, NULL);
		char *ccom = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, com, -1, ccom, dwNum, 0, NULL);

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, remark, -1, NULL, NULL, 0, NULL);
		char *cremark = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, remark, -1, cremark, dwNum, 0, NULL);

		sprintf(s, "2;u;update Info set Oranization = '%s' where Id = '%s';u;update Passinfo set Nickname = '%s', Email = '%s', Remark = '%s' where Id = '%s';", ccom, info.id.c_str(), cnick, cmail, cremark, info.id.c_str());
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
			delete client;
			delete deal;
			MessageBoxA(m_hWnd, "网络错误， 修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
			return;
		}

		MessageBoxA(m_hWnd, "修改成功", "OK!", MB_OK | MB_ICONASTERISK);

		delete client;
		delete deal;
	}
}

void CInfo::OnBnClickedInfoCal()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_KCOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_KEY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_AGA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INFO_CAL)->ShowWindow(SW_HIDE);
}

void CInfo::OnBnClickedInfoFindall()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_DEL)->ShowWindow(SW_SHOW);
	m_list.DeleteAllItems();

	string str("1;s;select Num, TicketsId, SouId, DesId, SetOff, Price, Clear from Ticket_view where Id ='");
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;

	str += info.id;
	str += "';";

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
		delete client;
		delete deal;
		MessageBoxA(m_hWnd, "网络错误， 修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	int pos, npos, i, line = 0;
	CString cstr;

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
			npos = npos - 1 - pos;
			i++;
		}
		cstr = str.substr(0, str.find(";")).c_str();
		str.erase(0, str.find(";") + 1);
		if (cstr == "")
			m_list.SetItemText(line, i, L"否");
		else
			m_list.SetItemText(line, i, L"是");

		line++;
	}

	reflash = false;

	delete client;
	delete deal;
}


void CInfo::OnBnClickedInfoFind()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_INFO_DEL)->ShowWindow(SW_SHOW);
	m_list.DeleteAllItems();

	string str;
	char s[1000];
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	SYSTEMTIME st;

	GetLocalTime(&st);
	sprintf(s, "1;s;select Num, TicketsId, SouId, DesId, SetOff, Price, Clear from Ticket_view where Id = '%s' and SetOff > '%4d-%.2d-%.2d';",
		info.id.c_str(), st.wYear, st.wMonth, st.wDay);
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
		delete client;
		delete deal;
		MessageBoxA(m_hWnd, "网络错误， 修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
		
		return;
	}

	int pos, npos, i, line = 0;
	CString cstr;

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
			npos = npos - 1 - pos;
			i++;
		}
		cstr = str.substr(0, str.find(";")).c_str();
		str.erase(0, str.find(";") + 1);
		if (cstr == "")
			m_list.SetItemText(line, i, L"否");
		else
			m_list.SetItemText(line, i, L"是");

		line++;
	}

	reflash = true;

	delete client;
	delete deal;
}


void CInfo::OnBnClickedInfoDel()
{
	// TODO: 在此添加控件通知处理程序代码

	if (MessageBox(L"需要取消" + m_num + L"票吗？", L"注意", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
		return;

	GetDlgItem(IDC_INFO_DEL)->ShowWindow(SW_HIDE);

	string str;
	char s[1000];
	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_num, -1, NULL, NULL, 0, NULL);
	char *num = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_num, -1, num, dwNum, 0, NULL);

	str = num;
	sprintf(s, "2;d;delete from Tickets where TicketsId = '%s';i;insert into GiveUp values('%s', '%s');", num, str.substr(12, 3).c_str(), str.substr(15, 3).c_str());
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
		delete client;
		delete deal;
		MessageBoxA(m_hWnd, "网络错误， 修改失败", "ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str.compare("OK"))			//!=Ok
		MessageBoxA(m_hWnd, "退票失败", "ERROR!", MB_OK | MB_ICONWARNING);
	else
	{
		MessageBoxA(m_hWnd, "退票成功", "Option!", MB_OK | MB_ICONWARNING);
		if (reflash == false)
			this->OnBnClickedInfoFindall();
		else
			this->OnBnClickedInfoFind();
	}

	delete client;
	delete deal;
}


void CInfo::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;

	if (-1 != pNMListView->iItem)
	{
		if (m_list.GetItemText(pNMListView->iItem, 1) != "")
			m_num = m_list.GetItemText(pNMListView->iItem, 1);
	}
}