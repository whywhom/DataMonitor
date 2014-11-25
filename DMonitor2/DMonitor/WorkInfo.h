#pragma once
class CWorkInfo
{
public:
	CWorkInfo(void);
	~CWorkInfo(void);
	void init();
public:
	CString strSignal;//信号名称
	CString strTitle;//信号缩写,解析数据用
	CString strUnit;//单位
	int leftLimit;//左边界
	int rightLimit;//右边界
	int trackNum;//轨道数
	COLORREF curveColor; //颜色
};

