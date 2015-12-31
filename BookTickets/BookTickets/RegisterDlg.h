#pragma once
#include "afxwin.h"

#include <regex>

const std::regex pmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
const std::regex pid("^\\d{15}$|^\\d{18}$|^\\d{17}(\\d|X|x)$");
const std::regex ppasswd("^[a-zA-Z0-9]\\w{5,}$");

// CRegisterDlg 对话框

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_sex;
	CComboBox m_idname;
	afx_msg void OnBnClickedRegReg();

private:
	CString m_email;
	CString m_name;
	CString m_nick;
	CString m_key;
	CString m_passwd;
	CString m_id;
	CString m_company;
	CString m_sexi;
public:
	afx_msg void OnEnChangeRegEml();
	afx_msg void OnEnChangeRegName();
	afx_msg void OnEnChangeRegNick();
	afx_msg void OnEnChangeRegPas();
	afx_msg void OnEnChangeRegAga();
	afx_msg void OnEnChangeRegIdt();
	afx_msg void OnEnChangeRegCom();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
