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

	// �������� ������ ������
	void GetCompressedMethod(DWORD *dwCompressed, char *szFormat, DWORD dwBitCount);
public:
	// ������ ���������� ��� ��������� ���� 
	HRESULT GetClassEnumerator(REFCLSID clsidDeviceClass);
	// �������� �������� ���������
	// ���� pDevParamInfo - �� �����, �� ���������� ���������� ��������� � �������
	// ���� pDevParamInfo - �����, �� ���������� � ���� �������� ���������
	int GetDeviceFriendlyName(device_param_info *pDevParamInfo);
	// ����������� ��� ���������� ������
	// ���� pDevParamInfo - �� �����, �� ���������� ���������� ���������� 
	// ���� pDevParamInfo - �����, �� ���������� � ���� �������������� ���������� � �������
	int GetCameraResolution(camera_resolution *pCamResolution, IBaseFilter *pBaseFilter);
};

