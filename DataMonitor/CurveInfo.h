#pragma once
class CCurveInfo
{
public:
	CCurveInfo(void);
	~CCurveInfo(void);
	void init(void);
public:
	CString strCurveName;//��ʾ
	int curveName;//��ʾ
	int curveData;//����
	int curveTrack;//���
	int leftLimit;//��߽�
	int rightLimit;//�ұ߽�
	COLORREF curveColor; //��ɫ
	int graduation;//�̶�
	int lineType;//����
	int lineWidth;//�߿�
	int wrapMode;//����ģʽ
	int wrapCount;//���ƴ���
};

