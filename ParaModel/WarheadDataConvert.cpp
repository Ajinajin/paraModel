
#include <ParaType.h>
#include <WarheadDataConvert.h>
#include <math.h>
#include <Qpoint.h>

PARADES WarheadDataConvert::FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code)
{
	for (size_t i = 0; i < vArmHeadTopo.mapArmHead.size(); i++)
	{
		if (vArmHeadTopo.mapArmHead[i].sUnitName.contains(Code, Qt::CaseSensitive)) 
		{
			return vArmHeadTopo.mapArmHead[i];
		}
	}

	PARADES oRe; 
	memset(&oRe, 0, sizeof(oRe)); 
	return oRe; 
}

// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
int WarheadDataConvert::CaArmHeadData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vOriginShape)
{
	float fR[10]; 
	float fT[10]; 
	float fH[10]; 
	memset(fR, 0, 10 * sizeof(float)); 
	memset(fT, 0, 10 * sizeof(float));
	memset(fH, 0, 10 * sizeof(float));
	fR[1] = FindMapArmHead(vArmHeadTopo, "R1").nUnitPropty;
	fR[2] = FindMapArmHead(vArmHeadTopo, "R2").nUnitPropty;
	fR[4] = FindMapArmHead(vArmHeadTopo, "R4").nUnitPropty;
	fR[5] = FindMapArmHead(vArmHeadTopo, "R5").nUnitPropty;
	fR[6] = FindMapArmHead(vArmHeadTopo, "R6").nUnitPropty;
	fR[7] = FindMapArmHead(vArmHeadTopo, "R7").nUnitPropty;
	fR[8] = FindMapArmHead(vArmHeadTopo, "R8").nUnitPropty;
	fR[9] = FindMapArmHead(vArmHeadTopo, "R9").nUnitPropty;
	fT[1] = FindMapArmHead(vArmHeadTopo, "T1").nUnitPropty;
	fT[2] = FindMapArmHead(vArmHeadTopo, "T2").nUnitPropty;
	fT[4] = FindMapArmHead(vArmHeadTopo, "T4").nUnitPropty;
	fT[5] = FindMapArmHead(vArmHeadTopo, "T5").nUnitPropty; 
	fT[8] = FindMapArmHead(vArmHeadTopo, "T8").nUnitPropty;
	fT[9] = FindMapArmHead(vArmHeadTopo, "T9").nUnitPropty;
	fH[1] = FindMapArmHead(vArmHeadTopo, "H1").nUnitPropty; 
	fH[4] = FindMapArmHead(vArmHeadTopo, "H4").nUnitPropty;
	fH[5] = FindMapArmHead(vArmHeadTopo, "H5").nUnitPropty; 

	//unitType �������� �������� 1-���� 2-Բ�� 3-����� 4-��
	//��ջ�������
	vOriginShape.clear(); 
	
	//ս�������Ϊ �����ϻ��±߳�+ǰ�Ǻ��+��Ǻ��

	//ǰ��Ϊ ���25 ���ƿ�25 �� ǰ�ǰ뾶*2 ���ĵ�����Ϊ 
	SimpleShape Front;
	Front.nWH[0] = fT[8];
	Front.nWH[1] = fR[8] * 2;
	// �����μ���  ��ԭʼ���� �پ������� ��ͬ
	Front.unitType = 1;
	Front.unitIdx = 8;
	Front.nCen[0] = - fT[8] / 2;
	Front.nCen[1] = 0 ;
	vOriginShape.push_back(Front); 

	//���Ϊ ���25 ���ƿ�25 �� ��ǰ뾶*2 ���ĵ�����Ϊ 1000-25/2-�����ϻ��±߳�-ǰ�Ǻ�� /2,1000
	SimpleShape oBack;
	oBack.nWH[0] = fT[9];
	oBack.nWH[1] = fR[9]*2;
	oBack.unitType = 1;
	oBack.unitIdx = 9;
	oBack.nCen[0] = - fT[9] / 2 - fH[1] - fT[8];
	oBack.nCen[1] = 0;
	vOriginShape.push_back(oBack);

#pragma region ���
	//��Ǽ�¼�˹յ㣬���ݹյ������ �Ӿ��·�����
	SimpleShape ShellShapeB;
	ShellShapeB.unitType = 3;
	ShellShapeB.unitIdx = 11;
	float ShellWidth = fT[1];
	ShellShapeB.nWH[0] = ShellWidth;
	// ����Ϊ1 ������
	ShellShapeB.nWH[0] = 1;
	//������ǹյ���Ƶײ�
	PixelPos PointB;//�ײ����

	// ��һ����Ҫ����ǰ��˵�
	if ( bFirst )
	{
		// ԭ�ȵ�ֵ��Ϊ��ֵ
		int nPtNum = vArmHeadTopo.vTurnPoint.size(); 
		for (int i = 0; i < nPtNum; i++)
		{
			vArmHeadTopo.vTurnPoint[i].setX(-vArmHeadTopo.vTurnPoint[i].x()); 
		}
		QPointF ptB1(-fT[8],fR[1]-fT[1]/2); 
		// ���ǰ�����ӵ�
		vArmHeadTopo.vTurnPoint.insert(0, ptB1); 
		// ��Ӻ�����ӵ�
		QPointF ptB2(-fT[8] - fH[1],fR[1] - fT[1] / 2);
		vArmHeadTopo.vTurnPoint.push_back(ptB2); 

		bFirst = false; 
	}
	// �������ֻ��Ҫ�ı�ֵ
	else
	{
		QPointF ptB2(-fT[8] - fH[1], fR[1] - fT[1] / 2);
		int nPtNum = vArmHeadTopo.vTurnPoint.size(); 
		vArmHeadTopo.vTurnPoint[nPtNum - 1] = ptB2; 
	}
	
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PointB.nXY[0] = vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeB.vCorner.push_back(PointB);
	}
	vOriginShape.push_back(ShellShapeB);

	SimpleShape ShellShapeT;
	ShellShapeT.unitType = 3;
	ShellShapeT.unitIdx = 12;
	ShellShapeT.nWH[0] = ShellWidth;
	ShellShapeT.nWH[0] = 1;
	ShellShapeT.vCorner = ShellShapeB.vCorner;
	for (size_t i = 0; i < ShellShapeT.vCorner.size(); i++)
	{
		ShellShapeT.vCorner[i].nXY[1] = -ShellShapeT.vCorner[i].nXY[1];
	}
	vOriginShape.push_back(ShellShapeT);
