// PassWD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BookTickets.h"
#include "PassWD.h"
#include "afxdialogex.h"

#include "RegisterDlg.h"
#include "ClientSocket.h"
#include "Deal.h"

Info info;


// CPassWD �Ի���

IMPLEMENT_DYNAMIC(CPassWD, CDialogEx)

CPassWD::CPassWD(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PASSWD, pParent)
{

}

CPassWD::~CPassWD()
{
}

void CPassWD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPassWD, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDCANCEL, &CPassWD::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPassWD::OnBnClickedOk)
	ON_EN_CHANGE(IDC_PWD_ID, &CPassWD::OnChangePwdId)
	ON_EN_CHANGE(IDC_PWD_PWD, &CPassWD::OnChangePwdPwd)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_PWD_REG, &CPassWD::OnStnClickedPwdReg)
END_MESSAGE_MAP()


// CPassWD ��Ϣ�������


void CPassWD::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}


void CPassWD::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
	ExitProcess(0);			//�˳�����
}


void CPassWD::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	info.id = "00000000000000";
	info.user = 'e';
	CDialogEx::OnOK();

	ClientSocket *client = new ClientSocket;
	Deal *deal = new Deal;
	string str;
	char s[1000];

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_id, -1, NULL, NULL, 0, NULL);
	char *id = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_id, -1, id, dwNum, 0, NULL);

	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_passwd, -1, NULL, NULL, 0, NULL);
	char *passwd = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_passwd, -1, passwd, dwNum, 0, NULL);

	sprintf(s, "1;s;select Id, Password from Passinfo where Nickname = '%s' or Email = '%s';", id, id);
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
		MessageBox(L"�������Ӵ���,��������", L"ERROR", MB_OK | MB_ICONWARNING);

		delete[] id;
		delete[] passwd;
		delete client;
		delete deal;

		return;
	}

	if (str == "Fail" || str == "" || str.substr(str.find("|") + 1, str.find(";") - str.find("|") - 1) != passwd)
		GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_SHOW);
	else
	{
		info.id = str.substr(0, str.find("|"));
		if (info.id > "15000000000000")
			info.user = 'u';
		else
			if (info.id > "10000000000000")
				info.user = 'c';
			else
				if (info.id > "500000000000")
					info.user = 'e';
				else
					info.user = 'a';

		CDialogEx::OnOK();
	}
}


void CPassWD::OnChangePwdId()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_PWD_ID);
	pBox->GetWindowTextW(m_id);
}


void CPassWD::OnChangePwdPwd()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_PWD_PWD);
	pBox->GetWindowTextW(m_passwd);
}


HBRUSH CPassWD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (IDC_PWD_ERR == pWnd->GetDlgCtrlID())          //�ж����ڻ��Ƶ��ǲ���������Ҫ�ı�Ŀؼ�
	{
		pDC->SetBkMode(TRANSPARENT);                   //����͸�����
		pDC->SetTextColor(RGB(255, 0, 0));             //����������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //���Զ���Ļ�ˢȥ���Ʊ���
	}

	if (IDC_PWD_REG == pWnd->GetDlgCtrlID())          //�ж����ڻ��Ƶ��ǲ���������Ҫ�ı�Ŀؼ�
	{
		pDC->SetBkMode(TRANSPARENT);                   //����͸�����
		pDC->SetTextColor(RGB(79, 148, 205));         //����������ɫ
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //���Զ���Ļ�ˢȥ���Ʊ���
	}

	return hbr;
}


void CPassWD::OnStnClickedPwdReg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CRegisterDlg *reg = new CRegisterDlg();

	reg->DoModal();
	reg->DestroyWindow();

	delete reg;
}


BOOL CPassWD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
