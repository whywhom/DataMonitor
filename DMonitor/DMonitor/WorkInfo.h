#pragma once
class CWorkInfo
{
public:
	CWorkInfo(void);
	~CWorkInfo(void);
public:
	CString strTitie;//信号名称
	CString strAbbr;//信号缩写
	CString strUnit;//单位
	int leftLimit;//左边界
	int rightLimit;//右边界
	COLORREF curveColor; //颜色
};