#pragma endregion




#pragma region ��Ƭ
	//��Ǽ�¼�˹յ㣬���ݹյ㼰��Ǻ�ȼ���õ�ֵ������Ƭλ��
	
	float FragmentWidth = fR[6];		// ���� fH[7]
	SimpleShape FragmentShapeB;
	FragmentShapeB.nWH[0] = FragmentWidth;
	FragmentShapeB.nWH[0] = 1;
	FragmentShapeB.unitType = 4;
	FragmentShapeB.unitIdx = 13;
	//������ǹյ����
	FragmentShapeB.vCorner = ShellShapeB.vCorner;

	for (size_t i = 0; i < FragmentShapeB.vCorner.size(); i++)
	{
		FragmentShapeB.vCorner[i].nXY[1] = FragmentShapeB.vCorner[i].nXY[1] - (fR[1] - fR[2]) / 2; 
	}
	vOriginShape.push_back(FragmentShapeB);

	SimpleShape FragmentShapeT;
	FragmentShapeT.nWH[0] = FragmentWidth;
	FragmentShapeT.nWH[0] = 1;
	FragmentShapeT.unitType = 4;
	FragmentShapeT.unitIdx = 14;
	FragmentShapeT.vCorner = FragmentShapeB.vCorner;
	for (size_t i = 0; i < FragmentShapeT.vCorner.size(); i++)
	{
		FragmentShapeT.vCorner[i].nXY[1] = -FragmentShapeT.vCorner[i].nXY[1] ;
	}
	vOriginShape.push_back(FragmentShapeT);
#pragma endregion





#pragma region �ڳ�
	//����������ĳߴ�ƽ�ƺ��

	SimpleShape InnerShapeB;
	InnerShapeB.nWH[0] = fT[2];
	InnerShapeB.unitType = 3;
	InnerShapeB.unitIdx = 15;

	InnerShapeB.vCorner = ShellShapeB.vCorner; 
	//������ǹյ����
	float fOffY = fR[1] - fR[2] - fT[1] / 2 - fT[2] / 2; 
	for (size_t i = 0; i < InnerShapeB.vCorner.size(); i++)
	{
		InnerShapeB.vCorner[i].nXY[1] -= fOffY ;
	}
	vOriginShape.push_back(InnerShapeB);

	SimpleShape InnerShapeT;
	InnerShapeT.nWH[0] = fT[2];
	InnerShapeT.unitType = 3;
	InnerShapeT.unitIdx = 16;
	InnerShapeT.vCorner = ShellShapeT.vCorner;
	//������ǹյ����
	for (size_t i = 0; i < InnerShapeT.vCorner.size(); i++)
	{
		InnerShapeT.vCorner[i].nXY[1] += fOffY ;
	}
	vOriginShape.push_back(InnerShapeT);
