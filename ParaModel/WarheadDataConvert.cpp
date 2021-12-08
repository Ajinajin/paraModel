
#include <ParaType.h>
#include <WarheadDataConvert.h>
#include <math.h>
#include <Qpoint.h>

PARADES WarheadDataConvert::FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code)
{
	for (size_t i = 0; i < vArmHeadTopo.mapArmHead.size(); i++)
	{
		if (vArmHeadTopo.mapArmHead[i].sUnitName.contains(Code, Qt::CaseSensitive)) {
			return vArmHeadTopo.mapArmHead[i];
		}
	}
}

// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
int WarheadDataConvert::CalPlaneData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vPlaneDraw)
{
	//unitType �������� �������� 1-���� 2-Բ�� 3-����� 4-��
	//��ջ�������
	vPlaneDraw.clear();
	//������ԭ��Ϊ 200,200
	QPointF f = QPointF(600, 300);
	//ս�������Ϊ �����ϻ��±߳�+ǰ�Ǻ��+��Ǻ��

	//ǰ��Ϊ ���25 ���ƿ�25 �� ǰ�ǰ뾶*2 ���ĵ�����Ϊ 1000-25/2,1000
	SimpleShape Front;
	float FrontWidth = FindMapArmHead(vArmHeadTopo, "T8").nUnitPropty;
	float FrontHeight = FindMapArmHead(vArmHeadTopo, "R8").nUnitPropty * 2;
	Front.nWH[0] = FrontWidth;
	Front.nWH[1] = FrontHeight;
	Front.unitType = 1;
	Front.unitIdx = 8;
	Front.nCen[0] = f.x() - FrontWidth / 2;
	Front.nCen[1] = f.y();
	vPlaneDraw.push_back(Front);

	//���Ϊ ���25 ���ƿ�25 �� ��ǰ뾶*2 ���ĵ�����Ϊ 1000-25/2-�����ϻ��±߳�-ǰ�Ǻ�� /2,1000
	SimpleShape Break;
	float BreakWidth = FindMapArmHead(vArmHeadTopo, "T9").nUnitPropty;
	float BreakHeight = FindMapArmHead(vArmHeadTopo, "R9").nUnitPropty * 2;
	float BulletLength = FindMapArmHead(vArmHeadTopo, "H1").nUnitPropty; ///�����ϱ߳�
	Break.nWH[0] = BreakWidth;
	Break.nWH[1] = BreakHeight;
	Break.unitType = 1;
	Break.unitIdx = 9;
	Break.nCen[0] = f.x() - BreakWidth / 2 - BulletLength - FrontWidth;
	Break.nCen[1] = f.y();
	vPlaneDraw.push_back(Break);


#pragma region ���
	//��Ǽ�¼�˹յ㣬���ݹյ������
	SimpleShape ShellShapeB;
	ShellShapeB.unitType = 4;
	ShellShapeB.unitIdx = 11;
	float ShellWidth = FindMapArmHead(vArmHeadTopo, "Shell").nUnitPropty;
	ShellShapeB.nWH[0] = ShellWidth;
	//������ǹյ���Ƶײ�
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//�ײ����
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeB.vCorner.push_back(PointB);
	}
	vPlaneDraw.push_back(ShellShapeB);

	SimpleShape ShellShapeT;
	ShellShapeT.unitType = 4;
	ShellShapeT.unitIdx = 11;
	ShellShapeT.nWH[0] = ShellWidth;
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointT;//�������
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(ShellShapeT);
#pragma endregion




#pragma region ��Ƭ
	//��Ǽ�¼�˹յ㣬���ݹյ㼰��Ǻ�ȼ���õ�ֵ������Ƭλ��
	float FragmentWidth = FindMapArmHead(vArmHeadTopo, "Fragment").nUnitPropty;
	SimpleShape FragmentShapeB;
	FragmentShapeB.nWH[0] = FragmentWidth;
	FragmentShapeB.unitType = 4;
	FragmentShapeB.unitIdx = 12;
	//������ǹյ����
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//�ײ�
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y() - ShellWidth;
		FragmentShapeB.vCorner.push_back(PointB);

	}
	vPlaneDraw.push_back(FragmentShapeB);

	SimpleShape FragmentShapeT;
	FragmentShapeT.nWH[0] = FragmentWidth;
	FragmentShapeT.unitType = 4;
	FragmentShapeT.unitIdx = 12;
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointT;//����
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y() + ShellWidth;
		FragmentShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(FragmentShapeT);
