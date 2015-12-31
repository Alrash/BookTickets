
// BookTicketsDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "Book.h"
#include "Info.h"
#include "Manage.h"


// CBookTicketsDlg 对话框
class CBookTicketsDlg : public CDialogEx
{
// 构造
public:
	CBookTicketsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKTICKETS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CList<CRect, CRect> listRect;
	afx_msg void OnBnClickedCancel();
private:
	CTabCtrl m_tab;
	CRect m_rect;

	CBook m_book;
	CInfo m_info;
	CManage m_manage;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
