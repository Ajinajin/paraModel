#pragma once

// ��������ģ ģ�� ���ݽṹ����
#include <vector>
#include <QtCore/qstring.h>
#include <QPoint>
#include <QList>
#include <QVector>
using namespace std;

typedef vector<int> VINT;
typedef vector<VINT> VVINT;
typedef vector<float> VFLOAT;

typedef QList<QPointF> LISTPT;
typedef QVector<LISTPT> VECLSTPT;

// ����ƽ���
typedef struct PixelPos
{
	int nXY[2];
	int nLineWidth;
}PixelPos;
typedef vector<PixelPos> VPIXELPOS;

// ƽ��ͼ���弸������ ��������
typedef struct SimpleShape
{
	int nCen[2];			// ���ĵ�
	int nWH[2];				// ���� XY��Χ
	int unitType;
	int unitIdx;
	VPIXELPOS vCorner;
	QList<QPoint> lstCtrlPts;
}SimpleShape;
typedef vector<SimpleShape> VSHAPE	;
// ƽ��ͼ���弸������ ��������
typedef struct EdgeShape
{
	int nCen[2];			// ���ĵ�
	int nWH[2];				// ���� XY��Χ
	int unitType;			// 0 1 2 
	int unitIdx;
	QList<QPointF> lstPts;
}EdgeShape;
typedef vector<EdgeShape> VEDGESHAPE	;




// �ռ���ά��
typedef struct Ver3D
{
	int nIdx;
	float fXYZ[3];			//�ռ�λ�� YΪ�ռ�߶�
}Ver3D;
typedef vector<Ver3D> V3DPT;
typedef vector<V3DPT> VV3DPT;

typedef struct GeoShape
{
	int nShapeType;				// ��״���� �� Բ �����
	int nThickNess;				// ǽ�Ͱ�ĺ��
	int nShapeRange[4];			// ������������ ��С����
	int nCen[2];				// ���ĵ� Բ�ĵ�
	int nNumOrRadius;			// Բ�뾶 ���� ����ε�����
	QString nShapeName;				// ��������
	VINT vPolyPt;				// ����αպ϶��㼯 ��ʱ������ ��β���غ�
}GeoShape;

typedef struct MatInfo
{
	int MatType;				// ��������
	int nMatPicType;			// ������ͼ����
	VFLOAT vMatPara;			// ��������
}MatInfo;

// ����������Ԫ
typedef struct BasicUnit
{
	int nUnitIdx;				// ����������ԪIdx
	int nUnitType;				// ������Ԫ���� ��������ǽ�Ŵ�
	GeoShape oShape;			// ������������
	MatInfo  oMaterial;			// ��ɲ�������
}BasicUnit;
typedef vector<BasicUnit> VUNITTABLE;

// �������˵�Ԫ
typedef struct TopoUnit
{
	int nTopoIdx;				// �������
	int nUnitType;				// ���Ĺ�����Ԫ���� ��������ǽ�Ŵ�
	int nCenUnitIdx;			// ���Ĺ�����ԪId
	int nAdjUnitIdx[12];		// ǰ�����ҷ��� �����±�ʾ��ǽ ÿ����������� �ڽӵ�ԪId ���ڽ�-1
	int nEdgeType;				// �߽����� ǰ���������� �����޽ǶȻ��� 45�� ����Ϊ1 ����Ϊ6
	int nStatusFlag;			// ��־0 1 2 ���� ɾ�� ����
	int nUnitAngle;				// 0 90�� ���� ����������չΪ 45�� ����Ƕ�
	int nCenPos[4];				// ����������Ԫ���������µ� ���ϵ�߶�  Yֵ��ʾ�߶�
}TopoUnit;


// ¥�����˵�Ԫ
typedef struct LayerUnit
{
	QString sLayerName;			// ¥������
	QString sVersion;			// �汾��
	QString sIcon;				// ¥��汾�� 
	vector<TopoUnit> vLayerTopo;	// ¥������
}LayerUnit;

typedef vector<TopoUnit> VTOPOTABLE;
typedef vector<LayerUnit> VBUILDTOPO;