#pragma endregion





#pragma region �ڳ�
	//��Ǽ�¼�˹յ㣬���ݹյ㼰��Ǻ�ȡ���Ƭ��ȼ���õ�ֵ������Ƭλ��
	float LiningWidth = FindMapArmHead(vArmHeadTopo, "Lining").nUnitPropty;

	SimpleShape LiningShapeB;
	LiningShapeB.nWH[0] = LiningWidth;
	LiningShapeB.unitType = 4;
	LiningShapeB.unitIdx = 13;
	//������ǹյ����
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//�ײ�
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y() - ShellWidth - FragmentWidth;
		LiningShapeB.vCorner.push_back(PointB);
	}
	vPlaneDraw.push_back(LiningShapeB);

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
	//��Ǽ�¼�˹յ㣬���ݹյ㼰��Ǻ�ȡ���Ƭ��ȡ��ڳĺ�ȼ���õ�ֵ������Ƭλ��
	SimpleShape AmmunitionShape;
	AmmunitionShape.unitType = 4;
	AmmunitionShape.unitIdx = 7;
	AmmunitionShape.nWH[0] = 1;
	//������ǹյ����
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//�ײ�
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y() - ShellWidth - FragmentWidth - LiningWidth;
		AmmunitionShape.vCorner.push_back(PointB);
	}
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{

		PixelPos PointT;//����
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[vArmHeadTopo.vTurnPoint.size()-i-1].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[vArmHeadTopo.vTurnPoint.size()-i-1].y() + ShellWidth + FragmentWidth + LiningWidth;
		AmmunitionShape.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(AmmunitionShape);

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
	BulletCore.unitIdx = 4; 
	BulletCore.nWH[0] = BulletCoreWidth;

	PixelPos BulletFuseAPoint;//���Ĺܵ�1  
	BulletFuseAPoint.nXY[0] = f.x() - FrontWidth;
	BulletFuseAPoint.nXY[1] = f.y() - BulletFuseRadius;
	BulletFuseAPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseAPoint);


	PixelPos BulletFuseBPoint;//���Ĺܵ�2
	BulletFuseBPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseBPoint.nXY[1] = f.y() - BulletFuseRadius;
	BulletFuseBPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseBPoint);


	PixelPos BulletFuseCPoint;//���Ĺܵ�3
	BulletFuseCPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseCPoint.nXY[1] = f.y() - BulletCoreRadius;
	BulletFuseCPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseCPoint);


	PixelPos BulletCoreAPoint;//��о1
	BulletCoreAPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength - BulletCoreLength;
	BulletCoreAPoint.nXY[1] = f.y() - BulletCoreRadius;
	BulletCoreAPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreAPoint);


	PixelPos BulletCoreBPoint;//��о2
	BulletCoreBPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength - BulletCoreLength;
	BulletCoreBPoint.nXY[1] = f.y() + BulletCoreRadius;
	BulletCoreBPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreBPoint);


	PixelPos BulletCoreCPoint;//��о3
	BulletCoreCPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletCoreCPoint.nXY[1] = f.y() + BulletCoreRadius;
	BulletCoreCPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreCPoint);


	PixelPos BulletFuseDPoint;//���Ĺܵ�4
	BulletFuseDPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseDPoint.nXY[1] = f.y() + BulletFuseRadius;
	BulletFuseDPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseDPoint);


	PixelPos BulletFuseEPoint;//���Ĺܵ�5
	BulletFuseEPoint.nXY[0] = f.x() - FrontWidth;
	BulletFuseEPoint.nXY[1] = f.y() + BulletFuseRadius;
	BulletFuseEPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseEPoint);

	vPlaneDraw.push_back(BulletCore);

	return 1;
}
