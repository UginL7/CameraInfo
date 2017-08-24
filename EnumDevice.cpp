#include "EnumDevice.h"

CEnumDevice::CEnumDevice()
{
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pSystemDeviceEnum);
	if (FAILED(hr))
	{
		pSystemDeviceEnum = NULL;
	}
}

CEnumDevice::~CEnumDevice()
{
	pSystemDeviceEnum->Release();
}

// Получени метода сжатия
void CEnumDevice::GetCompressedMethod(DWORD *dwCompressed, char *szFormat, DWORD dwBitCount)
{
	char *pCompressed = (char*)dwCompressed;
	if (*pCompressed == '\0')
	{
		sprintf(szFormat, "RGB%d", dwBitCount);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			*szFormat++ = *pCompressed++;
		}
	}
}

// Создаёт энумератор для заданного типа 
HRESULT CEnumDevice::GetClassEnumerator( REFCLSID clsidDeviceClass )
{
	if (pSystemDeviceEnum == NULL)
	{
		return S_FALSE;
	}
	hr = pSystemDeviceEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumMoniker, 0);

	return hr;
}

// Получает перечень устройств
// Если pDevParamInfo - не задан, то возвращает количество устройств в системе
// Если pDevParamInfo - задан, то возвращает в него названия устройств
int CEnumDevice::GetDeviceFriendlyName(device_param_info *pDevParamInfo)
{
	ULONG ulBuffSize = 0;
	IPropertyBag *pBag;
	VARIANT varName;
	ULONG ulFetched;
	struct device_param_info pDevInfo;

	int nDeviceCounter = 0;
	while (pEnumMoniker->Next(1, &pMoniker, &ulFetched) == S_OK)
	{
		hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void**)&pBag);
		if (SUCCEEDED(hr))
		{
			VariantInit(&varName);
			hr = pBag->Read(L"Description", &varName, 0);
			if (FAILED(hr))
			{
				hr = pBag->Read(L"FriendlyName", &varName, 0);
			}
			if (SUCCEEDED(hr))
			{
				if (pDevParamInfo == nullptr)
				{
					nDeviceCounter++;
				}
				else
				{
					ulBuffSize = WideCharToMultiByte(CP_ACP, 0, (WCHAR *)((char *)varName.bstrVal), -1, 0, 0, 0, 0);
					if (ulBuffSize > 0)
					{
						memset((void*)&pDevInfo, 0, sizeof(device_param_info));
						WideCharToMultiByte(CP_ACP, 0, (WCHAR *)((char *)varName.bstrVal), -1, pDevInfo.szName, ulBuffSize, 0, 0);

						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pDevInfo.pBaseFilter);
						if (SUCCEEDED(hr))
						{
							memcpy(pDevParamInfo, (void*)&pDevInfo, sizeof(device_param_info));
							pDevParamInfo++;
						}
						
					}

				}
			}
			VariantClear(&varName);
			pBag->Release();
		}
		pMoniker->Release();
	}
	if (pDevParamInfo == nullptr)
	{
		pEnumMoniker->Reset();
	}
	else
	{
		pEnumMoniker->Release();
	}
	return nDeviceCounter;
}

// Перечисляет все разрешения камеры
// Если pDevParamInfo - не задан, то возвращает количество разрешений 
// Если pDevParamInfo - задан, то возвращает в него поддерживаемые разрешения и форматы
int CEnumDevice::GetCameraResolution(camera_resolution *pCamResolution, IBaseFilter *pBaseFilter)
{
	int nCounter = 0;
	ULONG ulFetchedMT = 0;	
	ULONG ulFetched = 0;
	struct camera_resolution pCamRes;
	AM_MEDIA_TYPE *pMT;
	VIDEOINFOHEADER *pVIH;
	IPin *pPin;
	IEnumPins *pEnumPin;
	IEnumMediaTypes *pEnumMediaType;
	IKsPropertySet *pKSPropSet;
	GUID PinCategory;
	DWORD dwRet;

	hr = pBaseFilter->EnumPins(&pEnumPin);
	if (SUCCEEDED(hr))
	{
		while (pEnumPin->Next(1, &pPin, &ulFetched) == S_OK)
		{
			hr = pPin->QueryInterface(IID_PPV_ARGS(&pKSPropSet));
			if (SUCCEEDED(hr))
			{
				hr = pKSPropSet->Get(AMPROPSETID_Pin, AMPROPERTY_PIN_CATEGORY, NULL, 0, &PinCategory, sizeof(GUID), &dwRet);
				if (SUCCEEDED(hr) && dwRet == sizeof(GUID))
				{
					if (PinCategory == PIN_CATEGORY_CAPTURE)
					{
						hr = pPin->EnumMediaTypes(&pEnumMediaType);
						if (SUCCEEDED(hr))
						{
							while (pEnumMediaType->Next(1, &pMT, &ulFetchedMT) == S_OK)
							{
								if (pCamResolution == nullptr)
								{
									nCounter++;
								}
								else
								{
									pVIH = reinterpret_cast<VIDEOINFOHEADER *>(pMT->pbFormat);
									memset((void*)&pCamRes, 0, sizeof(camera_resolution));
									pCamRes.lWidth = pVIH->bmiHeader.biWidth;
									pCamRes.lHeight = pVIH->bmiHeader.biHeight;
									pCamRes.usBitCount = pVIH->bmiHeader.biBitCount;
									GetCompressedMethod(&pVIH->bmiHeader.biCompression, pCamRes.szFormat, pCamRes.usBitCount);
									pCamRes.ulCompressed = pVIH->bmiHeader.biCompression;

									memcpy(pCamResolution, (void*)&pCamRes, sizeof(camera_resolution));
									pCamResolution++;
								}
							}
							pEnumMediaType->Release();
						}
					}
				}
			}
			pPin->Release();
		}
	}


	if (pCamResolution == nullptr)
	{
		pEnumPin->Reset();
	}
	else
	{
		pEnumPin->Release();
	}
	return nCounter;
}