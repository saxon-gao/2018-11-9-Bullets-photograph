#pragma once
//3.����halcon12
//(1)��Ŀ - ���� - �������� - ������ - ���� - ����������������£�
//halconcpp.lib
//(2)��Ŀ - ���� - �������� - VC++Ŀ¼ - ����Ŀ¼������£�
//D : \software\halcon12\include;
//D:\software\halcon12\include\halconcpp;
//(3)��Ŀ - ���� - �������� - VC++Ŀ¼ - ��Ŀ¼������£�
//D : \software\halcon12\lib\x64 - win64
//	(4)��Ŀ - ���� - �������� - ���ù�����������Ŀƽ̨��Ϊx64��
//	(5)Դ�ļ���������£�
//
//#include "Halconcpp.h"
//	using namespace HalconCpp;


//�칫�ҵ���halcon·��  D:\program\halcon
//���˵���halcon·��		F:\program files\HALCON


#include "Halconcpp.h"
using namespace HalconCpp;


BOOL halcon_test(void *p = NULL);								//ȫ�ֲ��Ժ���


struct WorkThreadFunParameters {
	HWND m_hMainWnd = 0;
	volatile long m_lStopSignal = 0;//ֹͣ���   1ʱͼ�����߳�ֹͣѭ��
	volatile long m_lIsSaveImage = 0;//�Ƿ񱣴�ͼƬ���  1ʱ����ͼƬ
	CString m_szPath;
	//HANDLE m_hIsSaveIamge=0;//���߳� �������̱߳���ͼƬ���� ͬ���ź�
	//int m_iDelayTime = 0;//�ӳ�ʱ��ms���������ʾ������� halcon ץȡ��ʱ��ͼ�񱨴� Ҳ����try ���

	LONG64 m_lHalconWindId = 0;//halcon ����ID
	int m_iImageHeight = 0;//���ͼ��߶ȣ�����ͼƬ��ȡͼ��������
	int m_iLineRate = 0;//ÿ������
	int m_iExposureTime = 0;//�ع�ʱ��ms

};


//��⼷�۷���
unsigned int __stdcall gWorkThreadFun(PVOID pM);
//�ж����գ���grabImage()����ʱ�ж������˳�grab��try  catch���Բ���
void abortGrab();


namespace MFC_HALCON
{
//��һ��halcon����   �����������ڣ��ڸ������е����ϡ�����λ��
	LONG64 MH_OpenWindow(HWND h_wFather, CRect rect);	

//��ȡָ��·��ͼ��
	BOOL MH_ReadImage(CString szpath, HObject &Image);

//����ͼ��ָ��·��
	BOOL MH_WriteImage(CString szpath, HObject &Image);

//��ʾHObjectͼ��
	BOOL MH_DispImage(HObject &Image, HTuple wndId, CRect position =0);
};