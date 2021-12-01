#pragma once

// ��������ģ ģ�� ���ݽṹ����
#include <vector>
using namespace std;

typedef vector<int> VINT;
typedef vector<float> VFLOAT;

// ����ƽ���
typedef struct PixelPos
{
	int nXY[2];
}PixelPos;
typedef vector<PixelPos> VPIXELPOS;

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
	int nUnitType;				// ������Ԫ���� ������ǽ�Ŵ�
	GeoShape oShape;			// ������������
	MatInfo  oMaterial;			// ��ɲ�������
}BasicUnit;
typedef vector<BasicUnit> VUNITTABLE;

// �������˵�Ԫ
typedef struct TopoUnit
{
	int nTopoIdx;				// �������
	int nUnitType;				// ���Ĺ�����Ԫ���� ������ǽ�Ŵ�
	int nCenUnitIdx;			// ���Ĺ�����ԪId
	int nAdjUnitIdx[10];			// ǰ�����ҷ��� �����±�ʾ��ǽ ÿ����������� �ڽӵ�ԪId ���ڽ�-1
	int nEdgeType;				// �߽����� ǰ���������� �����޽ǶȻ��� 45�� ����Ϊ1 ����Ϊ6
	int nStatusFlag;			// ��־0 1 2 ���� ɾ�� ����
	int nUnitAngle;				// 0 90�� ����
	int nCenPos[4];				// ����������Ԫ���������µ� ���ϵ�߶�  Yֵ��ʾ�߶�
}TopoUnit;
typedef vector<TopoUnit> VTOPOTABLE;
typedef vector<TopoUnit> VLAYERTOPO;
typedef vector<VLAYERTOPO> VBUILDTOPO;

// ���˵�Ԫ��Ӧ��ʵ�ʿռ�����
typedef struct UnitData
{
	Ver3D oData[8];
}UnitData;
typedef vector<UnitData> VUNITDATA;

// ���ϵͳģ���Ĵ洢��ʽ IO������

// ����ϵͳģ���� �������Ͷ�Ӧ�����˱��

// ��Ƶ�����ͼ�Ļ���ģ������ƶ� �༭ ����

// ��ƶ����ͼ�Ļ���ģ������ƶ� �༭ ����

// ����֮��ĺϹ��Լ��
 
// ϵͳ·����Ϣ
typedef struct SysPath
{
	string sExePath;			// ��ִ���ļ�����·�� ��.exe
	string sExeDir;				// ��ִ���ļ�����Ŀ¼����·��
	string sProcLibDir;			// �㷨��·��
	string sTmpDir;				// ��ʱ����·��
	string sModelLibDir;		// ģ��·��
	string sBoomLibDir;			// ���ļ�·��
	string sParaLibDir;			// �������ϲ���·��
}SysPath;