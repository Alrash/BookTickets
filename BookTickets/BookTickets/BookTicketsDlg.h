
// BookTicketsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "Book.h"
#include "Info.h"
#include "Manage.h"


// CBookTicketsDlg �Ի���
class CBookTicketsDlg : public CDialogEx
{
// ����
public:
	CBookTicketsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKTICKETS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
