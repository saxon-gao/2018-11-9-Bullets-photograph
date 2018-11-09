#include "stdafx.h"
#include "MfcHalcon.h"
#include "logger\StaticLogger.h"
#include "Resource.h"
extern CStaticLogger g_logger;
#define COLOR_CAMERA
using namespace HalconCpp;
void imageProcess(HObject ho_image, HObject *ho_regionOk, HObject *ho_regionNg)
{

	// Local iconic variables
	HObject  ho_Image1, ho_Image2, ho_Image3, ho_ImageSub;
	HObject  ho_Regions, ho_RegionClosing, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionTrans, ho__RegionDilation;
	HObject  ho_Regions2, ho_RegionOpening, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions1, ho_RegionClosing1, ho_RegionFillUp;
	HObject  ho_SelectedRegions2, ho__RegionTrans1, ho_ObjectsConcat;
	HObject  ho_RegionUnion, ho_ConnectedRegions2, ho_RegionTrans1;
	HObject  ho_ObjectSelected, ho_ImageReduced, ho_Regions1;

	// Local control variables
	HTuple  hv_Number, hv_Index, hv_Value;


	Decompose3(ho_image, &ho_Image1, &ho_Image2, &ho_Image3);
	SubImage(ho_Image1, ho_Image3, &ho_ImageSub, 3, 0);
	Threshold(ho_ImageSub, &ho_Regions, 117, 255);
	ClosingCircle(ho_Regions, &ho_RegionClosing, 5);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 4000, 11000);
	ShapeTrans(ho_SelectedRegions, &ho_RegionTrans, "outer_circle");
	DilationCircle(ho_RegionTrans, &ho__RegionDilation, 3.5);


	Threshold(ho_Image3, &ho_Regions2, 0, 48);
	OpeningCircle(ho_Regions2, &ho_RegionOpening, 2.5);
	Connection(ho_RegionOpening, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, "area", "and", 8500, 15630.8);
	ClosingCircle(ho_SelectedRegions1, &ho_RegionClosing1, 20);
	FillUp(ho_RegionClosing1, &ho_RegionFillUp);
	SelectShape(ho_RegionFillUp, &ho_SelectedRegions2, "circularity", "and", 0.6156,
		1);
	ShapeTrans(ho_SelectedRegions2, &ho__RegionTrans1, "inner_circle");

	ConcatObj(ho__RegionDilation, ho__RegionTrans1, &ho_ObjectsConcat);
	Union1(ho_ObjectsConcat, &ho_RegionUnion);
	Connection(ho_RegionUnion, &ho_ConnectedRegions2);


	ShapeTrans(ho_ConnectedRegions2, &ho_RegionTrans1, "inner_circle");

	CountObj(ho_RegionTrans1, &hv_Number);

	GenRegionPoints(&(*ho_regionOk), 1, 1);
	GenRegionPoints(&(*ho_regionNg), 1, 1);
	{
		HTuple end_val31 = hv_Number;
		HTuple step_val31 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val31, step_val31); hv_Index += step_val31)
		{
			SelectObj(ho_RegionTrans1, &ho_ObjectSelected, hv_Index);
			ReduceDomain(ho_Image1, ho_ObjectSelected, &ho_ImageReduced);

			Threshold(ho_ImageReduced, &ho_Regions1, 150, 249);
			RegionFeatures(ho_Regions1, "area", &hv_Value);


			if (0 != (hv_Value>1000))
			{
				Union2((*ho_regionOk), ho_ObjectSelected, &(*ho_regionOk));
			}
			else
			{
				Union2((*ho_regionNg), ho_ObjectSelected, &(*ho_regionNg));
			}
		}
	}
	return;
}



HTuple  ghv_AcqHandle;

