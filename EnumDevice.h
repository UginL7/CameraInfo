#pragma once

#include "structures.h"

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
	int GetDeviceAvailableResolution(camera_frame_format_info *pCamResolution, IBaseFilter *pBaseFilter);
};

