#pragma once


// CStudyDlgTabPage4 �Ի���

class  CIni;

class CStudyDlgTabPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CStudyDlgTabPage4)

public:
	CStudyDlgTabPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStudyDlgTabPage4();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDY1_P4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CIni *m_pCini;
	CString m_szOld_pw;
	CString m_szNew_pw;
	//ȷ�ϸ���
	afx_msg void OnBnClickedButton2();
	//ȡ������
	afx_msg void OnBnClickedButton1();
};
