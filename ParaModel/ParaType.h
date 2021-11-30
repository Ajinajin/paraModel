#pragma once

// 参数化建模 模块 数据结构定义
#include <vector>
using namespace std;

typedef vector<int> VINT; 
typedef vector<float> VFLOAT;
 
// 整型平面点
typedef struct PixelPos
{
	int nXY[2];
}PixelPos;
typedef vector<PixelPos> VPIXELPOS;

// 空间三维点
typedef struct Ver3D
{
	int nIdx;
	float fXYZ[3];			//空间位置 Y为空间高度
}Ver3D;
typedef vector<Ver3D> V3DPT;
typedef vector<V3DPT> VV3DPT;

typedef struct GeoShape
{
	int nShapeType;				// 形状类型 矩形 1 圆 2 多边形3
	int nThickNess;				// 墙和板的厚度
	int nShapeRange[4];			// 外框的左下右上 从小到大
	int nCen[2];				// 中心点 圆心点
	int nNumOrRadius;			// 圆半径 或者 多边形点数量
	VINT vPolyPt;				// 多边形闭合顶点集 逆时针排列 首尾点重合
}GeoShape;

typedef struct MatInfo
{
	int MatType;				// 材质类型
	int nMatPicType;			// 材质贴图类型
	VFLOAT vMatPara;			// 材质属性
}MatInfo;

// 基本构件单元
typedef struct BasicUnit
{
	int nUnitIdx;				// 基本构件单元Idx
	int nUnitType;				// 基本单元类型 柱1  梁2 板3 墙4 门5 窗6
	GeoShape oShape;			// 外形轮廓描述
	MatInfo  oMaterial;			// 组成材质描述
}BasicUnit;
typedef vector<BasicUnit> VUNITTABLE; 

// 基本拓扑单元
typedef struct TopoUnit
{
	int nTopoType;				// 中心构件单元类型 柱梁板墙门窗
	int nCenUnitIdx;			// 中心构件单元Id
	int nAdjUnitIdx[12];			// 前后上下左右 邻接单元Id 无邻接-1
	int nEdgeType;				// 边界类型 前后上下左右
	Ver3D oUnitSE[2];			// 基本构件单元的起始点和终止点 Y值表示高度范围 有可能为半高墙 或者悬空梁
}TopoUnit;
typedef vector<TopoUnit> VTOPOTABLE;

// 设计系统模板库的存储方式 IO操作库

// 构建系统模板库的 构件表格和对应的拓扑表格

// 设计单类视图的基本模板关联移动 编辑 功能

// 设计多个视图的基本模板关联移动 编辑 功能

// 属性之间的合规性检查