#pragma endregion



#pragma region ��ҩ
	SimpleShape AmmunitionShape;
	AmmunitionShape.unitType = 4;
	AmmunitionShape.unitIdx = 7;
	AmmunitionShape.nWH[0] = 1;
	//�����ڳĳߴ���������������ڳĺ�ȵõ�

	// �Ӿ��·��ڳ�
	AmmunitionShape.vCorner = InnerShapeB.vCorner;
	for (size_t i = 0; i < AmmunitionShape.vCorner.size(); i++)
	{
		AmmunitionShape.vCorner[i].nXY[1] -= fT[2]/2;
	}
	// �Ӿ��Ϸ��ڳ�
	VPIXELPOS vUpper = InnerShapeT.vCorner; 
	int i = vUpper.size() - 1; 
	for (; i >= 0 ; i--)
	{
		vUpper[i].nXY[1] += fT[2] / 2; 		
		AmmunitionShape.vCorner.push_back(vUpper[i]);
	}
	vOriginShape.push_back(AmmunitionShape);

#pragma endregion


	//��о
	SimpleShape BulletCore;

	BulletCore.unitType = 4;
	BulletCore.unitIdx = 4; 
	BulletCore.nWH[0] = fT[5];

	PixelPos BulletFuseAPoint;//���Ĺܵ�1  
	BulletFuseAPoint.nXY[0] = -fT[8];
	BulletFuseAPoint.nXY[1] = -fR[4] + fT[4]/2;
	BulletFuseAPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseAPoint);


	PixelPos BulletFuseBPoint;//���Ĺܵ�2
	BulletFuseBPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseBPoint.nXY[1] = -fR[4] + fT[4]/2 ;
	BulletFuseBPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseBPoint);


	PixelPos BulletFuseCPoint;//���Ĺܵ�3
	BulletFuseCPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseCPoint.nXY[1] = -fR[5] + fT[5]/2 ;
	BulletFuseCPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseCPoint);


	PixelPos BulletCoreAPoint;//��о1
	BulletCoreAPoint.nXY[0] = -fT[8] - fH[4] - fH[5];
	BulletCoreAPoint.nXY[1] = -fR[5] + fT[5]/2 ;
	BulletCoreAPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreAPoint);


	PixelPos BulletCoreBPoint;//��о2
	BulletCoreBPoint.nXY[0] = -fT[8] - fH[4] - fH[5];
	BulletCoreBPoint.nXY[1] = fR[5] - fT[5]/2 ;
	BulletCoreBPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreBPoint);


	PixelPos BulletCoreCPoint;//��о3
	BulletCoreCPoint.nXY[0] = -fT[8] - fH[4];
	BulletCoreCPoint.nXY[1] = fR[5]- fT[5]/2;
	BulletCoreCPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreCPoint);


	PixelPos BulletFuseDPoint;//���Ĺܵ�4
	BulletFuseDPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseDPoint.nXY[1] = fR[4]- fT[4]/2;
	BulletFuseDPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseDPoint);


	PixelPos BulletFuseEPoint;//���Ĺܵ�5
	BulletFuseEPoint.nXY[0] = -fT[8];
	BulletFuseEPoint.nXY[1] = fR[4]- fT[4]/2;
	BulletFuseEPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseEPoint);

	vOriginShape.push_back(BulletCore);

	return 0;
}

int WarheadDataConvert::CenterArmHeadData(VSHAPE const& vOriginShape, VSHAPE& vDrawShape, QSize oScreenSzie)
{
	int nShapeNum = vOriginShape.size(); 
	if (nShapeNum < 1)
		return 0; 

	vDrawShape = vOriginShape; 
	int i = 0; 
	for ( i= 0 ; i < 2 ; i++ )
	{
		vDrawShape[i].nCen[0] += oScreenSzie.width() / 2; 
		vDrawShape[i].nCen[1] += oScreenSzie.height() / 2; 
	}
	int nPixelNum; 
	for (i = 2; i < nShapeNum ; i++)
	{
		nPixelNum = vOriginShape[i].vCorner.size(); 
		for ( int j = 0 ;j < nPixelNum; j++)
		{
			vDrawShape[i].vCorner[j].nXY[0] +=  oScreenSzie.width() / 2;
			vDrawShape[i].vCorner[j].nXY[1] += oScreenSzie.height() / 2;
		}
	}

	return 0;
}