BOOL halcon_test(void* p)
{
		HTuple newHWindowID(*((LONG64 *)p));
		//HTuple ImageWidth;
		//HTuple ImageHeight;
		//HObject hImage;
		//ReadImage(&hImage, "UserImages.bmp");
		//GetImagePointer1(hImage, NULL, NULL, &ImageWidth, &ImageHeight);
		//SetPart(newHWindowID, 0, 0, ImageHeight, ImageWidth);// 
		//DispObj(hImage, newHWindowID);
		SetFramegrabberParam(ghv_AcqHandle, "do_abort_grab", 0);
	return 0;
}


void abortGrab()
{
	SetFramegrabberParam(ghv_AcqHandle, "do_abort_grab", 0);
}



unsigned int __stdcall gWorkThreadFun(PVOID pM)
{
	//return 0;
	/*工作流程
	读取配资文件
	初始化相机
	检测是否退出循环
	获取PLC 工件到位信息
	光源接通
	延时
	拍照
	光源断开
	图像处理
	结果回传
	循环
	释放相机
	*/
	g_logger->Log(ILogger::LogLevel::LL_INFO, _T("拍照线程执行"));
	WorkThreadFunParameters *pWorkThreadFunParameters = (WorkThreadFunParameters *)pM;
	// Local control variables
	HTuple  hv_AcqHandle;
	HTuple Ht_err;
	int grab_timeout = -1;
#ifdef COLOR_CAMERA
	//Image Acquisition 01: Code generated by Image Acquisition 01
	//Image Acquisition 01: Attention: The initialization may fail in case parameters need to
	//Image Acquisition 01: be set in a specific order (e.g., image resolution vs. offset).
	try
	{
		OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "default", -1, "default", -1,
			"false", "default", "c42f90f13756_Hikvision_MVCL02041GC", 0, -1, &hv_AcqHandle);
	}
	catch (const HException& Ht_err)
	{
		g_logger->Log(ILogger::LogLevel::LL_ERROR, _T("OpenFramegrabber error！"));
		AfxMessageBox(_T("打开相机失败"));
		g_logger->Log(ILogger::LogLevel::LL_INFO, _T("拍照线程退出"));
		//通知主线程启用开始按钮
		PostMessage(pWorkThreadFunParameters->m_hMainWnd, ID_USER_MSG_THREAD_EXIT, NULL, NULL);
		return 0;
	}

	ghv_AcqHandle = hv_AcqHandle;

	//通知主线程启用停止按钮
	PostMessage(pWorkThreadFunParameters->m_hMainWnd, ID_USER_MSG_THREAD_initialed, NULL, NULL);

	
	SetFramegrabberParam(hv_AcqHandle, "Height", pWorkThreadFunParameters->m_iImageHeight);
	SetFramegrabberParam(hv_AcqHandle, "AcquisitionMode", "SingleFrame");
	SetFramegrabberParam(hv_AcqHandle, "AcquisitionLineRate", pWorkThreadFunParameters->m_iLineRate);
	SetFramegrabberParam(hv_AcqHandle, "ExposureTime", pWorkThreadFunParameters->m_iExposureTime);
	SetFramegrabberParam(hv_AcqHandle, "grab_timeout", grab_timeout);
	SetFramegrabberParam(hv_AcqHandle, "color_space", "rgb");
	//GrabImageStart(hv_AcqHandle, -1);
