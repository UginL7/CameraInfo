#pragma once
#include<dshow.h>
#include<dvdmedia.h>
#include<atlbase.h>
#include<initguid.h>

// {C1F400A0-3F08-11D3-9F0B-006008039E37}
DEFINE_GUID(CLSID_SampleGrabber,
	0xC1F400A0, 0x3F08, 0x11D3, 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37); //qedit.dll

static const int DEVICE_NAME_LENGTH = 256;

struct device_param_info
{
	IBaseFilter *pBaseFilter = NULL;
	wchar_t wszName[DEVICE_NAME_LENGTH] = { 0 };
	char szName[DEVICE_NAME_LENGTH] = { 0 };
};

struct sample_size
{
	unsigned long ulWidth = 0;
	unsigned long ulHeight = 0;
	unsigned long ulFrameSize = 0;
	unsigned short usBitCount = 0;
};

struct camera_frame_format_info
{
	sample_size image_size;
	char szFormat[16] = { 0 };
	char szVIH[16] = { 0 };
	bool isCSCNeeded = false;
	bool isDecoderNeeded = false;

	AM_MEDIA_TYPE pAM_MediaType = { 0 };
};
