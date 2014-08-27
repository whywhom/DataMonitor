#include "StdAfx.h"
#include "PetroData.h"


CPetroData::CPetroData(void)
{
	dept = 0;//深度
	temp = 0;//井温
	rm = 0;//电阻率
	gr = 0;//放射性
	mag[0] = 0;//磁三分量
	mag[1] = 0;//磁三分量
	mag[2] = 0;//磁三分量
	ccl = 0;//磁定位器
}


CPetroData::~CPetroData(void)
{
	dept = 0;//深度
	temp = 0;//井温
	rm = 0;//电阻率
	gr = 0;//放射性
	mag[0] = 0;//磁三分量
	mag[1] = 0;//磁三分量
	mag[2] = 0;//磁三分量
	ccl = 0;//磁定位器
}