#else
	//Image Acquisition 01: Code generated by Image Acquisition 01
	//Image Acquisition 01: Attention: The initialization may fail in case parameters need to
	//Image Acquisition 01: be set in a specific order (e.g., image resolution vs. offset).
	try
	{
		OpenFramegrabber("GigEVision", 0, 0, 0, 0, 0, 0, "default", -1, "default", -1,
			"false", "default", "c42f90f1bc48_Hikvision_MVCL02040GM", 0, -1, &hv_AcqHandle);
	}
	catch (const HException& Ht_err)
	{
		g_logger->Log(ILogger::LogLevel::LL_ERROR, _T("OpenFramegrabber error！"));
		AfxMessageBox(_T("打开相机失败"));
		g_logger->Log(ILogger::LogLevel::LL_INFO, _T("拍照线程退出"));
		//通知主线程启用开始按钮
		PostMessage(pWorkThreadFunParameters->m_hMainWnd, ID_USER_MSG_THREAD_EXIT, NULL, NULL);
		return 0;
	}
	SetFramegrabberParam(hv_AcqHandle, "RegionSelector", "Region0");
	SetFramegrabberParam(hv_AcqHandle, "Height", pWorkThreadFunParameters->m_iImageHeight);
	SetFramegrabberParam(hv_AcqHandle, "AcquisitionMode", "Continuous");
	SetFramegrabberParam(hv_AcqHandle, "AcquisitionLineRate", pWorkThreadFunParameters->m_iLineRate);
	SetFramegrabberParam(hv_AcqHandle, "TriggerSelector", "FrameBurstStart");
	SetFramegrabberParam(hv_AcqHandle, "ExposureTime", pWorkThreadFunParameters->m_iExposureTime);
	SetFramegrabberParam(hv_AcqHandle, "grab_timeout", grab_timeout);
	//GrabImageStart(hv_AcqHandle, -1);
#endif // COLOR_CAMERA

	

	
	bool isNotSetPart = TRUE;
	HObject  ho_Image, ho_ImageRotate;
	int timeoutNum = 0;
	while (pWorkThreadFunParameters->m_lStopSignal != 1)
	{
		try
		{
			g_logger->Log(ILogger::LogLevel::LL_INFO, _T("1111111"));
			GrabImage(&ho_Image, hv_AcqHandle);
			g_logger->Log(ILogger::LogLevel::LL_INFO, _T("2222222"));
		}
		catch (HalconCpp::HException & Ht_err)
		{
			g_logger->Log(ILogger::LogLevel::LL_WARN, _T("GrabImage Image error"));
			CloseFramegrabber(hv_AcqHandle);
			g_logger->Log(ILogger::LogLevel::LL_INFO, _T("拍照线程退出"));
			//通知主线程启用开始按钮
			PostMessage(pWorkThreadFunParameters->m_hMainWnd, ID_USER_MSG_THREAD_EXIT, NULL, NULL);
			//AfxMessageBox(_T("GrabImage Image error"));
			return 0;
		}

		//保存上一次图片
		if (1 == pWorkThreadFunParameters->m_lIsSaveImage && !isNotSetPart)  //
		{
			MFC_HALCON::MH_WriteImage(pWorkThreadFunParameters->m_szPath, ho_ImageRotate);
			//AfxMessageBox(_T("保存一张图片"));
			InterlockedDecrement((LPLONG)&pWorkThreadFunParameters->m_lIsSaveImage);
		}

		RotateImage(ho_Image, &ho_ImageRotate, 90, "constant");
		if (isNotSetPart)//单次执行
		{
			HTuple w, h;
			GetImageSize(ho_ImageRotate, &w, &h);
			SetPart(pWorkThreadFunParameters->m_lHalconWindId, 0, 0, h, w);
			SetDraw(pWorkThreadFunParameters->m_lHalconWindId, "margin");
			SetLineWidth(pWorkThreadFunParameters->m_lHalconWindId, 2);

			isNotSetPart = FALSE;
			g_logger->Log(ILogger::LogLevel::LL_DEBUG, _T("SetPart"));
		}

		//HObject  ho_regionOk, ho_regionNg;
		
		//dev_update_off();
		//GenEmptyRegion(&ho_regionOk);
		//GenEmptyRegion(&ho_regionNg);
		//imageProcess(ho_ImageRotate, &ho_regionOk, &ho_regionNg);
		//dev_update_on();

		
			ClearWindow(pWorkThreadFunParameters->m_lHalconWindId);
			DispObj(ho_ImageRotate, pWorkThreadFunParameters->m_lHalconWindId);
			//SetColor(pWorkThreadFunParameters->m_lHalconWindId, "red");
			//DispObj(ho_regionNg, pWorkThreadFunParameters->m_lHalconWindId);
			//SetColor(pWorkThreadFunParameters->m_lHalconWindId, "green");
			//DispObj(ho_regionOk, pWorkThreadFunParameters->m_lHalconWindId);

			//测试用
			timeoutNum++;
			g_logger->Log(ILogger::LogLevel::LL_INFO, _T("抓取一张图像%d"), timeoutNum);
			
			char chsz[20] = {0};
			sprintf(chsz, "this is %d pic", timeoutNum);
			HTuple hstrrr(chsz);
			SetTposition(pWorkThreadFunParameters->m_lHalconWindId, 10, 10);
			WriteString(pWorkThreadFunParameters->m_lHalconWindId, hstrrr);
	}
	CloseFramegrabber(hv_AcqHandle);
	g_logger->Log(ILogger::LogLevel::LL_INFO, _T("拍照线程退出"));
	//通知主线程启用开始按钮
	PostMessage(pWorkThreadFunParameters->m_hMainWnd, ID_USER_MSG_THREAD_EXIT, NULL, NULL);
	return 0;
}


