// StartUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StartUpDlg.h"
#include "afxdialogex.h"
#include "resource.h" 

// CStartUpDlg �Ի���

IMPLEMENT_DYNAMIC(CStartUpDlg, CDialogEx)

CStartUpDlg::CStartUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STARTUP, pParent)
{
	
}

CStartUpDlg::~CStartUpDlg()
{
}

void CStartUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartUpDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStartUpDlg ��Ϣ�������


void CStartUpDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}


void CStartUpDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(m_lSTRTimer);
	OnOK();
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CStartUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lSTRTimer = SetTimer( 1, 1500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
