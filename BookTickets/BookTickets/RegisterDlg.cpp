// RegisterDlg.cpp : 实现文件
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

// CRegisterDlg 对话框

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


// CRegisterDlg 消息处理程序


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_sex.AddString(L"男");
	m_sex.AddString(L"女");
	//m_sex.SetCurSel(0);

	m_idname.AddString(L"身份证");
	m_idname.AddString(L"护照");
	m_idname.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegisterDlg::OnBnClickedRegReg()
{
	// TODO: 在此添加控件通知处理程序代码

	bool flag = true;
	GetDlgItem(IDC_COMBO2)->GetWindowTextW(m_sexi); //IDC_COMBO2->sex

	//email匹配 为 ***(.)***@***.c**
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_email, -1, NULL, NULL, 0, NULL);
	char *email = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, m_email, -1, email, dwNum, 0, NULL);
	if (!regex_match(email, pmail))
	{
		//不符合
		GetDlgItem(IDC_REG_EMAIL)->ShowWindow(SW_SHOW);
		flag = false;
	}

	//15位或18位身份证号
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

	while (m_name.Replace(L" ", L""));	//除去空格
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

		if (m_id == "身份证")
			which = "0";
		else
			which = "1";

		if (m_sexi == "男")
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
			MessageBox(L"网络连接错误，注册失败", L"ERROR!", MB_OK | MB_ICONWARNING);

			delete id;
			delete deal;
			delete client;

			return;
		}

		if (str == "Fail" ||  str == "")
		{
			MessageBox(L"注册失败", L"ERROR!", MB_OK | MB_ICONWARNING);

			delete id;
			delete deal;
			delete client;

			return;
		}

		MessageBox(L"注册成功", L"Option", MB_OK | MB_ICONASTERISK);

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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_EMAIL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_EML);
	pBox->GetWindowTextW(m_email);
}


void CRegisterDlg::OnEnChangeRegName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_NULL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_NAME);
	pBox->GetWindowTextW(m_name);
}


void CRegisterDlg::OnEnChangeRegNick()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_NULL)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_NICK);
	pBox->GetWindowTextW(m_nick);
}


void CRegisterDlg::OnEnChangeRegPas()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_KEYERR)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_PAS);
	pBox->GetWindowTextW(m_key);
}


void CRegisterDlg::OnEnChangeRegAga()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_DIF)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_AGA);
	pBox->GetWindowTextW(m_passwd);
}


void CRegisterDlg::OnEnChangeRegIdt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_REG_IDERR)->ShowWindow(SW_HIDE);
	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_IDT);
	pBox->GetWindowTextW(m_id);
}


void CRegisterDlg::OnEnChangeRegCom()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_REG_COM);
	pBox->GetWindowTextW(m_company);
}


HBRUSH CRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (IDC_REG_FIX == pWnd->GetDlgCtrlID() || IDC_REG_EMAIL == pWnd->GetDlgCtrlID()
		|| IDC_REG_NULL == pWnd->GetDlgCtrlID() || IDC_REG_DIF == pWnd->GetDlgCtrlID() 
		|| IDC_REG_IDERR == pWnd->GetDlgCtrlID() || IDC_REG_KEYERR == pWnd->GetDlgCtrlID())          //判断正在绘制的是不是我们需要改变的控件
	{
		pDC->SetBkMode(TRANSPARENT);                   //设置透明输出
		pDC->SetTextColor(RGB(255, 0, 0));             //设置文字颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //用自定义的画刷去绘制背景
	}

	return hbr;
}
