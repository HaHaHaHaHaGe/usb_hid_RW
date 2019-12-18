// hook_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Dbt.h>
#include <iostream>
#include "usb_hid.h"
#define DLL_EXPORT __declspec(dllexport)
usb_hid *usb = nullptr;
#ifdef __cplusplus
extern "C"
{
#endif
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
#ifdef __cplusplus
}
#endif
