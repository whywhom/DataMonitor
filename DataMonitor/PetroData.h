#pragma once
class CPetroData
{
public:
	CPetroData(void);
	~CPetroData(void);
	float dept;//深度			DEPT			m			一组，小数
	float temp;//井温		    TEMP			C 			一组，小数
	float rm;//电阻率			RM				rm			一组，小数
	float gr;//放射性			GR				GAPI       一组，小数
	float mag[3];//磁三分量		MAG			mag			三组，小数
	int ccl;//磁定位器		CCL				CCL			一组，整数	
};

