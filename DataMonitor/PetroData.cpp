// PetroData.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "PetroData.h"


// CPetroData

CPetroData::CPetroData()
{
	dept.integer = 0;//深度
	dept.decimal = 0;
	temp.integer = 0;//井温
	temp.decimal = 0;
	rm.integer = 0;//电阻率
	rm.decimal = 0;
	gr.integer = 0;//放射性
	gr.decimal = 0;
	mag[0].integer = 0;//磁三分量
	mag[0].decimal = 0;
	mag[1].integer = 0;//磁三分量
	mag[1].decimal = 0;
	mag[2].integer = 0;//磁三分量
	mag[2].decimal = 0;
	ccl.integer = 0;//磁定位器
	ccl.decimal = 0;
}

CPetroData::~CPetroData()
{
	dept.integer = 0;//深度
	dept.decimal = 0;
	temp.integer = 0;//井温
	temp.decimal = 0;
	rm.integer = 0;//电阻率
	rm.decimal = 0;
	gr.integer = 0;//放射性
	gr.decimal = 0;
	mag[0].integer = 0;//磁三分量
	mag[0].decimal = 0;
	mag[1].integer = 0;//磁三分量
	mag[1].decimal = 0;
	mag[2].integer = 0;//磁三分量
	mag[2].decimal = 0;
	ccl.integer = 0;//磁定位器
	ccl.decimal = 0;
}


// CPetroData 成员函数
