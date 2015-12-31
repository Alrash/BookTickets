// PassWD.cpp : 实现文件
//

#include "stdafx.h"
#include "BookTickets.h"
#include "PassWD.h"
#include "afxdialogex.h"

#include "RegisterDlg.h"
#include "ClientSocket.h"
#include "Deal.h"

Info info;


// CPassWD 对话框

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


// CPassWD 消息处理程序


void CPassWD::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}


void CPassWD::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
	ExitProcess(0);			//退出程序
}


void CPassWD::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(L"网络连接错误,请检查网络", L"ERROR", MB_OK | MB_ICONWARNING);

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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_PWD_ID);
	pBox->GetWindowTextW(m_id);
}


void CPassWD::OnChangePwdPwd()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	CEdit *pBox = (CEdit *)GetDlgItem(IDC_PWD_PWD);
	pBox->GetWindowTextW(m_passwd);
}


HBRUSH CPassWD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (IDC_PWD_ERR == pWnd->GetDlgCtrlID())          //判断正在绘制的是不是我们需要改变的控件
	{
		pDC->SetBkMode(TRANSPARENT);                   //设置透明输出
		pDC->SetTextColor(RGB(255, 0, 0));             //设置文字颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //用自定义的画刷去绘制背景
	}

	if (IDC_PWD_REG == pWnd->GetDlgCtrlID())          //判断正在绘制的是不是我们需要改变的控件
	{
		pDC->SetBkMode(TRANSPARENT);                   //设置透明输出
		pDC->SetTextColor(RGB(79, 148, 205));         //设置文字颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);     //用自定义的画刷去绘制背景
	}

	return hbr;
}


void CPassWD::OnStnClickedPwdReg()
{
	// TODO: 在此添加控件通知处理程序代码

	CRegisterDlg *reg = new CRegisterDlg();

	reg->DoModal();
	reg->DestroyWindow();

	delete reg;
}


BOOL CPassWD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetDlgItem(IDC_PWD_ERR)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
