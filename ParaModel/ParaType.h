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
	int nShapeType;				// ��״���� ���� 1 Բ 2 �����3
	int nThickNess;				// ǽ�Ͱ�ĺ��
	int nShapeRange[4];			// ������������ ��С����
	int nCen[2];				// ���ĵ� Բ�ĵ�
	int nNumOrRadius;			// Բ�뾶 ���� ����ε�����
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
	int nUnitType;				// ������Ԫ���� ��1  ��2 ��3 ǽ4 ��5 ��6
	GeoShape oShape;			// ������������
	MatInfo  oMaterial;			// ��ɲ�������
}BasicUnit;
typedef vector<BasicUnit> VUNITTABLE; 

// �������˵�Ԫ
typedef struct TopoUnit
{
	int nTopoType;				// ���Ĺ�����Ԫ���� ������ǽ�Ŵ�
	int nCenUnitIdx;			// ���Ĺ�����ԪId
	int nAdjUnitIdx[12];			// ǰ���������� �ڽӵ�ԪId ���ڽ�-1
	int nEdgeType;				// �߽����� ǰ����������
	Ver3D oUnitSE[2];			// ����������Ԫ����ʼ�����ֹ�� Yֵ��ʾ�߶ȷ�Χ �п���Ϊ���ǽ ����������
}TopoUnit;
typedef vector<TopoUnit> VTOPOTABLE;

// ���ϵͳģ���Ĵ洢��ʽ IO������

// ����ϵͳģ���� �������Ͷ�Ӧ�����˱��

// ��Ƶ�����ͼ�Ļ���ģ������ƶ� �༭ ����

// ��ƶ����ͼ�Ļ���ģ������ƶ� �༭ ����

// ����֮��ĺϹ��Լ��

