// StudyPWDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StudyPWDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CStudyPWDlg �Ի���

IMPLEMENT_DYNAMIC(CStudyPWDlg, CDialogEx)

CStudyPWDlg::CStudyPWDlg(CString *psz, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STUDY_PW, pParent)
	, m_szPW(_T(""))
{
	m_pszPW_ret = psz;
}

CStudyPWDlg::~CStudyPWDlg()
{
}

void CStudyPWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PW, m_szPW);
}


BEGIN_MESSAGE_MAP(CStudyPWDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CStudyPWDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStudyPWDlg ��Ϣ�������


void CStudyPWDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}


void CStudyPWDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(1);
	*m_pszPW_ret = m_szPW;
	CDialogEx::OnOK();
}


void CStudyPWDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
