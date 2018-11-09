#pragma once
#include "afxwin.h"


// CStudyDlgTabPage0 �Ի���

class CStudyDlgTabPage0 : public CDialogEx
{
	DECLARE_DYNAMIC(CStudyDlgTabPage0)

public:
	CStudyDlgTabPage0(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStudyDlgTabPage0();

	void updateMember(void*p);

public:
	//�ɼ�ģʽ ��֡������
	CComboBox m_CameraAcquisitionMode;
	// ͼ����
	int m_iImgeWidth;
	// ��Ƶ
	int m_iLineRate;
	//�ع�ʱ��
	int m_iExposureTime;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDY1_P0 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
