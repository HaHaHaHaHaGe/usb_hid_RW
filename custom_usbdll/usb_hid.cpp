#include "stdafx.h"
#include "usb_hid.h"
#include <process.h>
#include <mmsystem.h> 
#include <iostream>

usb_hid::usb_hid()
{
	close = false;
	isnewdata = false;
	dev_handle_ = NULL;
	hid_init();
	thrad_handle_ = (HANDLE)_beginthreadex(NULL, 0, usb_read_proc, this, NULL, NULL);
	CloseHandle(thrad_handle_);
}


usb_hid::~usb_hid()
{
	stop();
	hid_exit();
}


int usb_hid::start()
{
	dev_info_ = hid_enumerate(vendor_id, product_id);
	if (!dev_info_)//没有读到设备信息
	{
		is_run_ = false;
		std::cout << "cannot enumerate dev!" << std::endl;
		return -1;
	}
	//根据vid pid打开设备
	dev_handle_ = hid_open(vendor_id, product_id, NULL);
	if (!dev_handle_) 
	{
		std::cout << "cannot open dev!" << std::endl;
		return -1;
	}

	is_run_ = true;
	//thrad_handle_ = (HANDLE)_beginthreadex(NULL, 0, usb_read_proc, this, NULL, NULL);
	//if (thrad_handle_ == NULL)
	//{
	//	std::cout << "create thread error!" << std::endl;
	//	is_run_ = false;
	//	return -1;
	//}
	//WaitForSingleObject(thrad_handle_, 5000);
	//CloseHandle(thrad_handle_);
	
	return 0;
}

int usb_hid::stop()
{
	is_run_ = false;
	close = true;
	//WaitForSingleObject(thrad_handle_, 5000);
	//CloseHandle(thrad_handle_);
	if (dev_info_ != NULL)
	{
		hid_free_enumeration(dev_info_);
		dev_info_ = NULL;
	}
	if (dev_handle_ != NULL)
	{
		hid_close(dev_handle_);
		dev_handle_ = NULL;
	}
	return 0;
}

bool usb_hid::get_state()
{
	return is_run_;
}

unsigned int _stdcall usb_hid::usb_read_proc(LPVOID lp_param)
{
	usb_hid * params = (usb_hid *)lp_param;
	unsigned char buf[128];
	memset(buf, 0, sizeof(buf));
	int res = 0;
	while (!params->close)
	{
		if (params->dev_handle_ != NULL)
			res = hid_read(params->dev_handle_, buf, sizeof(buf));
		else
			res = -1;

		if (res > 0)
		{
			memcpy(params->Recv, buf, res);
			params->recv_len = res;
			params->isnewdata = true;
			//for (int i = 0; i < res; i++)
			//	printf("%02hhx ", params->Recv[i]);
			//printf("\n");
		}
		if (res < 0)
		{
			if (params->dev_info_ != NULL)
			{
				hid_free_enumeration(params->dev_info_);
				params->dev_info_ = NULL;
			}
			if (params->dev_handle_ != NULL)
			{
				hid_close(params->dev_handle_);
				params->dev_handle_ = NULL;
			}
			params->is_run_ = false;
			params->start();
		}
		Sleep(10);
	}
	return 0;
}

int usb_hid::usb_send_data(unsigned char * data, int len) {
	if (data == NULL)
		return -1;
	unsigned char *send = new unsigned char[len+1];
	memcpy(send + 1, data, len);
	*send = 0;
	int res = 0;
	if(dev_handle_ != NULL)
		res = hid_write(dev_handle_, send, len + 1);
	//printf("hid_write data res: %d\n",res);
	delete send;
	return res;
}

unsigned char * usb_hid::usb_read_data(int *len)
{
	if (isnewdata)
	{
		isnewdata = false;
		*len = recv_len;
		return Recv;
	}
	return nullptr;
}
