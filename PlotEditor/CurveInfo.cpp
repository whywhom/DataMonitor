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
	strCurveName = "";//��ʾ
	curveName = 0;//��ʾ
	curveData = 0;//����
	curveTrack = 0;//���
	leftLimit = 0;//��߽�
	rightLimit = 0;//�ұ߽�
	curveColor = RGB(255,0,0); //��ɫ
	graduation = 0;//�̶�
	lineType = 0;//����
	lineWidth = 0;//�߿�
	wrapMode = 0;//����ģʽ
	wrapCount = 0;//���ƴ���
}