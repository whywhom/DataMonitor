#include "StdAfx.h"
#include "CurveInfo.h"


CCurveInfo::CCurveInfo(void)
{
	init();
}


CCurveInfo::~CCurveInfo(void)
{
}

void CCurveInfo::init(void)
{
	strCurveName = "";//标示
	curveName = 0;//标示
	curveData = 0;//数据
	curveTrack = 0;//轨道
	leftLimit = 0;//左边界
	rightLimit = 0;//右边界
	curveColor = RGB(255,0,0); //颜色
	graduation = 0;//刻度
	lineType = 0;//线型
	lineWidth = 0;//线宽
	wrapMode = 0;//回绕模式
	wrapCount = 0;//回绕次数
}