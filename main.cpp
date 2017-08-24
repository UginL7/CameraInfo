#include<stdio.h>
#include<windows.h>
#include <ks.h>
#include <ksproxy.h>

#include "EnumDevice.h"

#pragma comment (lib, "strmiids.lib")

int main()
{
	device_param_info *pBufferOfDevice;
	camera_resolution *pCameraResolution;
	void *pResOriginal;
	void *pDevOriginal;
	int nSizeOfDeviceArray = 0;
	int nSizeOfResolutionArray = 0;
	HRESULT hr;
	CEnumDevice *pEnumDevice;
	device_param_info DevInfo;
	camera_resolution CamRes;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr != S_OK)
	{
		printf("CoInititializeEx error");
		system("pause");
		return 0;
	}

	pEnumDevice = new CEnumDevice();

	hr = pEnumDevice->GetClassEnumerator(CLSID_VideoInputDeviceCategory);
	if (hr != S_OK)
	{
		delete pEnumDevice;
		printf("GetClassEnumerator error");
		system("pause");
		return 0;
	}

	nSizeOfDeviceArray = pEnumDevice->GetDeviceFriendlyName(nullptr);
	if (nSizeOfDeviceArray == 0) {
		delete pEnumDevice;
		printf("GetDeviceFriendlyName error");
		system("pause");
		return 0;
	}

	pBufferOfDevice = (device_param_info *)malloc(nSizeOfDeviceArray * sizeof(device_param_info));
	if (pBufferOfDevice == nullptr)
	{
		delete pEnumDevice;
		printf("malloc error");
		system("pause");
		return 0;
	}
	pDevOriginal = pBufferOfDevice;

	memset(pBufferOfDevice, 0, nSizeOfDeviceArray * sizeof(device_param_info));
	pEnumDevice->GetDeviceFriendlyName(pBufferOfDevice);
	for (int i = 0; i < nSizeOfDeviceArray; i++)
	{
		DevInfo = pBufferOfDevice[0];
		printf("%d)\tname = %s\n\n", i+1,DevInfo.szName);

		// Перечисление для каждой камеры разрешения и формата картинки
		nSizeOfResolutionArray = pEnumDevice->GetCameraResolution(nullptr, DevInfo.pBaseFilter);
		pCameraResolution = (camera_resolution *)malloc(sizeof(camera_resolution) * nSizeOfResolutionArray);
		pResOriginal = pCameraResolution;
		memset(pCameraResolution, 0, nSizeOfResolutionArray * sizeof(camera_resolution));
		pEnumDevice->GetCameraResolution(pCameraResolution, DevInfo.pBaseFilter);
		for (int i = 0; i < nSizeOfResolutionArray; i++)
		{
			CamRes = pCameraResolution[0];
			printf("%d)\twidth = %d\theight=%d\tbit color=%d\tcompressed=%s\n", i+1, CamRes.lWidth, CamRes.lHeight, CamRes.usBitCount, CamRes.szFormat);
			pCameraResolution++;
		}
		pCameraResolution = (camera_resolution*)pResOriginal;
		free(pCameraResolution);

		printf("\n\n=================================================\n\n");
		pBufferOfDevice++;
	}

	pBufferOfDevice = (device_param_info *)pDevOriginal;
	free(pBufferOfDevice); // Рушится куча, спросить Макса, что тут не так
	delete pEnumDevice;
	system("pause");
	return 0;
}