#pragma once


// CStartUpDlg �Ի���

class CStartUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartUpDlg)

public:
	CStartUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartUpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTUP };
#endif
public:
	UINT_PTR m_lSTRTimer;//������ʾʱ���ʱ��ID��OnInitDialog�ڸ�ʱ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
