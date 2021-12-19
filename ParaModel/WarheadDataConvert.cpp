
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

// 得到战斗部各参数
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

// 拓扑结构数据转为现实数据 只考虑矩形截面
int WarheadDataConvert::CaArmHeadData(VSHAPE& vOriginShape)
{
	//unitType 构建类型 翻译如下 1-矩形 2-圆形 3-多边形 4-线段
	vOriginShape.clear(); 
	
	//战斗部宽度为 弹体上或下边长+前盖厚度+后盖厚度

	//前盖 按矩形绘制 
	SimpleShape Front;
	Front.nWH[0] = fT[8];
	Front.nWH[1] = fR[8] * 2;
	// 按矩形计算  先原始坐标 再居中坐标 下同
	Front.unitType = 1;
	Front.unitIdx = 1;
	Front.nCen[0] = - fT[8] / 2;
	Front.nCen[1] = 0 ;
	vOriginShape.push_back(Front); 

	//后盖 按矩形绘制
	SimpleShape oBack;
	oBack.nWH[0] = fT[9];
	oBack.nWH[1] = fR[9]*2;
	oBack.unitType = 1;
	oBack.unitIdx = 2;
	oBack.nCen[0] = - fT[9] / 2 - fH[1] - fT[8];
	oBack.nCen[1] = 0;
	vOriginShape.push_back(oBack);


#pragma region 外壳
	//外壳记录了拐点，根据拐点绘制线 视觉下方壳线 数据为大
	SimpleShape ShellShapeB;
	ShellShapeB.unitType = 3;
	ShellShapeB.unitIdx = 3;
	float ShellWidth = fT[1];
	ShellShapeB.nWH[0] = ShellWidth;
	// 先设为1 调试用
// 	ShellShapeB.nWH[0] = 1;
	//根据外壳拐点绘制底部
	for (size_t i = 0; i < vArmHeadTopo.vTurnPoint.size(); i++)
	{
		PixelPos PointB;//底部外壳
		PointB.nXY[0] = f.x() - vArmHeadTopo.vTurnPoint[i].x();
		PointB.nXY[1] = f.y() + vArmHeadTopo.vTurnPoint[i].y();
		ShellShapeB.vCorner.push_back(PointB);
	}
	vPlaneDraw.push_back(ShellShapeB);

	// 第一次需要插入前后端点
	if ( bFirst )
	{
		// 原先的值变为负值
		int nPtNum = vCtrlPts.size(); 
		for (int i = 0; i < nPtNum; i++)
	{
			vCtrlPts[i].setX(-vCtrlPts[i].x()); 
	}
		QPoint ptB1(-fT[8],fR[1]-fT[1]/2); 
		// 添加前盖连接点
		vCtrlPts.insert(0, ptB1); 
		// 添加后盖连接点
		QPoint ptB2(-fT[8] - fH[1],fR[1] - fT[1] / 2);
		vCtrlPts.push_back(ptB2); 

		bFirst = false; 
	}
	// 后面操作只需要改变值
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
	// 先占位 后续修改数据为 曲线数据
	// vOriginShape[2] 视觉下方 数据大 的外壳
	vOriginShape.push_back(ShellShapeB);
	ShellShapeB.unitIdx = 4;
	// vOriginShape[3] 视觉下方 数据大 的内衬
	vOriginShape.push_back(ShellShapeB);
	// vOriginShape[4] 视觉下方 数据小 的内衬
	vOriginShape.push_back(ShellShapeB);
	// vOriginShape[5] 视觉下方 数据小 的外壳
	ShellShapeB.unitIdx = 3;
	vOriginShape.push_back(ShellShapeB);

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
	// vOriginShape[6] 弹药外轮廓
	ShellShapeB.unitIdx = 5;
	vOriginShape.push_back(ShellShapeB);

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
	BulletCore.unitIdx = 7; 
	BulletCore.nWH[0] = fT[5];

	PixelPos BulletFuseAPoint;//中心管点1  
	BulletFuseAPoint.nXY[0] = -fT[8];
	BulletFuseAPoint.nXY[1] = -fR[4] + fT[4]/2;
	BulletFuseAPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseAPoint);


	PixelPos BulletFuseBPoint;//中心管点2
	BulletFuseBPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseBPoint.nXY[1] = -fR[4] + fT[4]/2 ;
	BulletFuseBPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseBPoint);


	PixelPos BulletFuseCPoint;//中心管点3
	BulletFuseCPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseCPoint.nXY[1] = -fR[5] + fT[5]/2 ;
	BulletFuseCPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseCPoint);


	PixelPos BulletCoreAPoint;//弹芯1
	BulletCoreAPoint.nXY[0] = -fT[8] - fH[4] - fH[5];
	BulletCoreAPoint.nXY[1] = -fR[5] + fT[5]/2 ;
	BulletCoreAPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreAPoint);


	PixelPos BulletCoreBPoint;//弹芯2
	BulletCoreBPoint.nXY[0] = -fT[8] - fH[4] - fH[5];
	BulletCoreBPoint.nXY[1] = fR[5] - fT[5]/2 ;
	BulletCoreBPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreBPoint);


	PixelPos BulletCoreCPoint;//弹芯3
	BulletCoreCPoint.nXY[0] = -fT[8] - fH[4];
	BulletCoreCPoint.nXY[1] = fR[5]- fT[5]/2;
	BulletCoreCPoint.nLineWidth = fT[5];
	BulletCore.vCorner.push_back(BulletCoreCPoint);


	PixelPos BulletFuseDPoint;//中心管点4
	BulletFuseDPoint.nXY[0] = -fT[8] - fH[4];
	BulletFuseDPoint.nXY[1] = fR[4]- fT[4]/2;
	BulletFuseDPoint.nLineWidth = fT[4];
	BulletCore.vCorner.push_back(BulletFuseDPoint);


	PixelPos BulletFuseEPoint;//中心管点5
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
