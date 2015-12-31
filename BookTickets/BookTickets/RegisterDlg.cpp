// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BookTickets.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"

#include <regex>
#include "ClientSocket.h"
#include "Deal.h"

using std::regex;
using std::regex_match;

// CRegisterDlg �Ի���

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REG, pParent)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_sex);
	DDX_Control(pDX, IDC_REG_ID, m_idname);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDB_REG_REG, &CRegisterDlg::OnBnClickedRegReg)
	ON_EN_CHANGE(IDC_REG_EML, &CRegisterDlg::OnEnChangeRegEml)
	ON_EN_CHANGE(IDC_REG_NAME, &CRegisterDlg::OnEnChangeRegName)
	ON_EN_CHANGE(IDC_REG_NICK, &CRegisterDlg::OnEnChangeRegNick)
	ON_EN_CHANGE(IDC_REG_PAS, &CRegisterDlg::OnEnChangeRegPas)
	ON_EN_CHANGE(IDC_REG_AGA, &CRegisterDlg::OnEnChangeRegAga)
	ON_EN_CHANGE(IDC_REG_IDT, &CRegisterDlg::OnEnChangeRegIdt)
	ON_EN_CHANGE(IDC_REG_COM, &CRegisterDlg::OnEnChangeRegCom)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRegisterDlg ��Ϣ�������


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_sex.AddString(L"��");
	m_sex.AddString(L"Ů");
	//m_sex.SetCurSel(0);

	m_idname.AddString(L"���֤");
	m_idname.AddString(L"����");
	m_idname.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CRegisterDlg::OnBnClickedRegReg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	bool flag = true;
	GetDlgItem(IDC_COMBO2)->GetWindowTextW(m_sexi); //IDC_COMBO2->sex

	//emailƥ�� Ϊ ***(.)***@***.c**
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_email, -1, NULL, NULL, 0, NULL);
	char *email = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_email, -1, email, dwNum, 0, NULL);
	if (!regex_match(email, pmail))
	{
		//������
		GetDlgItem(IDC_REG_EMAIL)->ShowWindow(SW_SHOW);
		flag = false;
	}

	//15λ��18λ���֤��
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_id, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_id, -1, id, dwNum, 0, NULL);
	if (!regex_match(id, pid))
	{
		GetDlgItem(IDC_REG_IDERR)->ShowWindow(SW_SHOW);
		//MessageBox(L"dsb");
		flag = false;
	}

	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_key, -1, NULL, NULL, 0, NULL);
	char *passwd = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_key, -1, passwd, dwNum, 0, NULL);
	if (!regex_match(passwd, ppasswd))
	{
		GetDlgItem(IDC_REG_KEYERR)->ShowWindow(SW_SHOW);
		flag = false;
	}

	if (m_key != m_passwd)
	{
		GetDlgItem(IDC_REG_DIF)->ShowWindow(SW_SHOW);
		flag = false;
	}

	while (m_name.Replace(L" ", L""));	//��ȥ�ո�
	while (m_nick.Replace(L" ", L""));

	if (m_name == "" || m_nick == ""/* || m_sexi == ""*/)
	{
		GetDlgItem(IDC_REG_NULL)->ShowWindow(SW_SHOW);
		flag = false;
	}

	if (flag == true)
	{
		ClientSocket *client = new ClientSocket;
		Deal *deal = new Deal;
		string str, which, sex;
		char s[1000];

		GetDlgItemText(IDC_REG_ID, m_id);
		GetDlgItemText(IDC_COMBO2, m_sexi);

		if (m_id == "���֤")
			which = "0";
		else
			which = "1";

		if (m_sexi == "��")
			sex = "1";
		else
			sex = "0";

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_name, -1, NULL, NULL, 0, NULL);
		char *name = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, m_name, -1, name, dwNum, 0, NULL);

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_nick, -1, NULL, NULL, 0, NULL);
		char *nick = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, m_nick, -1, nick, dwNum, 0, NULL);

		dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_company, -1, NULL, NULL, 0, NULL);
		char *com = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, m_company, -1, com, dwNum, 0, NULL);

		sprintf(s, "3;r;;i;insert into Passinfo values('&r', '%s', '%s', '%s', '');i;insert into Info values('&r', '%s', %s, '%s', %s, '%s');",
					nick, passwd, email, name, sex.c_str(), id, which.c_str(), com);
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
			MessageBox(L"�������Ӵ���ע��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

			delete id;
			delete deal;
			delete client;

			return;
		}

		if (str == "Fail" ||  str == "")
		{
			MessageBox(L"ע��ʧ��", L"ERROR!", MB_OK | MB_ICONWARNING);

			delete id;
			delete deal;
			delete client;

			return;
		}

		MessageBox(L"ע��ɹ�", L"Option", MB_OK | MB_ICONASTERISK);

		delete deal;
		delete client;
		CDialogEx::OnOK();
	}

	delete email;
	delete id;
	delete passwd;
}


void CRegisterDlg::OnEnChangeRegEml()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_EMAIL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_EML);
	pBox->GetWindowTextW(m_email);
}


void CRegisterDlg::OnEnChangeRegName()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_NULL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_NAME);
	pBox->GetWindowTextW(m_name);
}


void CRegisterDlg::OnEnChangeRegNick()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_NULL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_NICK);
	pBox->GetWindowTextW(m_nick);
}


void CRegisterDlg::OnEnChangeRegPas()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_KEYERR)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_PAS);
	pBox->GetWindowTextW(m_key);
}


void CRegisterDlg::OnEnChangeRegAga()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_DIF)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_AGA);
	pBox->GetWindowTextW(m_passwd);
}


void CRegisterDlg::OnEnChangeRegIdt()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_REG_IDERR)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_IDT);
	pBox->GetWindowTextW(m_id);
}


void CRegisterDlg::OnEnChangeRegCom()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_COM);
	pBox->GetWindowTextW(m_company);
}


HBRUSH CRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	if (IDC_REG_FIX == pWnd->GetDlgCtrlID() || IDC_REG_EMAIL == pWnd->GetDlgCtrlID()
		|| IDC_REG_NULL == pWnd->GetDlgCtrlID() || IDC_REG_DIF == pWnd->GetDlgCtrlID() 
		|| IDC_REG_IDERR == pWnd->GetDlgCtrlID() || IDC_REG_KEYERR == pWnd->GetDlgCtrlID())          //�ж����ڻ��Ƶ��ǲ���������Ҫ�ı�Ŀؼ�
	{
		pDC->SetBkMode(TRANSPARENT);                   //����͸�����
		pDC->SetTextColor(RGB(255, 0, 0));             //����������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //���Զ���Ļ�ˢȥ���Ʊ���
	}

	return hbr;
}
