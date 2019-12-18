// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "usb_hid.h"
#define DLL_EXPORT __declspec(dllexport)
usb_hid *usb = nullptr;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DLL_EXPORT void init()
{
	if (usb == nullptr)
		usb = new usb_hid;
}
DLL_EXPORT void deinit()
{
	if (usb != nullptr)
	{
		usb->stop();
		delete usb;
		usb = nullptr;
	}
}
DLL_EXPORT unsigned char* read(int *len)
{
	if (usb != nullptr)
	{
		return usb->usb_read_data(len);
	}
	return nullptr;
}
DLL_EXPORT int write(unsigned char *data, int len)
{
	if (usb != nullptr)
		return usb->usb_send_data(data, len);
	return -1;
}