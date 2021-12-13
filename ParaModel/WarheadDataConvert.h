#pragma once

#include <ParaType.h>
#include <QSize>

class WarheadDataConvert
{
public:
	bool bFirst; 
	
	// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
	int CaArmHeadData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vOriginShape);
	int CenterArmHeadData(VSHAPE const& vOriginShape, VSHAPE& vDrawShape, QSize oScreenSzie);
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);

};