#include "CaptureVideo.h"



CCaptureVideo::CCaptureVideo(device_param_info* pCaptureDevice, camera_frame_format_info* pFrameInfo)
{
	pDevParam = pCaptureDevice;
	pFrameFormat = pFrameInfo;
}


CCaptureVideo::~CCaptureVideo()
{
	if (pGraphBuilder)
	{
		pGraphBuilder.Release();
	}
	if (pCaptureGraphBuilder2)
	{
		pCaptureGraphBuilder2.Release();
	}
	if (pColorSC)
	{
		pColorSC.Release();
	}
	if (pDecompressor)
	{
		pDecompressor.Release();
	}
	if (pRenderer)
	{
		pRenderer.Release();
	}
}

void CCaptureVideo::InitGraph()
{
	HRESULT hr;
	hr = pGraphBuilder.CoCreateInstance(CLSID_FilterGraph);
	if (CheckHRError(hr))
	{
		onExit = true;
		return;
	}

	hr = pCaptureGraphBuilder2.CoCreateInstance(CLSID_CaptureGraphBuilder2);
	if (CheckHRError(hr))
	{
		onExit = true;
		return;
	}

	hr = pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
	if (CheckHRError(hr))
	{
		onExit = true;
		return;
	}
}

bool CCaptureVideo::GetPointerToFilter()
{
	HRESULT hr;
	
	hr = pDecompressor.CoCreateInstance(CLSID_MjpegDec);
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}
	hr = pGraphBuilder->AddFilter(pDecompressor, L"MJPEG Decompressor");
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}


	hr = pSGrabber.CoCreateInstance(CLSID_SampleGrabber);
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}
	hr = pGraphBuilder->AddFilter(pSGrabber, L"Sample Grabber");
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}
	hr = pSGrabber->QueryInterface(IID_PPV_ARGS(&pSampleGrabber));
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}

	
	hr = pColorSC.CoCreateInstance(CLSID_Colour);
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}
	hr = pGraphBuilder->AddFilter(pColorSC, L"Color Space Converter");
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}


	hr = pRenderer.CoCreateInstance(CLSID_VideoRenderer);
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}
	hr = pGraphBuilder->AddFilter(pRenderer, L"Video Renderer");
	CheckHRError(hr);
	if (onExit == true)
	{
		return onExit;
	}


	return false;
}

void CCaptureVideo::AddElementToFilter()
{
	IPin *pPinIn;
	IPin *pPinOut;
	HRESULT hr;
	FILTER_INFO FilterInfo;
	bool bRet = GetPointerToFilter();
	if (bRet == true)
	{
		return;
	}

	/*CComQIPtr<ISampleGrabber, &IID_ISampleGrabber> pSG_sample_grabber(pSampleGrabber);
	hr = pSG_sample_grabber->SetMediaType(&pFrameFormat->pAM_MediaType);
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}
*/
	hr = pCaptureGraphBuilder2->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pDecompressor, IID_IBaseFilter, (void**)&pPinIn);
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}
	pPinIn->Release();

	if (pFrameFormat->isDecoderNeeded == true)
	{
		hr = pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_CAPTURE, &pFrameFormat->pAM_MediaType.majortype, pDevParam->pBaseFilter, NULL, NULL);
	}
	else
	{
		hr = pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pDevParam->pBaseFilter, NULL, pSGrabber);
	}
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}

	/*hr = pGraphBuilder->AddFilter(pDevParam->pBaseFilter, pDevParam->wszName);
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}

	// Получение указателя для декомпрессорa и добавление его в фильтр граф
	CComPtr<IBaseFilter> pDecompressor;

	hr = pDecompressor.CoCreateInstance(CLSID_MjpegDec);
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}
	hr = pGraphBuilder->AddFilter(pDecompressor, L"MJPEG Decompressor");
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}

	hr = pCaptureGraphBuilder2->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pDecompressor, IID_IBaseFilter, (void**)&pPinIn);
	CheckHRError(hr);
	if (onExit == true)
	{
		return;
	}
	pPinIn->Release();


	

	*/
}

// Проверка на ошибку
bool CCaptureVideo::CheckHRError(HRESULT hr)
{
	char szText[512] = { 0 };
	char szOutput[MAX_ERROR_TEXT_LEN + 10] = { 0 };
	DWORD dwRet = 0;
	if (hr == S_OK)
	{
		return false;
	}

	dwRet = AMGetErrorText(hr, szText, MAX_ERROR_TEXT_LEN);
	sprintf(szOutput, "-- Error : %s. Code %x\n", szText, hr);
	OutputDebugString(szOutput);
	return true;
}