#pragma once
#include "structures.h"
#include "SampleGrabber.h"

class CCaptureVideo
{
public:
	CCaptureVideo(device_param_info* pCaptureDevice, camera_frame_format_info* pFrameInfo);
	~CCaptureVideo();

public:
	CComPtr <IGraphBuilder> pGraphBuilder;
	CComPtr <ICaptureGraphBuilder2> pCaptureGraphBuilder2;
	CComPtr <ISampleGrabber> pSampleGrabber;
	CComPtr <IBaseFilter> pSGrabber;
	CComPtr <IBaseFilter> pColorSC;
	CComPtr <IBaseFilter> pDecompressor;
	CComPtr <IBaseFilter> pRenderer;
	bool onExit = false;
	void InitGraph();
	void AddElementToFilter();
	// Проверка на ошибку
	bool CheckHRError(HRESULT hr);
private:
	struct device_param_info* pDevParam;
	struct camera_frame_format_info* pFrameFormat;
	bool GetPointerToFilter();
};

