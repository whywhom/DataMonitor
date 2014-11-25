#include "StdAfx.h"
#include "WorkInfo.h"


CWorkInfo::CWorkInfo(void)
{
	init();
}


CWorkInfo::~CWorkInfo(void)
{
}

void CWorkInfo::init()
{
	strSignal = _T("");//信号名称
	strTitle = _T("");//信号缩写,解析数据用
	strUnit = _T("");//单位
	leftLimit = 0;//左边界
	rightLimit = 100;//右边界
	trackNum = 1;//轨道数
	curveColor = RGB(255,0,0); //颜色
}
