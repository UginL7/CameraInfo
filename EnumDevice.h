#pragma once

#include<dshow.h>
#include<atlbase.h>
#include<initguid.h>

static const int DEVICE_NAME_LENGTH = 256;
struct device_param_info
{
	IBaseFilter *pBaseFilter = NULL;
	char szName[DEVICE_NAME_LENGTH] = { 0 };
};

struct camera_resolution
{
	long lWidth = 0;
	long lHeight = 0;
	unsigned short usBitCount = 0;
	unsigned long ulCompressed = 0;
	char szFormat[16] = { 0 };
};

class CEnumDevice
{
public:
	
	CEnumDevice();
	~CEnumDevice();


private:
	HRESULT hr;
	ICreateDevEnum *pSystemDeviceEnum;
	IEnumMoniker *pEnumMoniker;
	IMoniker *pMoniker;

	// Получени метода сжатия
	void GetCompressedMethod(DWORD *dwCompressed, char *szFormat, DWORD dwBitCount);
public:
	// Создаёт энумератор для заданного типа 
	HRESULT GetClassEnumerator(REFCLSID clsidDeviceClass);
	// Получает перечень устройств
	// Если pDevParamInfo - не задан, то возвращает количество устройств в системе
	// Если pDevParamInfo - задан, то возвращает в него названия устройств
	int GetDeviceFriendlyName(device_param_info *pDevParamInfo);
	// Перечисляет все разрешения камеры
	// Если pDevParamInfo - не задан, то возвращает количество разрешений 
	// Если pDevParamInfo - задан, то возвращает в него поддерживаемые разрешения и форматы
	int GetCameraResolution(camera_resolution *pCamResolution, IBaseFilter *pBaseFilter);
};

