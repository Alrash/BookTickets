#pragma once

// CPassWD �Ի���

class CPassWD : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWD)

public:
	CPassWD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassWD();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangePwdId();
	afx_msg void OnChangePwdPwd();
private:
	CStringW m_id;
	CStringW m_passwd;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedPwdReg();
	virtual BOOL OnInitDialog();
};
