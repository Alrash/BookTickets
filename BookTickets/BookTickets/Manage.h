#pragma once
#include "afxcmn.h"

#include <regex>


const std::regex compare("^\\d{15}$|^\\d{18}$");

// CManage �Ի���

class CManage : public CDialogEx
{
	DECLARE_DYNAMIC(CManage)

public:
	CManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CManage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CList<CRect, CRect> listRect;
	afx_msg void OnBnClickedManTicket();
	afx_msg void OnBnClickedManPay();
	afx_msg void OnBnClickedManGiv();
	afx_msg void OnBnClickedManCom();
	afx_msg void OnBnClickedManAge();
	afx_msg void OnEnChangeManId();
	afx_msg void OnEnChangeManTid();
	afx_msg void OnEnChangeManComid();
	afx_msg void OnEnChangeManAgeid();
private:
	CListCtrl m_list;
	bool m_tid, m_id, m_age, m_com;
};
