#pragma once


// CStudyPWDlg �Ի���

class CStudyPWDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStudyPWDlg)

public:
	CString m_szPW;
	CString *m_pszPW_ret;



public:
	CStudyPWDlg(CString *psz, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStudyPWDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDY_PW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCancel();
};