//----------------------------MFC_HALCON BEGIN-----------------------------------------------

LONG64 MFC_HALCON::MH_OpenWindow(HWND h_wFather ,CRect rect) {
	HTuple HFatherWindow;
	HTuple newHWindowID;
	HFatherWindow = (Hlong)h_wFather;
	OpenWindow(rect.top, rect.left, rect.Width(), rect.Height(), HFatherWindow, "visible", "", &newHWindowID);
	return newHWindowID.L();
}

BOOL MFC_HALCON::MH_ReadImage(CString szpath, HObject &Image) {
	//T2A
	char chPath[301];
	int n = szpath.GetLength(); //按字符计算，str的长度
	int len = WideCharToMultiByte(CP_ACP, 0, szpath, n, NULL, 0, NULL, NULL);//按Byte计算str长度
	if (len > 300)
		return -1;
	WideCharToMultiByte(CP_ACP, 0, szpath, n, chPath, len, NULL, NULL);//宽字节转换为多字节编码
	chPath[len] = '\0';//不要忽略末尾结束标志
	HTuple path(chPath);
	ReadImage(&Image, path);//
	return 0;
}

BOOL MFC_HALCON::MH_WriteImage(CString szpath, HObject &Image) {
	//T2A
	if (szpath.IsEmpty()) {
		AfxMessageBox(_T("图像保存路径不可用！"));
		return -1;
	}

	CTime tm = CTime::GetCurrentTime();
	CString szImageName;
	szImageName.Format(_T("\\%d_%d_%d_%d_%d_%d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	CString szPathAndName = szpath + szImageName + _T(".bmp");

	char chPath[301];
	int n = szPathAndName.GetLength(); //按字符计算，str的长度
	int len = WideCharToMultiByte(CP_ACP, 0, szPathAndName, n, NULL, 0, NULL, NULL);//按Byte计算str长度
	if (len > 300) {
		AfxMessageBox(_T("图像保存路径太长！"));
		return -2;
	}
	WideCharToMultiByte(CP_ACP, 0, szPathAndName, n, chPath, len, NULL, NULL);//宽字节转换为多字节编码
	chPath[len] = '\0';//不要忽略末尾结束标志
	HTuple path(chPath);
	WriteImage(Image, "bmp", 0, path);//
	return 0;
}

BOOL MFC_HALCON::MH_DispImage(HObject &Image, HTuple wndId, CRect position) {
	if (position.Width() == 0)
	{
		HTuple width, height;
		GetImageSize(Image, &width, &height);
		SetPart(wndId, 0, 0, height, width);
	}
	else
	{
		SetPart(wndId, 0, 0, 50, 200);
	}
	DispObj(Image, wndId);
	return 0;
}
//----------------------------MFC_HALCON OVER-----------------------------------------------