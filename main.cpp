#include<stdio.h>
#include<windows.h>
#include <ks.h>
#include <ksproxy.h>
#include "CaptureVideo.h"
#include "EnumDevice.h"


#pragma comment (lib, "strmiids.lib")
#pragma comment (lib, "Quartz.lib")

void CapturingVideo()
{

}

int main()
{
	device_param_info *pAvailableDevices = nullptr;
	device_param_info *pDecompressor = nullptr;
	device_param_info *pColorSC = nullptr;
	device_param_info *pRenderer = nullptr;
	camera_frame_format_info *pAvailableResolution = nullptr;
	device_param_info DevInfo;
	camera_frame_format_info FrameRes;
	void *pResOriginal;
	void *pDevOriginal;
	int nSizeOfDeviceArray = 0;
	int nSizeOfResolutionArray = 0;
	int nSizeNeeded = 0;
	HRESULT hr;
	CEnumDevice *pEnumDevice = nullptr;
	int nCameraChoise = 0;
	int nResolutionChoise = 0;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr != S_OK)
	{
		printf("CoInititializeEx error");
		system("pause");
		return 0;
	}

	pEnumDevice = new CEnumDevice();

	// получение энумератора камер(устройств видео захвата)
	hr = pEnumDevice->GetClassEnumerator(CLSID_VideoInputDeviceCategory);
	if (hr != S_OK)
	{
		delete pEnumDevice;
		printf("GetClassEnumerator error");
		system("pause");
		return 0;
	}

	// Получение количества доступных камер
	nSizeOfDeviceArray = pEnumDevice->GetDeviceFriendlyName(nullptr);
	if (nSizeOfDeviceArray == 0) {
		delete pEnumDevice;
		printf("GetDeviceFriendlyName error");
		system("pause");
		return 0;
	}
	pAvailableDevices = (device_param_info *)malloc(nSizeOfDeviceArray * sizeof(device_param_info));
	if (pAvailableDevices == nullptr)
	{
		delete pEnumDevice;
		printf("malloc error");
		system("pause");
		return 0;
	}
	pDevOriginal = pAvailableDevices;

	// Перечисление доступных камер
	memset(pAvailableDevices, 0, nSizeOfDeviceArray * sizeof(device_param_info));
	pEnumDevice->GetDeviceFriendlyName(pAvailableDevices);
	for (int i = 0; i < nSizeOfDeviceArray; i++)
	{
		DevInfo = pAvailableDevices[0];
		printf("%d)\tname = %s\n\n", i + 1, DevInfo.szName);

		printf("\n\n=================================================\n\n");
		pAvailableDevices++;
	}

	// выбор камеры для работы
	printf("Input camera number : ");
	scanf_s("%d", &nCameraChoise);
	pAvailableDevices = (device_param_info*)pDevOriginal + nCameraChoise - 1;
	DevInfo = pAvailableDevices[0];

	// Получение количества разрешений для выбранной камеры 
	nSizeOfResolutionArray = pEnumDevice->GetDeviceAvailableResolution(nullptr, DevInfo.pBaseFilter);
	pAvailableResolution = (camera_frame_format_info *)malloc(sizeof(camera_frame_format_info) * nSizeOfResolutionArray);
	if (pAvailableResolution == nullptr)
	{
		delete pEnumDevice;
		printf("malloc error");
		system("pause");
		return 0;
	}
	pResOriginal = pAvailableResolution;

	// Перечисление для выбранной камеры разрешений и форматов картинки
	memset(pAvailableResolution, 0, nSizeOfResolutionArray * sizeof(camera_frame_format_info));
	pEnumDevice->GetDeviceAvailableResolution(pAvailableResolution, DevInfo.pBaseFilter);
	for (int i = 0; i < nSizeOfResolutionArray; i++)
	{
		FrameRes = pAvailableResolution[0];
		printf("%d)\twidth = %d\theight=%d\tbit color=%d\tcompressed=%s\tVIH=%s\n", i + 1, FrameRes.image_size.ulWidth, FrameRes.image_size.ulHeight, FrameRes.image_size.usBitCount, FrameRes.szFormat, FrameRes.szVIH);
		pAvailableResolution++;
	}

	// выбор разрешения для работы
	printf("Input resolution number : ");
	scanf_s("%d", &nResolutionChoise);
	pAvailableResolution = (camera_frame_format_info*)(pResOriginal)+nResolutionChoise - 1;
	FrameRes = pAvailableResolution[0];

	printf("%d)\twidth = %d\theight=%d\tbit color=%d\tcompressed=%s\tVIH=%s\n", nResolutionChoise, FrameRes.image_size.ulWidth, FrameRes.image_size.ulHeight, FrameRes.image_size.usBitCount, FrameRes.szFormat, FrameRes.szVIH);


	if (pDecompressor != nullptr)
	{
		pDecompressor->pBaseFilter->Release();
		free(pDecompressor);
	}
	if(pColorSC != nullptr)
	{
		pColorSC->pBaseFilter->Release();
		free(pColorSC);
	}
	if (pRenderer != nullptr)
	{
		pRenderer->pBaseFilter->Release();
		free(pRenderer);
	}
	
	if (pAvailableResolution != nullptr)
	{
		pAvailableResolution = (camera_frame_format_info*)pResOriginal;
		free(pAvailableResolution);
	}	
	
	if (pAvailableDevices != nullptr)
	{
		pAvailableDevices = (device_param_info *)pDevOriginal;
		// Освобождение ресурсов
		for (int i = 0; i < nSizeOfDeviceArray; i++)
		{
			DevInfo = pAvailableDevices[0];
			DevInfo.pBaseFilter->Release();
			pAvailableDevices++;
		}
		pAvailableDevices = (device_param_info *)pDevOriginal;
		free(pAvailableDevices);
	}

	if (pEnumDevice != nullptr)
	{
		delete pEnumDevice;
	}

	CoUninitialize();
	system("pause");
	return 0;
}