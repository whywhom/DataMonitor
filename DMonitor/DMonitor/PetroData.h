#pragma once

// CPetroData 命令目标
#include"PublicInterface.h"

class CPetroData : public CObject
{
public:
	CPetroData();
	virtual ~CPetroData();
	void CPetroData::InitData();
	DATA_PART dept;//深度			DEPT			m			一组，小数
	DATA_PART temp;//井温		    TEMP			C 			一组，小数
	DATA_PART rm;//电阻率			RM				rm			一组，小数
	DATA_PART gr;//放射性			GR				GAPI       一组，小数
	DATA_PART mag[3];//磁三分量		MAG			mag			三组，小数
	DATA_PART ccl;//磁定位器		CCL				CCL			一组，整数	
};


