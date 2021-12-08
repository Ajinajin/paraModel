
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

// 拓扑结构数据转为现实数据 只考虑矩形截面
int WarheadDataConvert::CalPlaneData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vPlaneDraw)
{
	//unitType 构建类型 翻译如下 1-矩形 2-圆形 3-多边形 4-线
	//清空画布数据
	vPlaneDraw.clear();
	//设坐标原点为 200,200
	QPointF f = QPointF(600, 300);
	//战斗部宽度为 弹体上或下边长+前盖厚度+后盖厚度

	//前盖为 厚度25 绘制宽25 高 前盖半径*2 中心点坐标为 1000-25/2,1000
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

	//后盖为 厚度25 绘制宽25 高 后盖半径*2 中心点坐标为 1000-25/2-弹体上或下边长-前盖厚度 /2,1000
	SimpleShape Break;
	float BreakWidth = FindMapArmHead(vArmHeadTopo, "T9").nUnitPropty;
	float BreakHeight = FindMapArmHead(vArmHeadTopo, "R9").nUnitPropty * 2;
	float BulletLength = FindMapArmHead(vArmHeadTopo, "H1").nUnitPropty; ///弹体上边长
	Break.nWH[0] = BreakWidth;
	Break.nWH[1] = BreakHeight;
	Break.unitType = 1;
	Break.unitIdx = 9;
	Break.nCen[0] = f.x() - BreakWidth / 2 - BulletLength - FrontWidth;
	Break.nCen[1] = f.y();
	vPlaneDraw.push_back(Break);


#pragma region 外壳
	//外壳记录了拐点，根据拐点绘制线
	SimpleShape ShellShapeB;
	ShellShapeB.unitType = 4;
	ShellShapeB.unitIdx = 11;
	float ShellWidth = FindMapArmHead(vArmHeadTopo, "Shell").nUnitPropty;
	ShellShapeB.nWH[0] = ShellWidth;
	//根据外壳拐点绘制底部
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//底部外壳
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
		PixelPos PointT;//顶部外壳
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(ShellShapeT);
#pragma endregion




#pragma region 破片
	//外壳记录了拐点，根据拐点及外壳厚度计算好的值生成破片位置
	float FragmentWidth = FindMapArmHead(vArmHeadTopo, "Fragment").nUnitPropty;
	SimpleShape FragmentShapeB;
	FragmentShapeB.nWH[0] = FragmentWidth;
	FragmentShapeB.unitType = 4;
	FragmentShapeB.unitIdx = 12;
	//根据外壳拐点绘制
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//底部
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
		PixelPos PointT;//顶部
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y() + ShellWidth;
		FragmentShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(FragmentShapeT);
#pragma endregion





#pragma region 内衬
	//外壳记录了拐点，根据拐点及外壳厚度、破片厚度计算好的值生成破片位置
	float LiningWidth = FindMapArmHead(vArmHeadTopo, "Lining").nUnitPropty;

	SimpleShape LiningShapeB;
	LiningShapeB.nWH[0] = LiningWidth;
	LiningShapeB.unitType = 4;
	LiningShapeB.unitIdx = 13;
	//根据外壳拐点绘制
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//底部
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
		PixelPos PointT;//顶部
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[i].y() + ShellWidth + FragmentWidth;
		LiningShapeT.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(LiningShapeT);
#pragma endregion



#pragma region 弹药
	//外壳记录了拐点，根据拐点及外壳厚度、破片厚度、内衬厚度计算好的值生成破片位置
	SimpleShape AmmunitionShape;
	AmmunitionShape.unitType = 4;
	AmmunitionShape.unitIdx = 7;
	AmmunitionShape.nWH[0] = 1;
	//根据外壳拐点绘制
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//底部
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y() - ShellWidth - FragmentWidth - LiningWidth;
		AmmunitionShape.vCorner.push_back(PointB);
	}
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{

		PixelPos PointT;//顶部
		PointT.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[vArmHeadTopo.vTurnPoint.size()-i-1].x();
		PointT.nXY[1] = f.y() - vArmHeadTopo.vTurnPoint[vArmHeadTopo.vTurnPoint.size()-i-1].y() + ShellWidth + FragmentWidth + LiningWidth;
		AmmunitionShape.vCorner.push_back(PointT);
	}
	vPlaneDraw.push_back(AmmunitionShape);

#pragma endregion


	//弹芯
	SimpleShape BulletCore;
	float BulletCoreWidth = FindMapArmHead(vArmHeadTopo, "T5").nUnitPropty; //弹芯厚度T5
	float BulletCoreRadius = FindMapArmHead(vArmHeadTopo, "R5").nUnitPropty;//弹芯半径R5
	float BulletCoreLength = FindMapArmHead(vArmHeadTopo, "H5").nUnitPropty;//弹芯长H5 

	float BulletFuseRadius = FindMapArmHead(vArmHeadTopo, "R4").nUnitPropty;//引信半径R4 
	float BulletFuseLength = FindMapArmHead(vArmHeadTopo, "H4").nUnitPropty;//中心管长H4
	float BulletFuseWidth = FindMapArmHead(vArmHeadTopo, "T4").nUnitPropty;//中心管厚度T4

	BulletCore.unitType = 4;
	BulletCore.unitIdx = 4; 
	BulletCore.nWH[0] = BulletCoreWidth;

	PixelPos BulletFuseAPoint;//中心管点1  
	BulletFuseAPoint.nXY[0] = f.x() - FrontWidth;
	BulletFuseAPoint.nXY[1] = f.y() - BulletFuseRadius;
	BulletFuseAPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseAPoint);


	PixelPos BulletFuseBPoint;//中心管点2
	BulletFuseBPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseBPoint.nXY[1] = f.y() - BulletFuseRadius;
	BulletFuseBPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseBPoint);


	PixelPos BulletFuseCPoint;//中心管点3
	BulletFuseCPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseCPoint.nXY[1] = f.y() - BulletCoreRadius;
	BulletFuseCPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseCPoint);


	PixelPos BulletCoreAPoint;//弹芯1
	BulletCoreAPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength - BulletCoreLength;
	BulletCoreAPoint.nXY[1] = f.y() - BulletCoreRadius;
	BulletCoreAPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreAPoint);


	PixelPos BulletCoreBPoint;//弹芯2
	BulletCoreBPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength - BulletCoreLength;
	BulletCoreBPoint.nXY[1] = f.y() + BulletCoreRadius;
	BulletCoreBPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreBPoint);


	PixelPos BulletCoreCPoint;//弹芯3
	BulletCoreCPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletCoreCPoint.nXY[1] = f.y() + BulletCoreRadius;
	BulletCoreCPoint.nLineWidth = BulletCoreWidth;
	BulletCore.vCorner.push_back(BulletCoreCPoint);


	PixelPos BulletFuseDPoint;//中心管点4
	BulletFuseDPoint.nXY[0] = f.x() - FrontWidth - BulletFuseLength;
	BulletFuseDPoint.nXY[1] = f.y() + BulletFuseRadius;
	BulletFuseDPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseDPoint);


	PixelPos BulletFuseEPoint;//中心管点5
	BulletFuseEPoint.nXY[0] = f.x() - FrontWidth;
	BulletFuseEPoint.nXY[1] = f.y() + BulletFuseRadius;
	BulletFuseEPoint.nLineWidth = BulletFuseWidth;
	BulletCore.vCorner.push_back(BulletFuseEPoint);

	vPlaneDraw.push_back(BulletCore);

	return 1;
}
