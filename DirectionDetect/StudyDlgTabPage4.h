#pragma once


// CStudyDlgTabPage4 对话框

class  CIni;

class CStudyDlgTabPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CStudyDlgTabPage4)

public:
	CStudyDlgTabPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStudyDlgTabPage4();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDY1_P4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CIni *m_pCini;
	CString m_szOld_pw;
	CString m_szNew_pw;
	//确认更改
	afx_msg void OnBnClickedButton2();
	//取消更改
	afx_msg void OnBnClickedButton1();
};
