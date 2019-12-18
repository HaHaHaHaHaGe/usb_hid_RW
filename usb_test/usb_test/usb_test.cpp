// hook_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Dbt.h>
#include <iostream>
#include "usb_hid.h"

usb_hid *usb = nullptr;
__declspec(dllexport) void init()
{
	if(usb == nullptr)
		usb = new usb_hid;
}
__declspec(dllexport) void deinit()
{
	if (usb != nullptr)
	{
		usb->stop();
		delete usb;
		usb = nullptr;
	}
}
__declspec(dllexport) unsigned char* read(int *len)
{
	if (usb != nullptr)
	{
		return usb->usb_read_data(len);
	}
	return nullptr;
}
__declspec(dllexport) int write(unsigned char *data, int len)
{
	if(usb != nullptr)
		return usb->usb_send_data(data, len);
	return -1;
}
//using namespace std;
//usb_hid usb;
//int _tmain(int argc, _TCHAR* argv[])
//{
//	unsigned char data[64];
//	for (int i = 0; i < 64; i++)
//		data[i] = i;
//
//
//	//int recv = usb.start();
//	//usb.usb_write_data(data,64);
//	while (1)
//	{
//		int len;
//		unsigned char* data = usb.usb_read_data(&len);
//		if (data != NULL)
//		{
//			for (int i = 0; i < len; i++)
//				printf("%02hhx ", data[i]);
//			printf("\n");
//		}
//		usb.usb_send_data(data, 64);
//	}
//
//	return 0;
//}