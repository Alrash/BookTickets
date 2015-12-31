// Manage.cpp : ʵ���ļ�
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


// CManage �Ի���

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


// CManage ��Ϣ�������


BOOL CManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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

	SetDlgItemText(IDC_MAN_ID, L"������ʹ���ߵ�֤����");
	SetDlgItemText(IDC_MAN_TID, L"������Ʊ��");
	SetDlgItemText(IDC_MAN_COMID, L"��˾����,������ʾȫ��");
	SetDlgItemText(IDC_MAN_AGEID, L"����������,������ʾȫ��");
	
	CRect rc;

	m_list.GetClientRect(&rc);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CManage::OnSize(UINT nType, int cx, int cy)
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

void CManage::OnBnClickedManTicket()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
		MessageBox(L"��������ȷ��֤����", L"ERROR!", MB_OK | MB_ICONWARNING);
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
		MessageBox(L"�������Ӵ����޸�ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"��ѯʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	if (str == "")
	{
		MessageBox(L"û�м�¼", L"Option", MB_OK | MB_ICONASTERISK);
		return;
	}

	int pos, npos;
	string message("");
	CString mesg;

	while ((pos = str.find(";")) != str.npos)
	{
		npos = str.find("|");
		sprintf(s, "����ţ�%s, ����ʱ�䣺%s\n", str.substr(0, npos).c_str(), str.substr(npos + 1, pos - npos -1).c_str());
		message += s;
		str.erase(0, pos + 1);
	}

	mesg = message.c_str();
	MessageBox(mesg, L"���л�Ʊ");

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManPay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
		MessageBox(L"��������ȷ�Ļ�Ʊ��", L"ERROR!", MB_OK | MB_ICONWARNING);
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
		MessageBox(L"�������Ӵ��󣬸���ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"����ʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);
		return;
	}

	MessageBox(L"����ɹ�", L"Option", MB_OK | MB_ICONASTERISK);

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManGiv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
		MessageBox(L"��������ȷ�Ļ�Ʊ��", L"ERROR!", MB_OK | MB_ICONWARNING);
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
		MessageBox(L"�������Ӵ�����Ʊʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "Fail")
	{
		MessageBox(L"��Ʊʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);

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
		MessageBox(L"�������Ӵ�����Ʊʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

		delete id;
		delete deal;
		delete client;

		return;
	}

	if (str == "OK")
		MessageBox(L"��Ʊ�ɹ�", L"Option", MB_OK | MB_ICONASTERISK);
	else
		MessageBox(L"��Ʊʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);

	delete id;
	delete deal;
	delete client;
}


void CManage::OnBnClickedManCom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	if (id != "" && sid != L"��˾����,������ʾȫ��")
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
		MessageBox(L"�������Ӵ��󣬸���ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

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

		MessageBox(L"��ѯʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);

		return;
	}

	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));

	m_list.InsertColumn(0, _T("��˾���"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(1, _T("��˾��"), LVCFMT_CENTER, 300);
	m_list.InsertColumn(2, _T("������"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(3, _T("��ע"), LVCFMT_CENTER, 400);

	if (str == "")
	{
		delete id;
		delete deal;
		delete client;

		MessageBox(L"��ѯ�޽��", L"Option!", MB_OK | MB_ICONASTERISK);

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	CString sid;
	string str;
	char s[1000];

	GetDlgItemText(IDC_MAN_AGEID, sid);
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, sid, -1, id, dwNum, 0, NULL);

	if (id != "" && sid != L"����������,������ʾȫ��")
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
		MessageBox(L"�������Ӵ��󣬲�ѯʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

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
		
		MessageBox(L"��ѯʧ�ܣ����������Ƿ�����", L"ERROR!", MB_OK | MB_ICONWARNING);
		
		return;
	}

	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));

	m_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, 70);
	m_list.InsertColumn(1, _T("������"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(2, _T("������"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(3, _T("������"), LVCFMT_CENTER, 130);
	m_list.InsertColumn(4, _T("����ʱ��"), LVCFMT_CENTER, 200);
	m_list.InsertColumn(5, _T("�۸�"), LVCFMT_CENTER, 100);
	m_list.InsertColumn(6, _T("������"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(7, _T("Ŀ�ĵ�"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(8, _T("����"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(9, _T("��ע"), LVCFMT_CENTER, 110);

	if (str == "")
	{
		delete id;
		delete deal;
		delete client;

		MessageBox(L"��ѯ�޽��", L"Option!", MB_OK | MB_ICONASTERISK);

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
			m_list.SetItemText(line, i - 1, L"��");
		else
			m_list.SetItemText(line, i - 1, L"��");
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;

	GetDlgItemText(IDC_MAN_ID, str);
	if (m_id == false && str != "������ʹ���ߵ�֤����")
	{
		((CEdit *)GetDlgItem(IDC_MAN_ID))->ModifyStyleEx(0, ES_NUMBER);
		m_id = true;
		SetDlgItemText(IDC_MAN_ID, L"");
	}
}


void CManage::OnEnChangeManTid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;

	GetDlgItemText(IDC_MAN_TID, str);
	if (m_tid == false && str != "������Ʊ��")
	{
		((CEdit *)GetDlgItem(IDC_MAN_TID))->ModifyStyleEx(0, ES_NUMBER);
		m_tid = true;
		SetDlgItemText(IDC_MAN_TID, L"");
	}
}


void CManage::OnEnChangeManComid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;

	GetDlgItemText(IDC_MAN_COMID, str);
	if (m_com == false && str != "��˾����,������ʾȫ��")
	{
		m_com = true;
		SetDlgItemText(IDC_MAN_COMID, L"");
	}
}


void CManage::OnEnChangeManAgeid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;

	GetDlgItemText(IDC_MAN_AGEID, str);
	if (m_age == false && str != "����������,������ʾȫ��")
	{
		m_age = true;
		SetDlgItemText(IDC_MAN_AGEID, L"");
	}
}
