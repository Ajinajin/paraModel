
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

// �õ�ս����������
int WarheadDataConvert::GetArmHeadPara(ArmHeadTopo& vArmHeadTopo)
{
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

	vCtrlPts = vArmHeadTopo.vCtrlPts; 

	return 0;
}

// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
int WarheadDataConvert::CaArmHeadData(VSHAPE& vOriginShape)
{
	//unitType �������� �������� 1-���� 2-Բ�� 3-����� 4-�߶�
	vOriginShape.clear(); 
	
	//ս�������Ϊ �����ϻ��±߳�+ǰ�Ǻ��+��Ǻ��

	//ǰ�� �����λ��� 
	SimpleShape Front;
	Front.nWH[0] = fT[8];
	Front.nWH[1] = fR[8] * 2;
	// �����μ���  ��ԭʼ���� �پ������� ��ͬ
	Front.unitType = 1;
	Front.unitIdx = 1;
	Front.nCen[0] = - fT[8] / 2;
	Front.nCen[1] = 0 ;
	vOriginShape.push_back(Front); 

	//��� �����λ���
	SimpleShape oBack;
	oBack.nWH[0] = fT[9];
	oBack.nWH[1] = fR[9]*2;
	oBack.unitType = 1;
	oBack.unitIdx = 2;
	oBack.nCen[0] = - fT[9] / 2 - fH[1] - fT[8];
	oBack.nCen[1] = 0;
	vOriginShape.push_back(oBack);


#pragma region ���
	//��Ǽ�¼�˹յ㣬���ݹյ������ �Ӿ��·����� ����Ϊ��
	SimpleShape ShellShapeB;
	ShellShapeB.unitType = 3;
	ShellShapeB.unitIdx = 3;
	float ShellWidth = fT[1];
	ShellShapeB.nWH[0] = ShellWidth;
	// ����Ϊ1 ������
// 	ShellShapeB.nWH[0] = 1;
	//������ǹյ���Ƶײ�
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//�ײ����
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeB.vCorner.push_back(PointB);
	}
	vPlaneDraw.push_back(ShellShapeB);

	// ��һ����Ҫ����ǰ��˵�
	if ( bFirst )
	{
		// ԭ�ȵ�ֵ��Ϊ��ֵ
		int nPtNum = vCtrlPts.size(); 
		for (int i = 0; i < nPtNum; i++)
	{
			vCtrlPts[i].setX(-vCtrlPts[i].x()); 
	}
		QPoint ptB1(-fT[8],fR[1]-fT[1]/2); 
		// ���ǰ�����ӵ�
		vCtrlPts.insert(0, ptB1); 
		// ��Ӻ�����ӵ�
		QPoint ptB2(-fT[8] - fH[1],fR[1] - fT[1] / 2);
		vCtrlPts.push_back(ptB2); 

		bFirst = false; 
	}
	// �������ֻ��Ҫ�ı�ֵ
	else
	{
		QPoint ptB2(-fT[8] - fH[1], fR[1] - fT[1] / 2);
		int nPtNum = vCtrlPts.size(); 
		vCtrlPts[nPtNum - 1] = ptB2; 
	}
	vPlaneDraw.push_back(FragmentShapeT);
#pragma endregion

	for (size_t i = 0; i < vCtrlPts.size(); i++)
	{
		PointB.nXY[0] = vCtrlPts[i].x();
		PointB.nXY[1] = vCtrlPts[i].y();
		ShellShapeB.vCorner.push_back(PointB);
	}
	// ��ռλ �����޸�����Ϊ ��������
	// vOriginShape[2] �Ӿ��·� ���ݴ� �����
	vOriginShape.push_back(ShellShapeB);
	ShellShapeB.unitIdx = 4;
	// vOriginShape[3] �Ӿ��·� ���ݴ� ���ڳ�
	vOriginShape.push_back(ShellShapeB);
	// vOriginShape[4] �Ӿ��·� ����С ���ڳ�
	vOriginShape.push_back(ShellShapeB);
	// vOriginShape[5] �Ӿ��·� ����С �����
	ShellShapeB.unitIdx = 3;
	vOriginShape.push_back(ShellShapeB);

	SimpleShape LiningShapeT;
	LiningShapeT.nWH[0] = LiningWidth;
	LiningShapeT.unitType = 4;
	LiningShapeT.unitIdx = 13;
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointT;//����
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y() + ShellWidth + FragmentWidth;
		LiningShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(LiningShapeT);
#pragma endregion



#pragma region ��ҩ
	// vOriginShape[6] ��ҩ������
	ShellShapeB.unitIdx = 5;
	vOriginShape.push_back(ShellShapeB);

#pragma endregion


	//��о
	SimpleShape BulletCore;
	float BulletCoreWidth = FindMapArmHead(vArmHeadTopo, "T5").nUnitPropty; //��о���T5
	float BulletCoreRadius = FindMapArmHead(vArmHeadTopo, "R5").nUnitPropty;//��о�뾶R5
	float BulletCoreLength = FindMapArmHead(vArmHeadTopo, "H5").nUnitPropty;//��о��H5 

	float BulletFuseRadius = FindMapArmHead(vArmHeadTopo, "R4").nUnitPropty;//���Ű뾶R4 
	float BulletFuseLength = FindMapArmHead(vArmHeadTopo, "H4").nUnitPropty;//���Ĺܳ�H4
	float BulletFuseWidth = FindMapArmHead(vArmHeadTopo, "T4").nUnitPropty;//���Ĺܺ��T4

	BulletCore.unitType = 4;
	BulletCore.unitIdx = 7; 
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
