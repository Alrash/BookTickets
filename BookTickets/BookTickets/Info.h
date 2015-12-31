#pragma once

#include <regex>
#include "afxcmn.h"

const std::regex ppasswd("^[a-zA-Z0-9]\\w{5,}$");

// CInfo 对话框

class CInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo)

public:
	CInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CList<CRect, CRect> listRect;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedInfoCha();
	afx_msg void OnEnChangeInfoKey();
	afx_msg void OnEnChangeInfoAga();

private:
	bool m_key, m_again;
	CString m_nick, m_mail, m_com, m_remark;
	CString m_num;
	bool reflash;
public:
	afx_msg void OnBnClickedInfoKcom();
	afx_msg void OnBnClickedInfoChag();
	afx_msg void OnBnClickedInfoFin();
	afx_msg void OnBnClickedInfoCal();
private:
	CListCtrl m_list;
public:
	afx_msg void OnBnClickedInfoFindall();
	afx_msg void OnBnClickedInfoFind();
	afx_msg void OnBnClickedInfoDel();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
};