// ���彨����Ϣ�����ṹ
typedef struct BuildProj
{
	float fVersion;					// �ṹ�汾 �ݶ�1.0
	int nBuildIdx;					// ����Idx
	int nTypeIdx;					// ��������Idx ����
	int nCalDrawData;				// �������ݼ����־ 0 δ���� vPlaneDraw������
	VINT vPlaneTopoIdx;				// ����ͼӦ��¥��ID
	VINT vLayerHigh;				// ¥�㶥��߶� �ӵ�һ��������� ����=¥��+1
	LayerUnit vPlaneTopo;			// ¥������ͼ �����ж�������
	VSHAPE     vPlaneDraw;			// ¥����弸������ ��Ӧ����ͼ ����
	QString sBuildName;				// ��������
	QString sProjFileName;			// �����ļ�����
	QString sModel3DFileName;		// ��Ӧ����άģ������ Ŀǰ�ݶ�K�ļ���ʽ
}BuildProj;

// ���˵�Ԫ��Ӧ��ʵ�ʿռ�����
typedef struct UnitData
{
	Ver3D oData[8];
}UnitData;
typedef vector<UnitData> VUNITDATA;

// ���ϵͳģ���Ĵ洢��ʽ IO������

// ����ϵͳģ���� ��������Ͷ�Ӧ�����˱���

// ��Ƶ�����ͼ�Ļ���ģ������ƶ� �༭ ����

// ��ƶ����ͼ�Ļ���ģ������ƶ� �༭ ����

// ����֮��ĺϹ��Լ��

// ϵͳ·����Ϣ
typedef struct SysPath
{
	string sExePath;			// ��ִ���ļ�����·�� ��.exe
	string sExeDir;				// ��ִ���ļ�����Ŀ¼����·��
	string sTopoUnitDir;		// ϵͳ���˹�����·��
	string sTopoLayerDir;		// ϵͳƽ��ͼ��·��
	string sWarheadDir;			// ϵͳս����·��


	string sProcLibDir;			// �㷨��·��
	string sTmpDir;				// ��ʱ����·��
	string sModelLibDir;		// ģ��·��
	string sBoomLibDir;			// ���ļ�·��
	string sParaLibDir;			// �������ϲ���·��
}SysPath;

// ս������������ݽṹ 
typedef struct PARADES
{
	float 	nUnitPropty;			// ��������
	QString sUnitName;				// ��������
}PARADES;

// ��Ƭ����
typedef struct FragDes
{
	int nFragType;					// ��Ƭ���� 0 1 2 ���� ���� Բ����
	float fFragPara[3];				// ����Ϊ�뾶 ����Ϊ������ Բ����Ϊ�뾶�ͳ���
}FragDes;

// ��Ǳ�������
typedef struct EdgeDes
{
	int nEdgeType;					// 012 �ֱ��Ӧֱ�߶� ����Բ�� �����Bezier���߶�
	VINT vCtrlPtIdx;				// ���Ƶ�Id
	QList<QPointF> lstCtrlPts;		// ���߶�Ӧ�Ŀ��Ƶ�
}EdgeDes;
typedef QList<EdgeDes> LSTEDGEDES;

// ս�����������˽ṹ �����������
typedef struct ArmHeadTopo
{
	int nArmHeadIdx;				// ս������������
	int nArrangeType;				// ��Ƭ�Ų����� 0 Ĭ�������Ų� 1 �����Ų�
	int nFragNum;					// ��Ƭ����
	FragDes oFragInfo;				// ��Ƭ��Ϣ 0 1 2 ���� ���� Բ����
	QList<QPointF> vCtrlPts;		// ��ǿ��ƹյ�
	LSTEDGEDES lstEdgeDes;			// ��Ǹ�����Ϣ
	VINT vFragGesture;				// ��Ƭ��̬ ����Ϊ1ʱ��ʾ������Ƭһ����̬
	VFLOAT vFragCenPos;				// ÿ����Ƭ���ĵ�λ�ú���̬�Ƕ� ������6�� ����1�� Բ����3��
	vector<PARADES> mapArmHead;		// ս�������� R1 ���Զ�
	QString sArmHeadName;			// ���� ���ͺͶ��󲻷� ����Ϊ����ս�����༭
	QString sArmHeadVersion;		// ���� ���ͺͶ��󲻷� ����Ϊ����ս�����༭
}ArmHeadTopo;
typedef vector<ArmHeadTopo> VARMHEAD;