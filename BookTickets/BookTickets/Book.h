#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CBook 对话框

class CBook : public CDialogEx
{
	DECLARE_DYNAMIC(CBook)

public:
	CBook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBook();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CList<CRect, CRect> listRect;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
private:
	CListCtrl m_list;
	CString m_set;
	CString m_off;
public:
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBookFind();
private:
	CComboBox m_year;
	CComboBox m_mon;
	CComboBox m_day;
public:
//	afx_msg void OnEnChangeBookSet();
//	afx_msg void OnEnChangeBookOff();
private:
	CComboBox m_setoff;
	CComboBox m_arrive;
	int tickets;
	int row;
public:
	afx_msg void OnCbnSelchangeBookMon();
	afx_msg void OnBnClickedBookBook();
};
