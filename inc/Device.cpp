#include "Device.h"
#include <stddef.h>
#include <iostream>
using namespace std;

Device::Device(DWORD _id)
	: id(_id)
	, handle(NULL)
{

}

void Device::SetHandle(HANDLE _handle)
{

	handle = _handle;
}

Device::~Device()
{

}





IMU::IMU(DWORD _id)
	: Device(_id)
{
}

IMU::~IMU()
{
	
}

bool IMU::RequestData()
{

}

void IMU::GetValues(const BYTE* data1, const BYTE* data2, float& roll, float& pitch, float& yaw)
{
	BYTE temp[4];
	int i;

	for(i=0; i<4; i++)
		temp[i] = data1[i];

	memcpy(&roll, temp, 4);

	for(i=0; i<4; i++)
		temp[i] = data1[4+i];

	memcpy(&pitch, temp, 4);

	for(i=0; i<4; i++)
		temp[i] = data2[i];

	memcpy(&yaw, temp, 4);

}

FT_Sensor::FT_Sensor(DWORD _id)
	: Device(_id)
{

}


const char* DeviceException::what() const throw()
{
	return "Device Error";

}



