#pragma once
class CCurveInfo
{
public:
	CCurveInfo(void);
	~CCurveInfo(void);
	void init(void);
public:
	CString strCurveName;//标示
	int curveName;//标示
	int curveData;//数据
	int curveTrack;//轨道
	int leftLimit;//左边界
	int rightLimit;//右边界
	COLORREF curveColor; //颜色
	int graduation;//刻度
	int lineType;//线型
	int lineWidth;//线宽
	int wrapMode;//回绕模式
	int wrapCount;//回绕次数
};

