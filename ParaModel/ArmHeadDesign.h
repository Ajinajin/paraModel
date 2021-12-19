#pragma once

#include "ParaType.h"

#include <QObject>

class CArmHeadDesign : public QObject
{
public:
	CArmHeadDesign();
	CArmHeadDesign(ArmHeadTopo oData);
	~CArmHeadDesign();
// 战斗部描述参数 均从下标1开始试用 对应图纸中相关尺寸 单位为mm
private:
	float fR[20];						// 设计图纸中fR参数
	float fT[10];						// 设计图纸中fT参数
	float fH[10]; 						// 设计图纸中fH参数
// 设计数据 相对坐标
public:
	LISTPT lstUpDesignCtrlPts;			// 上方外壳特征点/拐点 屏幕显示坐标
	ArmHeadTopo oDesignData;			// 设计图纸数据
// 绘制数据 屏幕坐标
public:
	QList<QPointF> lstUpDrawCtrlPts;	// 上方外壳特征点/拐点 屏幕显示坐标
	VEDGESHAPE vDrawShape;			// 整个战斗部外形绘制坐标尺寸  曲线计算完成
	VEDGESHAPE vDesignShape;		// 战斗部外形原始坐标尺寸  曲线为控制点表示
	LISTPT lstUpCurvePts;			// 外壳曲线上的点
	VECLSTPT vlstCurvePt;			// 0-7 数据从大到小8条曲线上的点
	VECLSTPT vlstPartPt;			// 0-4 5个区域轮廓点集
	VECLSTPT vlstDrawPartPt;		// 0-4 5个区域轮廓点集
	LISTPT   lstLinkFrontPt;		// 和前盖连接点
	LISTPT   lstLinkBackPt;			// 和后盖连接点

// 数据读写功能
public:
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);
	// 基于图纸的战斗部各参数
	int GetArmHeadPara(ArmHeadTopo& vArmHeadTopo);

	int ReadArmHeadInfo(QString sFile); 
	int WriteArmHeadInfo(QString sFile); 
// 数据编辑功能 RTH参数编辑
public:
	bool bParaCanEdit;							// 参数是否可编辑
	int ModifyRPara(float fNewR,int nRIdx);		// 编辑R参数
	int ModifyHPara(float fNewT,int nTIdx);		// 编辑T参数
	int ModifyTPara(float fNewH,int nHIdx);		// 编辑H参数
// 外壳编辑功能
public:
	bool bEdgeCanEdit;					// 外壳是否可编辑
	VINT vCanEditFlag;					// 外壳控制点可编辑标志 0 不可编辑 1 可编辑
	// 默认折线和圆弧不可编辑 只可交互式修改曲线段
	int AddCtrlPts(QPoint oAddPt, int nInsIdx); 
	int DelCtrlPts(int nDelIdx); 
private:

};

