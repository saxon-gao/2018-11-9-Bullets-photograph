
// DirectionDetectView.cpp : CDirectionDetectView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DirectionDetect.h"
#endif

#include "DirectionDetectDoc.h"
#include "DirectionDetectView.h"
#include "StudyDlg1.h"
#include "MfcHalcon.h"
#include "logger\StaticLogger.h"
#include "MainFrm.h"
#include "Ini.h"
#include "StudyPWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern CStaticLogger g_logger;
IMPLEMENT_DYNCREATE(CDirectionDetectView, CView)

BEGIN_MESSAGE_MAP(CDirectionDetectView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CMD_START, &CDirectionDetectView::OnCmdStart)
	ON_COMMAND(ID_CMD_STOP, &CDirectionDetectView::OnCmdStop)
	ON_MESSAGE(ID_USER_MSG_THREAD_EXIT, &CDirectionDetectView::OnEnableStartButtom)
	ON_MESSAGE(ID_USER_MSG_THREAD_initialed, &CDirectionDetectView::OnEnableStopButtom)
	ON_COMMAND(ID_EDIT_STUDY, &CDirectionDetectView::OnEditStudy)
	ON_WM_TIMER()
	ON_WM_SIZE()
	//ON_UPDATE_COMMAND_UI(ID_CMD_START, &CDirectionDetectView::OnUpdateCmdStart)
	//ON_UPDATE_COMMAND_UI(ID_CMD_STOP, &CDirectionDetectView::OnUpdateCmdStop)
	//ON_UPDATE_COMMAND_UI(ID_EDIT_STUDY, &CDirectionDetectView::OnUpdateEditStudy)
	//ON_UPDATE_COMMAND_UI(ID_APP_EXIT, &CDirectionDetectView::OnUpdateAppExit)
	ON_COMMAND(ID_FILE_OPEN, &CDirectionDetectView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDirectionDetectView::OnFileSave)
END_MESSAGE_MAP()

// CDirectionDetectView ����/����

CDirectionDetectView::CDirectionDetectView()
{
	// TODO: �ڴ˴���ӹ������
	m_hMainViewWindowID = 0;

	m_pIni = new CIni(_T(".\\bulletPhotograp.ini"));
	//int a = m_pIni->WriteInt(_T("Camera Configuration"), _T("ImageHeight"),1);
	//a = m_pIni->WriteInt(_T("Camera Configuration"), _T("LineRate"), 2);
	//a = m_pIni->WriteInt(_T("Camera Configuration"), _T("ExposureTime"), 3);
}

CDirectionDetectView::~CDirectionDetectView()
{
}

BOOL CDirectionDetectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	
	return CView::PreCreateWindow(cs);
}

// CDirectionDetectView ����

void CDirectionDetectView::OnDraw(CDC* /*pDC*/)
{
	CDirectionDetectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CDirectionDetectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDirectionDetectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDirectionDetectView ���

#ifdef _DEBUG
void CDirectionDetectView::AssertValid() const
{
	CView::AssertValid();
}

void CDirectionDetectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDirectionDetectDoc* CDirectionDetectView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDirectionDetectDoc)));
	return (CDirectionDetectDoc*)m_pDocument;
}
#endif //_DEBUG



void CDirectionDetectView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ʼ��סview����  halcon����
	setHaloconWindRect();
	m_hMainViewWindowID = MFC_HALCON::MH_OpenWindow(m_hWnd, m_rect);

	KillTimer(m_lTimer);
	CView::OnTimer(nIDEvent);
}


void CDirectionDetectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	//����halcon���ڴ�С
	if (0 != m_hMainViewWindowID)
	{
		//HalconCpp::CloseWindow(m_hMainViewWindowID);
		//m_hMainViewWindowID = MFC_HALCON::MH_OpenWindow(m_hWnd, m_rect);
		setHaloconWindRect();
		HalconCpp::SetWindowExtents(m_hMainViewWindowID, m_rect.top, m_rect.left, m_rect.Width(), m_rect.Height());
	}
}


void CDirectionDetectView::setHaloconWindRect()
{
	GetClientRect(m_rect);
	int topDistance = 7;//halcon���ھ���ͻ����ϱ߾�
	int imageWHScale = 4080/2048;//ͼ���߱�
	m_rect.top += topDistance;
	m_rect.left += 7;
	m_rect.right = (long)m_rect.right -7;
	//m_rect.bottom = m_rect.bottom-7;
	int bottom = topDistance + m_rect.Width() / imageWHScale;
	if (m_rect.bottom < bottom)
	{
		m_rect.bottom = m_rect.bottom;
	}
	else
	{
		m_rect.bottom = bottom;
	}
}


// CDirectionDetectView ��Ϣ�������
WorkThreadFunParameters g_WorkThreadFunParameter;

void CDirectionDetectView::OnCmdStart()
{
	/*
	�ؼ���������
	��ʼ��
	��ȡ�����ļ�
	���������߳�
	*/
	//((CMainFrame*)::AfxGetMainWnd())->enableMenu(1, ID_CMD_STOP);
	((CMainFrame*)::AfxGetMainWnd())->disableMenu(4, ID_CMD_START, ID_APP_EXIT, ID_FILE_OPEN, ID_EDIT_STUDY);
	
	//g_WorkThreadFunParameter.m_hIsSaveIamge = CreateEventW(NULL, FALSE, FALSE, NULL);
	g_WorkThreadFunParameter.m_hMainWnd = m_hWnd;
	g_WorkThreadFunParameter.m_lStopSignal = 0;
	g_WorkThreadFunParameter.m_lIsSaveImage = 0;
	g_WorkThreadFunParameter.m_szPath.Empty();
	g_WorkThreadFunParameter.m_szPath = m_pIni->GetString(_T("ImageProcess Configuration"), _T("ImageSavaPath"));
	g_WorkThreadFunParameter.m_iImageHeight = m_pIni->GetInt(_T("Camera Configuration"), _T("ImageHeight"));
	g_WorkThreadFunParameter.m_iLineRate = m_pIni->GetInt(_T("Camera Configuration"), _T("LineRate"));
	g_WorkThreadFunParameter.m_iExposureTime = m_pIni->GetInt(_T("Camera Configuration"), _T("ExposureTime"));
	//int delayTime = g_WorkThreadFunParameter.m_iImageHeight - 2000;
	//g_WorkThreadFunParameter.m_iDelayTime = delayTime > 0 ? delayTime : 0;
	
	g_WorkThreadFunParameter.m_lHalconWindId = m_hMainViewWindowID;
	m_handle = (HANDLE)_beginthreadex(NULL, 0, gWorkThreadFun, &g_WorkThreadFunParameter, 0, NULL);
	return;
}


void CDirectionDetectView::OnCmdStop()
{
	if(0==g_WorkThreadFunParameter.m_lStopSignal)
		InterlockedIncrement((LPLONG)&g_WorkThreadFunParameter.m_lStopSignal);
	//if (WAIT_OBJECT_0 != WaitForSingleObject(m_handle, INFINITE))
	//{
	//	AfxMessageBox(_T("�����߳��˳�����"));
	//	return;
	//}//�˷����������̣߳������߳̽������ʱ������Դ������halcon����

	//�ж�grab
	abortGrab();
	
	//((CMainFrame*)::AfxGetMainWnd())->enableMenu(4, ID_CMD_START, ID_APP_EXIT, ID_FILE_OPEN, ID_EDIT_STUDY);
	((CMainFrame*)::AfxGetMainWnd())->disableMenu(1, ID_CMD_STOP);
}

LRESULT  CDirectionDetectView::OnEnableStartButtom(WPARAM wParam, LPARAM lParam)
{
	((CMainFrame*)::AfxGetMainWnd())->enableMenu(4, ID_CMD_START, ID_APP_EXIT, ID_FILE_OPEN, ID_EDIT_STUDY);
	((CMainFrame*)::AfxGetMainWnd())->disableMenu(1, ID_CMD_STOP);
	return 0;
}
LRESULT  CDirectionDetectView::OnEnableStopButtom(WPARAM wParam, LPARAM lParam)
{
	((CMainFrame*)::AfxGetMainWnd())->enableMenu(1, ID_CMD_STOP);
	return 0;
}

void CDirectionDetectView::OnEditStudy()
{
	// TODO: �ڴ���������������
	CString szpw = _T("");
	
	CStudyPWDlg studyPWDlg(&szpw);
	INT_PTR ret = studyPWDlg.DoModal();
	if (1 == ret)
	{
		if (m_pIni->GetString(_T("Studypw"), _T("szStudypw"))== szpw)
		{
			CStudyDlg1 studyDlg(m_pIni);
			studyDlg.DoModal();
		}
		else
		{
			AfxMessageBox(_T("������ȷ����"));
		}
	}
}




void CDirectionDetectView::OnFileOpen()
{
	// TODO: �ڴ���������������
	CString m_path;
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		TEXT("֧�ֵ�ͼ���ļ� (*.jpg;*.gif;*.bmp;...)|*.jpg;*.gif;*.bmp|ר��Tiffͼ���ļ�(*.tiff;*.tif)|*.tiff;*.tif|�����ļ�(*.*)|*.*||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��ͼ���ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() == IDOK)
	{
		m_path = hFileDlg.GetPathName();
		HObject Image;
		CRect rect;
		MFC_HALCON::MH_ReadImage(m_path, Image);
		MFC_HALCON::MH_DispImage(Image, m_hMainViewWindowID);
	}
}


void CDirectionDetectView::OnFileSave()
{
	if (0 == g_WorkThreadFunParameter.m_lIsSaveImage)
		InterlockedIncrement((LPLONG)&g_WorkThreadFunParameter.m_lIsSaveImage);
}


