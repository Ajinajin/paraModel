#include "WarheadOglManager.h"
#include <QPalette>

#include <QDebug>

#include <fstream>
#include <set>
#include <malloc.h>
#include <iomanip>





// ����ȫ�ֱ��� �����޸�
//(�����Z����Ϊ0���ܿ�����������)
const QVector3D CAMERA_POSITION(0.0f, 400.0, 1000);
//const QVector3D CAMERA_POSITION(0.0f, 5.0f, 10.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

const float Pi = 3.141592654;
vector<VFLOAT> res;
int initFlag = 0;
fstream outfile("D:/Study/Work/HS/Ajinajin/paraModel/x64/Debug/outtest.txt", fstream::out);



//����������ߵ�ȫ�ֱ���
extern VECLSTPT oglCurveData;


//*********************�����Ժ���***********************************

//��ȡ��Ӧ����ID
int getUnitIdx(ArmHeadTopo* oglWarhead, QString str)
{
	for (int i = 0; i < oglWarhead->mapArmHead.size(); i++)
	{
		if (oglWarhead->mapArmHead[i].sUnitName == str) { return oglWarhead->mapArmHead[i].nUnitIndex; }
	}
	return -1;
}


//���� Բ��	Բ����Բ�뾶	Բ��СԲ�뾶	��뾶	Բ���߶�

vector<VFLOAT> getFragSphere(Ver3D center, float R1, float R2, float r, float h)
{
	vector<VFLOAT> finalRes;
	//��������ϵ
	//��ż������С���Բ������
	float resX;//X������
	float resY;//Y������
	float resZ;//Z����Ļ����

	//չʾ����
	if ((R1 - R2) / 4 > r)
	{
		//�����������
		VFLOAT Res1;//��1��
		VFLOAT Res2;//��2��
		VFLOAT Res3;//��3��
		int floor = 0;//����
		float floorheight = pow(3, 0.5) * r;//���
		floor = (h - r + pow(3, 0.5) * r / 2) / floorheight;

		//C��һ���м����
		float C = r / (R2 + r);

		//�������ÿһ�����װ�¶��ٸ�С��
		int num = 0;

		//half_a�ǽ�����Բ��һ��Բ��ռ�Ļ���
		float halfBeta = asin(C);
		//an�ǽ�����Բ������Բ��ռ�Ļ���
		float Beta = halfBeta * 2;
		//ÿ��ṹ��Ƭ�������
		num = 2 * Pi / Beta;
		//�����������
		if (((2 * ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(halfBeta) - (R2 + r)) + r) < R1)
		{
			for (int f = 0; f < floor; f++)
			{
				if (f % 2 == 0)
				{
					for (int i = 0; i < num; i++)
					{
						//��С����Բ�ļнǣ����ȣ�
						float angle1 = i * Beta;
						float angle2 = i * Beta + halfBeta;
						//��1��
						resX = center.fXYZ[0] + (R2 + r) * cos(angle1);
						resZ = center.fXYZ[2] + (R2 + r) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res1.push_back(resX);
						Res1.push_back(resY);
						Res1.push_back(resZ);
						//��2��
						resX = center.fXYZ[0] + ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(angle2);
						resZ = center.fXYZ[2] + ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res2.push_back(resX);
						Res2.push_back(resY);
						Res2.push_back(resZ);
						//��3��
						resX = center.fXYZ[0] + (2 * ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(halfBeta) - (R2 + r)) * cos(angle1);
						resZ = center.fXYZ[2] + (2 * ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(halfBeta) - (R2 + r)) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res3.push_back(resX);
						Res3.push_back(resY);
						Res3.push_back(resZ);
					}
				}
				else
				{
					for (int i = 0; i < num; i++)
					{
						//��С����Բ�ļнǣ����ȣ�
						float angle1 = i * Beta;
						float angle2 = i * Beta + halfBeta;
						//��1��
						resX = center.fXYZ[0] + (R2 + r) * cos(angle2);
						resZ = center.fXYZ[2] + (R2 + r) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res1.push_back(resX);
						Res1.push_back(resY);
						Res1.push_back(resZ);
						//��2��
						resX = center.fXYZ[0] + ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(angle1);
						resZ = center.fXYZ[2] + ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res2.push_back(resX);
						Res2.push_back(resY);
						Res2.push_back(resZ);
						//��3��
						resX = center.fXYZ[0] + (2 * ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(halfBeta) - (R2 + r)) * cos(angle2);
						resZ = center.fXYZ[2] + (2 * ((R2 + r) * cos(halfBeta) + pow(3, 0.5) * r) * cos(halfBeta) - (R2 + r)) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res3.push_back(resX);
						Res3.push_back(resY);
						Res3.push_back(resZ);
					}
				}
			}
			finalRes.push_back(Res1);
			finalRes.push_back(Res2);
			finalRes.push_back(Res3);
			return finalRes;
		}
		else
		{
			//C��һ���м����
			float C1 = (R1 - R2) / 2 - r;
			float C2 = pow((4 * r * r - C1 * C1), 0.5);
			floor = 0;//����
			floorheight = C1;//���
			floor = (h - C1 + r) / floorheight;
			//�������ÿһ�����װ�¶��ٸ�С��
			int num = 0;
			//half_a�ǽ�����Բ��һ��Բ��ռ�Ļ���
			float halfBeta = atan(C2 / (R2 + r + C1));
			//an�ǽ�����Բ������Բ��ռ�Ļ���
			float Beta = halfBeta * 2;
			num = 2 * Pi / Beta;
			for (int f = 0; f < floor; f++)
			{
				if (f % 2 == 0)
				{
					for (int i = 0; i < num; i++)
					{
						//��С����Բ�ļнǣ����ȣ�
						float angle1 = i * Beta;
						float angle2 = i * Beta + halfBeta;
						//��1��
						resX = center.fXYZ[0] + (R2 + r) * cos(angle1);
						resZ = center.fXYZ[2] + (R2 + r) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res1.push_back(resX);
						Res1.push_back(resY);
						Res1.push_back(resZ);
						//��2��
						resX = center.fXYZ[0] + (R2 + r + C1) * cos(angle2);
						resZ = center.fXYZ[2] + (R2 + r + C1) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res2.push_back(resX);
						Res2.push_back(resY);
						Res2.push_back(resZ);
						//��3��
						resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle1);
						resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res3.push_back(resX);
						Res3.push_back(resY);
						Res3.push_back(resZ);
					}
				}
				else
				{
					for (int i = 0; i < num; i++)
					{
						//��С����Բ�ļнǣ����ȣ�
						float angle1 = i * Beta;
						float angle2 = i * Beta + halfBeta;
						//��1��
						resX = center.fXYZ[0] + (R2 + r) * cos(angle2);
						resZ = center.fXYZ[2] + (R2 + r) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res1.push_back(resX);
						Res1.push_back(resY);
						Res1.push_back(resZ);
						//��2��
						resX = center.fXYZ[0] + (R2 + r + C1) * cos(angle1);
						resZ = center.fXYZ[2] + (R2 + r + C1) * sin(angle1);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res2.push_back(resX);
						Res2.push_back(resY);
						Res2.push_back(resZ);
						//��3��
						resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle2);
						resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle2);
						resY = r + center.fXYZ[1] + f * floorheight;
						Res3.push_back(resX);
						Res3.push_back(resY);
						Res3.push_back(resZ);
					}
				}
			}
			finalRes.push_back(Res1);
			finalRes.push_back(Res2);
			finalRes.push_back(Res3);
			return finalRes;
		}
	}

	//չʾ����
	else if ((R1 - R2) / 3 > r)
	{
		VFLOAT InternalRes;//�ڲ���
		VFLOAT ExternalRes;//�ⲿ��
		int floor = 0;//����
		float floorheight = 2 * r * sin(acos((R1 - R2 - 2 * r) / 2 * r));//���
		floor = (h - (2 * r - floorheight)) / floorheight;
		//C��һ���м����
		float C = ((R2 + r) * (R2 + r) + (R1 - r) * (R1 - r) - 4 * r * r) / (2 * (R2 + r) * (R1 - r));
		//�������ÿһ�����װ�¶��ٸ�С��
		int num = 0;
		//Beta��ÿ������ռ�õĻ���
		float Beta = acos(C);
		num = 2 * Pi / Beta;
		for (int f = 0; f < floor; f++)
		{
			if (f % 2 == 0)
			{
				for (int i = 0; i < num; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * Beta;
					if (i % 2 == 0)
					{
						//PS:�������ڵĴ�Բ��СԲ�뾶���ٻ�������0.005��������float�ͼ���ʱ���������
						resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle);
						resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle);
						resY = r + center.fXYZ[1] + f * floorheight;
						ExternalRes.push_back(resX);
						ExternalRes.push_back(resY);
						ExternalRes.push_back(resZ);
					}
					else
					{
						resX = center.fXYZ[0] + (R2 + r + 0.005) * cos(angle);
						resZ = center.fXYZ[2] + (R2 + r + 0.005) * sin(angle);
						resY = r + center.fXYZ[1] + f * floorheight;
						InternalRes.push_back(resX);
						InternalRes.push_back(resY);
						InternalRes.push_back(resZ);
					}
				}
			}
			else
			{
				for (int i = 0; i < num; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * Beta;
					if (i % 2 == 1)
					{
						resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle);
						resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle);
						resY = r + center.fXYZ[1] + f * floorheight;
						ExternalRes.push_back(resX);
						ExternalRes.push_back(resY);
						ExternalRes.push_back(resZ);
					}
					else
					{
						resX = center.fXYZ[0] + (R2 + r + 0.005) * cos(angle);
						resZ = center.fXYZ[2] + (R2 + r + 0.005) * sin(angle);
						resY = r + center.fXYZ[1] + f * floorheight;
						InternalRes.push_back(resX);
						InternalRes.push_back(resY);
						InternalRes.push_back(resZ);
					}
				}
			}
		}
		finalRes.push_back(InternalRes);
		finalRes.push_back(ExternalRes);
		return finalRes;
	}

	//չʾһ��
	else
	{
		VFLOAT Res;//����һ��

		int floor = 0;//����
		float floorheight = pow(3, 0.5) * r;//���
		floor = (h - r + pow(3, 0.5) * r / 2) / floorheight;

		//C��һ���м����
		float C = r / (R2 + r);

		//�������ÿһ�����װ�¶��ٸ�С��
		int num = 0;

		//half_a�ǽ�����Բ��һ��Բ��ռ�Ļ���
		float halfBeta = asin(C);
		//an�ǽ�����Բ������Բ��ռ�Ļ���
		float Beta = halfBeta * 2;
		num = 2 * Pi / Beta;
		for (int f = 0; f < floor; f++)
		{
			if (f % 2 == 0)
			{
				for (int i = 0; i < num; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * Beta;
					resX = center.fXYZ[0] + (R2 + r) * cos(angle);
					resZ = center.fXYZ[2] + (R2 + r) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight;

					Res.push_back(resX);
					Res.push_back(resY);
					Res.push_back(resZ);
				}
			}
			else
			{
				for (int i = 0; i < num; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * Beta + halfBeta;
					resX = center.fXYZ[0] + (R2 + r) * cos(angle);
					resZ = center.fXYZ[2] + (R2 + r) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight;

					Res.push_back(resX);
					Res.push_back(resY);
					Res.push_back(resZ);
				}
			}
		}
		finalRes.push_back(Res);
		return finalRes;
	}
}

//���� Բ��	�߶�����		С��뾶	
vector<VFLOAT> getFragSphere2(Ver3D center, VECLSTPT* oC, float r)
{
	vector<VFLOAT> finalRes;
	//�����������
	VFLOAT Res1;//��1��
	VFLOAT Res2;//��2��
	VFLOAT Res3;//��3��
	finalRes.push_back(Res1);
	finalRes.push_back(Res2);
	finalRes.push_back(Res3);

	int floor = 0;//����
	//��������ϵ
	//��ż������С���Բ������
	float resX;//X������
	float resY;//Y������
	float resZ;//Z����Ļ����

	int fff = oC->at(0).size();

	for (int Lnum = 0; Lnum < fff - 1; Lnum++)
	{

		//�߶θ߶�
		float h = oC->at(2)[Lnum].x() - oC->at(2)[Lnum + 1].x();
		//ÿһ���߶����ǳ���
		float alpha = atan((oC->at(0)[Lnum].y() - oC->at(0)[Lnum + 1].y()) / (oC->at(0)[Lnum].x() - oC->at(0)[Lnum + 1].x()));
		//��ʼR1��R2
		float R1 = oC->at(1)[Lnum].y();//195
		float R2 = oC->at(2)[Lnum].y();//170
		//��ʼ����ֵ
		float indentation0 = r / tan((Pi / 2 - alpha) / 2) - r;
		//ʵ��С�������
		//ÿ����һ�������ֵ
		float indentation = 2 * r * tan(alpha);
		//չʾ����
		if ((R1 - R2) > (6 * r + indentation))
		{
			int floor = 0;//����
			//float floorheight = pow(3, 0.5) * r * cos(alpha);//���
			float floorheight = 2 * r;
			//floor = (h - r + pow(3, 0.5) * r / 2) / floorheight;
			floor = h / floorheight;
			//ÿ����һ�������ֵ
			//float indentation = pow(3, 0.5) * r * sin(alpha);
			//float indentation = 2 * r * tan(alpha);
			//C��һ���м����
			//С��ƫ��ֵ
			float Left = 0;
			float OffsetBeta = 0;

			//�����������
			for (int f = 0; f < floor; f++)
			{
				//��һ��
				float halfBeta1 = asin(r / (R1 - r * 4));
				float Beta1 = halfBeta1 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num1 = 2 * Pi / Beta1;
				//С��ƫ��ֵ
				Left = 2 * Pi - num1 * Beta1;
				OffsetBeta = (Left / num1);
				for (int i = 0; i < num1; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta1 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r * 5 - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r * 5 - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[0].push_back(resX);
					finalRes[0].push_back(resY);
					finalRes[0].push_back(resZ);
				}
				//�ڶ���
				float halfBeta2 = asin(r / (R1 - r * 2));
				float Beta2 = halfBeta2 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num2 = 2 * Pi / Beta2;
				//С��ƫ��ֵ
				Left = 2 * Pi - num2 * Beta2;
				OffsetBeta = (Left / num2);
				for (int i = 0; i < num2; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta2 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r * 3 - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r * 3 - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[1].push_back(resX);
					finalRes[1].push_back(resY);
					finalRes[1].push_back(resZ);
				}
				//������
				float halfBeta3 = asin(r / (R1 - r));
				float Beta3 = halfBeta3 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num3 = 2 * Pi / Beta3;
				//С��ƫ��ֵ
				Left = 2 * Pi - num3 * Beta3;
				OffsetBeta = (Left / num3);
				for (int i = 0; i < num3; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta3 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[2].push_back(resX);
					finalRes[2].push_back(resY);
					finalRes[2].push_back(resZ);
				}

				R1 = R1 - indentation;
				R2 = R2 - indentation;
			}
		}

		//չʾ����
		else if ((R1 - R2) > (4 * r + indentation))
		{
			int floor = 0;//����
			//float floorheight = pow(3, 0.5) * r * cos(alpha);//���
			float floorheight = 2 * r;
			//floor = (h - r + pow(3, 0.5) * r / 2) / floorheight;
			floor = h / floorheight;
			//ÿ����һ�������ֵ
			//float indentation = pow(3, 0.5) * r * sin(alpha);
			//float indentation = 2 * r * tan(alpha);
			//C��һ���м����
			//С��ƫ��ֵ
			float Left = 0;
			float OffsetBeta = 0;

			//�����������
			for (int f = 0; f < floor; f++)
			{
				//��һ��
				float halfBeta1 = asin(r / (R1 - r * 4));
				float Beta1 = halfBeta1 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num1 = 2 * Pi / Beta1;
				//С��ƫ��ֵ
				Left = 2 * Pi - num1 * Beta1;
				OffsetBeta = (Left / num1);
				for (int i = 0; i < num1; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta1 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r * 3 - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r * 3 - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[0].push_back(resX);
					finalRes[0].push_back(resY);
					finalRes[0].push_back(resZ);
				}
				//�ڶ���
				float halfBeta2 = asin(r / (R1 - r * 2));
				float Beta2 = halfBeta2 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num2 = 2 * Pi / Beta2;
				//С��ƫ��ֵ
				Left = 2 * Pi - num2 * Beta2;
				OffsetBeta = (Left / num2);
				for (int i = 0; i < num2; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta2 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[1].push_back(resX);
					finalRes[1].push_back(resY);
					finalRes[1].push_back(resZ);
				}
				R1 = R1 - indentation;
				R2 = R2 - indentation;
			}
		}

		//չʾһ��
		else
		{
			int floor = 0;//����
			//float floorheight = pow(3, 0.5) * r * cos(alpha);//���
			float floorheight = 2 * r;
			//floor = (h - r + pow(3, 0.5) * r / 2) / floorheight;
			floor = h / floorheight;
			//ÿ����һ�������ֵ
			//float indentation = pow(3, 0.5) * r * sin(alpha);
			//float indentation = 2 * r * tan(alpha);
			//C��һ���м����
			//С��ƫ��ֵ
			float Left = 0;
			float OffsetBeta = 0;

			//�����������
			for (int f = 0; f < floor; f++)
			{
				//��һ��
				float halfBeta1 = asin(r / (R1 - r * 4));
				float Beta1 = halfBeta1 * 2;
				//�������ÿһ�����װ�¶��ٸ�С��
				int num1 = 2 * Pi / Beta1;
				//С��ƫ��ֵ
				Left = 2 * Pi - num1 * Beta1;
				OffsetBeta = (Left / num1);
				for (int i = 0; i < num1; i++)
				{
					//��С����Բ�ļнǣ����ȣ�
					float angle = i * (Beta1 + OffsetBeta);
					//��3��
					resX = center.fXYZ[0] + (R1 - r - indentation0) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r - indentation0) * sin(angle);
					resY = r + center.fXYZ[1] + f * floorheight - oC->at(0)[Lnum].x() + oC->at(0)[0].x();
					finalRes[0].push_back(resX);
					finalRes[0].push_back(resY);
					finalRes[0].push_back(resZ);
				}
				R1 = R1 - indentation;
				R2 = R2 - indentation;
			}

		}

	}
	return finalRes;

}





//****************************************************************


WarheadOGLManager::WarheadOGLManager(QWidget* parent) : QOpenGLWidget(parent)
{
	this->setGeometry(10, 0, OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT);
}

WarheadOGLManager::~WarheadOGLManager() {
	//if (camera)
		//delete camera;

}


void WarheadOGLManager::initializeGL()
{
	/*********** OGL���� ***********/
	pCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glEnable(GL_PROGRAM_POINT_SIZE);

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	isFirstMouse = GL_TRUE;
	isLeftMousePress = GL_FALSE;

	lastX = 0;
	lastY = 0;

	rotateRaw = 0.0f;
	rotatePitch = 0.0f;

	time.start();


	/************ ����� ***********/
	camera = new Camera(CAMERA_POSITION);


	allNodes.resize(0);
	allFragPos.resize(0);

	//
	initShowFlag = GL_FALSE;
	if_sectionalMode = 0;

	/************ ����shader ***********/

	ResourceManager::loadShader("warhead", ":/shaders/res/shaders/Warhead.vert", ":/shaders/res/shaders/Warhead.frag");
	ResourceManager::loadShader("WarheadSphere", ":/shaders/res/shaders/WarheadSphere.vert", ":/shaders/res/shaders/WarheadSphere.frag");
	ResourceManager::loadShader("sectionalWarhead", ":/shaders/res/shaders/sectionalWarhead.vert", ":/shaders/res/shaders/sectionalWarhead.frag");

	/************ ����Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");


	/***********  shader���� **************/

	targetModel.setToIdentity();
	targetModelsave.setToIdentity();
	targetModeluse.setToIdentity();

	targetModel.rotate(90, 0, 0, 1);
	targetModelsave.rotate(90, 0, 0, 1);
	targetModeluse.rotate(90, 0, 0, 1);

	int midH = 0;
	if (oglWarhead.mapArmHead.size() != 0)
	{
		for (int i = 0; i < oglWarhead.mapArmHead.size(); i++)
		{
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("�����ϱ߳�H1")) { midH += oglWarhead.mapArmHead[i].nUnitPropty / 2; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("ǰ�Ǻ��T8")) { midH += oglWarhead.mapArmHead[i].nUnitPropty / 2; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��Ǻ��T9")) { midH += oglWarhead.mapArmHead[i].nUnitPropty / 2; }
		}
	}

	targetModel.translate(midH, -midH, 0);


	ResourceManager::getShader("warhead").use().setMatrix4f("model", targetModel);

	ResourceManager::getShader("WarheadSphere").use().setMatrix4f("model", targetModel);

	ResourceManager::getShader("sectionalWarhead").use().setMatrix4f("model", targetModel);
	/***********  ����Uniform Buffer��ز��� **************/
	/*
	 * ��version 420���ϰ汾 ֱ������ɫ���н���uniform Block indices��Binding Points�󶨣�������һЩ
	 * �����330�汾������Ҫ�����ֶ��󶨹��̡�
	*/
	/*GLuint uniformBlockIndexRed = pCore->glGetUniformBlockIndex(ResourceManager::getShader("red").getShaderID(), "Matrices");
	GLuint uniformBlockIndexGreen = pCore->glGetUniformBlockIndex(ResourceManager::getShader("green").getShaderID(), "Matrices");
	GLuint uniformBlockIndexBlue = pCore->glGetUniformBlockIndex(ResourceManager::getShader("blue").getShaderID(), "Matrices");
	GLuint uniformBlockIndexYellow = pCore->glGetUniformBlockIndex(ResourceManager::getShader("yellow").getShaderID(), "Matrices");

	pCore->glUniformBlockBinding(ResourceManager::getShader("red").getShaderID(), uniformBlockIndexRed, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("green").getShaderID(), uniformBlockIndexGreen, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("blue").getShaderID(), uniformBlockIndexBlue, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("yellow").getShaderID(), uniformBlockIndexYellow, 0);*/

	pCore->glGenBuffers(1, &uboMatrices);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferData(GL_UNIFORM_BUFFER, 2 * 4 * sizeof(QVector4D), NULL, GL_STATIC_DRAW);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);

	pCore->glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * 4 * sizeof(QVector4D));

	///************ ������ɫ�������� ***********/
	pCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCore->glClearColor(245.0 / 255.0, 222.0 / 255.0, 179.0 / 255.0f, 1.0f);

}

void WarheadOGLManager::resizeGL(int w, int h)
{
	pCore->glViewport(0, 0, w, h);

	//������ʾ���ȱ�����
	GLfloat a = w;
	GLfloat b = h;
	int nMin = a;
	if (nMin > b)
	{
		nMin = b;
	}
	int nSx = (a - nMin) / 2;
	int nSy = (b - nMin) / 2;
	//pCore->glViewport(nSx, nSy, nMin, nMin);
}


void WarheadOGLManager::paintGL()
{
	pCore->glClear(GL_COLOR_BUFFER_BIT);
	pCore->glClear(GL_DEPTH_BUFFER_BIT);


	/*********** ��������֡��֮���ʱ����  ***************/
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//update
	this->updateGL();



	//����ģʽ
	if_sectionalMode = 1;


	ResourceManager::getShader("warhead").use();
	ResourceManager::getShader("warhead").use().setFloat("tranS",0.25);
	ResourceManager::getShader("warhead").use().setInteger("if_sectionalMode", if_sectionalMode);



	

	Ver3D center;
	center.fXYZ[0] = 0.0f, center.fXYZ[1] = 0.0f, center.fXYZ[2] = 0.0f;
	

	if (oglWarhead.mapArmHead.size() != 0 && oglDefColor.size()!=0)			//����ս����
	{
		if (initShowFlag == GL_FALSE)
		{
			showFlags.resize(oglWarhead.mapArmHead.size());
			for (int k = 0; k < oglWarhead.mapArmHead.size(); k++)
			{
				showFlags[k] = GL_TRUE;
			}
			initShowFlag = GL_TRUE;
		}


		//ս������������
		float CirBottomR;		//���������Բ���뾶
		float CirBottomH;		//���������Բ�����

		float CirTopR;		//����������Բ���뾶
		float CirTopH;		//����������Բ�����

		float sideR1, sideR2, sideH;			//���ǲ���Բ���뾶��߶�

		float fragR;			//��״����뾶

		float fuseR1, fuseH1, fuseR2, fuseH2;

		for (int i = 0; i < oglWarhead.mapArmHead.size(); i++)
		{

			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("ǰ�ǰ뾶R8")) { CirBottomR = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("ǰ�Ǻ��T8")) { CirBottomH = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��ǰ뾶R9")) { CirTopR = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��Ǻ��T9")) { CirTopH = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("�����ϱ߳�H1")) { sideH = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��ǰ뾶R1")) { sideR1 = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("�ڿǰ뾶R2")) { sideR2 = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��о�뾶R5")) { fragR = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("���Ű뾶R4")) { fuseR1 = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("���Ĺܳ�H4")) { fuseH1 = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��о�뾶R5")) { fuseR2 = oglWarhead.mapArmHead[i].nUnitPropty; }
			if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��о��H5")) { fuseH2 = oglWarhead.mapArmHead[i].nUnitPropty; }


		}

		//�����嶥���
		if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("��ǰ뾶R9"))])
		{
			QColor color = oglDefColor[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("��ǰ뾶R9"))];
			ResourceManager::getShader("warhead").use().setFloat("R", color.red());
			ResourceManager::getShader("warhead").use().setFloat("G", color.green());
			ResourceManager::getShader("warhead").use().setFloat("B", color.blue());


			center.fXYZ[1] = CirBottomH + sideH;
			InitAndDrawColumn(center, CirTopR, CirTopH);
		}


		//����������
		if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("ǰ�ǰ뾶R8"))])
		{
			QColor color = oglDefColor[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("ǰ�ǰ뾶R8"))];
			ResourceManager::getShader("warhead").use().setFloat("R", color.red());
			ResourceManager::getShader("warhead").use().setFloat("G", color.green());
			ResourceManager::getShader("warhead").use().setFloat("B", color.blue());

			center.fXYZ[0] = 0.0f, center.fXYZ[1] = 0.0f, center.fXYZ[2] = 0.0f;
			InitAndDrawColumn(center, CirBottomR, CirBottomH);
		}


		//����
		if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("���Ű뾶R4"))])
		{

			QColor color = oglDefColor[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("���Ű뾶R4"))];
			ResourceManager::getShader("warhead").use().setFloat("R", color.red());
			ResourceManager::getShader("warhead").use().setFloat("G", color.green());
			ResourceManager::getShader("warhead").use().setFloat("B", color.blue());


			center.fXYZ[1] = CirBottomH + 0.005f;
			DrawFuse(center, fuseR1, fuseH1, fuseR2, fuseH2, GL_FALSE);
		}



		//Բ���������
		//��һ�����ڶ����֮��Ҫװ��Ƭ�������Ƚ���һ�������Ϊ͸��
		//��Ϊ����������
		if (oglCurveData.size() != 0)
		{

			//���
			if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("��ǰ뾶R1"))])
			{
				QColor color = oglDefColor[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("��ǰ뾶R1"))];
				ResourceManager::getShader("warhead").use().setFloat("R", color.red());
				ResourceManager::getShader("warhead").use().setFloat("G", color.green());
				ResourceManager::getShader("warhead").use().setFloat("B", color.blue());

				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < oglCurveData[i].size() - 1; j++)
					{
						//���߶��γ�Բ̨
						//Բ̨��ԲR
						int downR = abs(oglCurveData[i][j].y());
						//Բ̨��ԲR
						int upR = abs(oglCurveData[i][j + 1].y());

						int H = oglCurveData[i][j].x() - oglCurveData[i][j + 1].x();

						//Բ̨�������ĸ߶�
						center.fXYZ[1] = abs(oglCurveData[i][j].x());

						DrawRoundPConeSide(center, downR, H, upR, GL_FALSE);
					}
				}
			}


		


			//�ڿ�
			if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("�ڿǰ뾶R2"))])
			{
				QColor color = oglDefColor[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("�ڿǰ뾶R2"))];
				ResourceManager::getShader("warhead").use().setFloat("R", color.red());
				ResourceManager::getShader("warhead").use().setFloat("G", color.green());
				ResourceManager::getShader("warhead").use().setFloat("B", color.blue());


				for (int i = 2; i < 4; i++)
				{
					for (int j = 0; j < oglCurveData[i].size() - 1; j++)
					{
						//���߶��γ�Բ̨
						//Բ̨��ԲR
						int downR = abs(oglCurveData[i][j].y());
						//Բ̨��ԲR
						int upR = abs(oglCurveData[i][j + 1].y());

						int H = oglCurveData[i][j].x() - oglCurveData[i][j + 1].x();

						//Բ̨�������ĸ߶�
						center.fXYZ[1] = abs(oglCurveData[i][j].x());

						DrawRoundPConeSide(center, downR, H, upR, GL_FALSE);
					}
				}
			}





			//����������߼���õ�������Ƭ�������

			center.fXYZ[1] = CirBottomH;

			res = getFragSphere2(center, &oglCurveData, fragR);


			//�洢
			if (bOutputK == 0)
			{
				for (int k = 0; k < res.size(); k++)
				{
					for (int m = 0; m < res[k].size(); m += 3)
					{
						Ver3D tmp;
						tmp.fXYZ[0] = res[k][m];
						tmp.fXYZ[1] = res[k][m + 1];
						tmp.fXYZ[2] = res[k][m + 2];

						allFragPos.push_back(tmp);
					}
				}
			}
		}
		


		

		//������״��Ƭ
		if (showFlags[getUnitIdx(&oglWarhead, QString::fromLocal8Bit("��о�뾶R5"))])
		{

			ResourceManager::getShader("WarheadSphere").use();
			ResourceManager::getShader("WarheadSphere").use().setInteger("if_sectionalMode", if_sectionalMode);


			//ÿ����ƬҪ��ͬ��ɫ
			{
				for (int i = 0; i < res.size(); i++)
				{
					if (res[i].size() != 0)
					{
						ResourceManager::getShader("WarheadSphere").use().setInteger("layer", i);

						DrawSphere(res[i], fragR);
					}

				}
			}


		}
		


	}


	if (bOutputK == 0 && oglWarhead.mapArmHead.size() != 0 && oglCurveData.size() != 0)
	{
		bOutputK++;
	}

}



void WarheadOGLManager::updateGL()
{

	QMatrix4x4 projection;
	GLfloat a = width();
	GLfloat b = height();
	//projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 2000.f);
	projection.ortho(-500, 500,-500, 500, 1,4000);
	//pCore->glViewport(0, 0, 1200, 1200);
	//projection.frustum(1000, 1000, 1000, 1000, 1, 1000);
	view = camera->getViewMatrix();

	int nMin = a;
	if (nMin > b)
	{
		nMin = b;
	}
	int nSx = (a - nMin) / 2;
	int nSy = (b - nMin) / 2;
	pCore->glViewport(nSx, nSy, nMin, nMin);

	

	ResourceManager::getShader("sectionalWarhead").use().setMatrix4f("model", targetModel);
	ResourceManager::getShader("sectionalWarhead").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("sectionalWarhead").use().setMatrix4f("view", view);


	ResourceManager::getShader("warhead").use().setMatrix4f("model", targetModel);
	ResourceManager::getShader("warhead").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("warhead").use().setMatrix4f("view", view);


	ResourceManager::getShader("WarheadSphere").use().setMatrix4f("model", targetModel);
	ResourceManager::getShader("WarheadSphere").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("WarheadSphere").use().setMatrix4f("view", view);



	/***********  ����Uniform Buffer��ز��� **************/
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(QVector4D), &projection);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(QVector4D), 4 * sizeof(QVector4D), &view);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void WarheadOGLManager::mouseMoveEvent(QMouseEvent* event)
{

	if (isLeftMousePress)//�������תģ��
	{
		targetModeluse = targetModelsave;

		GLint xpos = event->pos().x();
		GLint ypos = event->pos().y();


		if (isFirstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			isFirstMouse = GL_FALSE;
		}

		GLint xoffset = xpos - lastX;
		GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;


		rotateRaw += xoffset * 0.1;
		rotatePitch += yoffset * 0.1;

		//������������תģ�;���

		int midH=0;
		if (oglWarhead.mapArmHead.size() != 0)
		{
			for (int i = 0; i < oglWarhead.mapArmHead.size(); i++)
			{
				if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("�����ϱ߳�H1")) { midH += oglWarhead.mapArmHead[i].nUnitPropty/2; }
				if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("ǰ�Ǻ��T8")) { midH += oglWarhead.mapArmHead[i].nUnitPropty / 2; }
				if (oglWarhead.mapArmHead[i].sUnitName == QString::fromLocal8Bit("��Ǻ��T9")) { midH += oglWarhead.mapArmHead[i].nUnitPropty / 2; }
			}
		}



		targetModel.setToIdentity();
		targetModel.translate(0, midH, 0);


		GLfloat angle_now = qSqrt(qPow(xoffset, 2) + qPow(yoffset, 2)) / 5;
		targetModel.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModel *= targetModeluse;

		targetModelsave.setToIdentity();
		targetModelsave.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModelsave *= targetModeluse;

		targetModel.translate(-0, -midH, -0);
	}


}

void WarheadOGLManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)//ע����button��������buttons������
		isLeftMousePress = GL_TRUE;

	if (event->button() == Qt::RightButton)//ע����button��������buttons������
		isRightMousePress = GL_TRUE;
}

void WarheadOGLManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) { //ע����button��������buttons������
		isLeftMousePress = GL_FALSE;
		isFirstMouse = GL_TRUE;
	}
}

void WarheadOGLManager::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	camera->processMouseScroll(offset.y() / 20.0f);
}



//draw

void WarheadOGLManager::InitAndDrawColumn(Ver3D center, float radius, float height)
{
	//pCore->glDisable(GL_DEPTH_TEST);
	//pCore->glDepthMask(GL_FALSE);
	//pCore->glEnable(GL_BLEND);//������ɫ���
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
	//pCore->glEnable(GL_ALPHA_TEST);
	//pCore->glEnable(GL_SCISSOR_TEST);


	



	int pointsNum = 50;//һ��Ȧ����ĵ����
	vector<Ver3D> points;
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = radius * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = radius * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		points.push_back(tmp);
	}

	//�洢
	if (bOutputK == 0)
	{
		for (int k = 0; k < points.size(); k++)
		{
			allNodes.push_back(points[k]);
		}
	}


	//����
	
		float* vertices = new float[pointsNum * 12];
		for (int i = 0; i < pointsNum - 1; i++)
		{
			vertices[i * 12] = points[i].fXYZ[0];
			vertices[i * 12 + 1] = points[i].fXYZ[1];
			vertices[i * 12 + 2] = points[i].fXYZ[2];

			vertices[i * 12 + 3] = points[i + 1].fXYZ[0];
			vertices[i * 12 + 4] = points[i + 1].fXYZ[1];
			vertices[i * 12 + 5] = points[i + 1].fXYZ[2];

			vertices[i * 12 + 6] = points[i + 1].fXYZ[0];
			vertices[i * 12 + 7] = points[i + 1].fXYZ[1] + height;
			vertices[i * 12 + 8] = points[i + 1].fXYZ[2];

			vertices[i * 12 + 9] = points[i].fXYZ[0];
			vertices[i * 12 + 10] = points[i].fXYZ[1] + height;
			vertices[i * 12 + 11] = points[i].fXYZ[2];
		}
		{
			vertices[(pointsNum - 1) * 12] = points[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 1] = points[pointsNum - 1].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 2] = points[pointsNum - 1].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 3] = points[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 4] = points[0].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 5] = points[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 6] = points[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 7] = points[0].fXYZ[1] + height;
			vertices[(pointsNum - 1) * 12 + 8] = points[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 9] = points[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 10] = points[pointsNum - 1].fXYZ[1] + height;
			vertices[(pointsNum - 1) * 12 + 11] = points[pointsNum - 1].fXYZ[2];
		}

		pCore->glGenBuffers(1, &VBO);
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

		//draw
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glEnableVertexAttribArray(0);
		pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);






		
		pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);


		//delete
		pCore->glDeleteBuffers(1, &VBO);
		delete[] vertices;

	





	//����������Բ(����gl_triangles_fan�ķ�ʽ)

	//circle 1
	
		float* verticesBottom = new float[(pointsNum + 1) * 3 + 9];
		//��һ������Բ��
		verticesBottom[0] = center.fXYZ[0];
		verticesBottom[1] = center.fXYZ[1];
		verticesBottom[2] = center.fXYZ[2];
		for (int i = 0; i < pointsNum; i++)
		{
			verticesBottom[3 + i * 3] = points[i].fXYZ[0];
			verticesBottom[3 + i * 3 + 1] = points[i].fXYZ[1];
			verticesBottom[3 + i * 3 + 2] = points[i].fXYZ[2];
		}
		//���һ����
		{
			verticesBottom[(pointsNum + 1) * 3] = points[pointsNum - 1].fXYZ[0];
			verticesBottom[(pointsNum + 1) * 3 + 1] = points[pointsNum - 1].fXYZ[1];
			verticesBottom[(pointsNum + 1) * 3 + 2] = points[pointsNum - 1].fXYZ[2];

			verticesBottom[(pointsNum + 1) * 3 + 3] = center.fXYZ[0];
			verticesBottom[(pointsNum + 1) * 3 + 4] = center.fXYZ[1];
			verticesBottom[(pointsNum + 1) * 3 + 5] = center.fXYZ[2];

			verticesBottom[(pointsNum + 1) * 3 + 6] = points[0].fXYZ[0];
			verticesBottom[(pointsNum + 1) * 3 + 7] = points[0].fXYZ[1];
			verticesBottom[(pointsNum + 1) * 3 + 8] = points[0].fXYZ[2];
		}

		pCore->glGenBuffers(1, &VBO);
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesBottom, GL_STATIC_DRAW);

		//draw
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glEnableVertexAttribArray(0);
		pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);




		pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
		pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);




		//delete 
		pCore->glDeleteBuffers(1, &VBO);
		delete[] verticesBottom;

	




	//circle 2
	
		float* verticesTop = new float[(pointsNum + 1) * 3 + 9];
		//��һ������Բ��
		verticesTop[0] = center.fXYZ[0];
		verticesTop[1] = center.fXYZ[1] + height;
		verticesTop[2] = center.fXYZ[2];
		for (int i = 0; i < pointsNum; i++)
		{
			verticesTop[3 + i * 3] = points[i].fXYZ[0];
			verticesTop[3 + i * 3 + 1] = points[i].fXYZ[1] + height;
			verticesTop[3 + i * 3 + 2] = points[i].fXYZ[2];
		}
		//���һ����
		{
			verticesTop[(pointsNum + 1) * 3] = points[pointsNum - 1].fXYZ[0];
			verticesTop[(pointsNum + 1) * 3 + 1] = points[pointsNum - 1].fXYZ[1] + height;
			verticesTop[(pointsNum + 1) * 3 + 2] = points[pointsNum - 1].fXYZ[2];

			verticesTop[(pointsNum + 1) * 3 + 3] = center.fXYZ[0];
			verticesTop[(pointsNum + 1) * 3 + 4] = center.fXYZ[1] + height;
			verticesTop[(pointsNum + 1) * 3 + 5] = center.fXYZ[2];

			verticesTop[(pointsNum + 1) * 3 + 6] = points[0].fXYZ[0];
			verticesTop[(pointsNum + 1) * 3 + 7] = points[0].fXYZ[1] + height;
			verticesTop[(pointsNum + 1) * 3 + 8] = points[0].fXYZ[2];
		}

		pCore->glGenBuffers(1, &VBO);
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesTop, GL_STATIC_DRAW);

		//draw
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glEnableVertexAttribArray(0);
		pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);




		pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
		pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);


		//delete
		pCore->glDeleteBuffers(1, &VBO);
		delete[] verticesTop;


	//pCore->glDisable(GL_ALPHA_TEST);
	/*pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);*/
	//pCore->glEnable(GL_DEPTH_TEST);
	//pCore->glDisable(GL_SCISSOR_TEST);
	// 
	//pCore->glDisable(GL_BLEND);//������ɫ���
	//pCore->glDepthMask(GL_TRUE);
}

void WarheadOGLManager::DrawRing(Ver3D center, float r1, float r2)
{

	//pCore->glDepthMask(GL_FALSE);
	//pCore->glEnable(GL_BLEND);//������ɫ���
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����



	int pointsNum = 50;//һ��Ȧ����ĵ����


	vector<Ver3D> Cir1Points;//��Ȧ��Բ�ĵ�
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = r1 * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = r1 * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		Cir1Points.push_back(tmp);
	}

	vector<Ver3D> Cir2Points;//��ȦСԲ�ĵ�
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = r2 * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = r2 * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		Cir2Points.push_back(tmp);
	}


	//�洢
	if (bOutputK == 0)
	{
		for (int k = 0; k < Cir1Points.size(); k++)
		{
			allNodes.push_back(Cir1Points[k]);
		}
		for (int k = 0; k < Cir2Points.size(); k++)
		{
			allNodes.push_back(Cir2Points[k]);
		}
	}



	float* vertices = new float[pointsNum * 12];
	for (int i = 0; i < pointsNum - 1; i++)
	{
		vertices[i * 12] = Cir2Points[i].fXYZ[0];
		vertices[i * 12 + 1] = Cir2Points[i].fXYZ[1];
		vertices[i * 12 + 2] = Cir2Points[i].fXYZ[2];

		vertices[i * 12 + 3] = Cir2Points[i + 1].fXYZ[0];
		vertices[i * 12 + 4] = Cir2Points[i + 1].fXYZ[1];
		vertices[i * 12 + 5] = Cir2Points[i + 1].fXYZ[2];

		vertices[i * 12 + 6] = Cir1Points[i + 1].fXYZ[0];
		vertices[i * 12 + 7] = Cir1Points[i + 1].fXYZ[1];
		vertices[i * 12 + 8] = Cir1Points[i + 1].fXYZ[2];

		vertices[i * 12 + 9] = Cir1Points[i].fXYZ[0];
		vertices[i * 12 + 10] = Cir1Points[i].fXYZ[1];
		vertices[i * 12 + 11] = Cir1Points[i].fXYZ[2];
	}
	//���һ����
	{
		vertices[(pointsNum - 1) * 12] = Cir2Points[pointsNum - 1].fXYZ[0];
		vertices[(pointsNum - 1) * 12 + 1] = Cir2Points[pointsNum - 1].fXYZ[1];
		vertices[(pointsNum - 1) * 12 + 2] = Cir2Points[pointsNum - 1].fXYZ[2];

		vertices[(pointsNum - 1) * 12 + 3] = Cir2Points[0].fXYZ[0];
		vertices[(pointsNum - 1) * 12 + 4] = Cir2Points[0].fXYZ[1];
		vertices[(pointsNum - 1) * 12 + 5] = Cir2Points[0].fXYZ[2];

		vertices[(pointsNum - 1) * 12 + 6] = Cir1Points[0].fXYZ[0];
		vertices[(pointsNum - 1) * 12 + 7] = Cir1Points[0].fXYZ[1];
		vertices[(pointsNum - 1) * 12 + 8] = Cir1Points[0].fXYZ[2];

		vertices[(pointsNum - 1) * 12 + 9] = Cir1Points[pointsNum - 1].fXYZ[0];
		vertices[(pointsNum - 1) * 12 + 10] = Cir1Points[pointsNum - 1].fXYZ[1];
		vertices[(pointsNum - 1) * 12 + 11] = Cir1Points[pointsNum - 1].fXYZ[2];
	}


	pCore->glGenBuffers(1, &VBO);
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);




	/*pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);*/
	

	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices;
}

void WarheadOGLManager::DrawFuse(Ver3D center, float bigR, float height1, float smallR, float height2, GLboolean ifTrans)
{
	//if (ifTrans == GL_TRUE)
	//{
	//	pCore->glDepthMask(GL_FALSE);
	//	pCore->glEnable(GL_BLEND);//������ɫ���
	//	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
	//}




	int pointsNum = 50;//һ��Ȧ����ĵ����



	//��������ϴ���
	{
		if (ifTrans == GL_TRUE)
		{
			pCore->glDisable(GL_DEPTH_TEST);
			//pCore->glDepthMask(GL_FALSE);
			pCore->glEnable(GL_BLEND);//������ɫ���
			pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
		}
		vector<Ver3D> points;
		for (int i = 0; i < pointsNum; i++)
		{
			Ver3D tmp;
			tmp.fXYZ[0] = bigR * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
			tmp.fXYZ[1] = center.fXYZ[1];
			tmp.fXYZ[2] = bigR * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

			points.push_back(tmp);
		}
		//�洢
		if (bOutputK == 0)
		{
			for (int k = 0; k < points.size(); k++)
			{
				allNodes.push_back(points[k]);
			}
		}

		//����
		
			float* vertices = new float[pointsNum * 12];
			for (int i = 0; i < pointsNum - 1; i++)
			{
				vertices[i * 12] = points[i].fXYZ[0];
				vertices[i * 12 + 1] = points[i].fXYZ[1];
				vertices[i * 12 + 2] = points[i].fXYZ[2];

				vertices[i * 12 + 3] = points[i + 1].fXYZ[0];
				vertices[i * 12 + 4] = points[i + 1].fXYZ[1];
				vertices[i * 12 + 5] = points[i + 1].fXYZ[2];

				vertices[i * 12 + 6] = points[i + 1].fXYZ[0];
				vertices[i * 12 + 7] = points[i + 1].fXYZ[1] + height1;
				vertices[i * 12 + 8] = points[i + 1].fXYZ[2];

				vertices[i * 12 + 9] = points[i].fXYZ[0];
				vertices[i * 12 + 10] = points[i].fXYZ[1] + height1;
				vertices[i * 12 + 11] = points[i].fXYZ[2];
			}
			{
				vertices[(pointsNum - 1) * 12] = points[pointsNum - 1].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 1] = points[pointsNum - 1].fXYZ[1];
				vertices[(pointsNum - 1) * 12 + 2] = points[pointsNum - 1].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 3] = points[0].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 4] = points[0].fXYZ[1];
				vertices[(pointsNum - 1) * 12 + 5] = points[0].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 6] = points[0].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 7] = points[0].fXYZ[1] + height1;
				vertices[(pointsNum - 1) * 12 + 8] = points[0].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 9] = points[pointsNum - 1].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 10] = points[pointsNum - 1].fXYZ[1] + height1;
				vertices[(pointsNum - 1) * 12 + 11] = points[pointsNum - 1].fXYZ[2];
			}

			pCore->glGenBuffers(1, &VBO);
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

			//draw
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glEnableVertexAttribArray(0);
			pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);


			//delete
			pCore->glDeleteBuffers(1, &VBO);
			delete[] vertices;
		


		//���µ���Բ(����gl_triangles_fan�ķ�ʽ)   ps:�ϵ��治��
		
			float* verticesBottom = new float[(pointsNum + 1) * 3 + 9];
			//��һ������Բ��
			verticesBottom[0] = center.fXYZ[0];
			verticesBottom[1] = center.fXYZ[1];
			verticesBottom[2] = center.fXYZ[2];
			for (int i = 0; i < pointsNum; i++)
			{
				verticesBottom[3 + i * 3] = points[i].fXYZ[0];
				verticesBottom[3 + i * 3 + 1] = points[i].fXYZ[1];
				verticesBottom[3 + i * 3 + 2] = points[i].fXYZ[2];
			}
			//���һ����
			{
				verticesBottom[(pointsNum + 1) * 3] = points[pointsNum - 1].fXYZ[0];
				verticesBottom[(pointsNum + 1) * 3 + 1] = points[pointsNum - 1].fXYZ[1];
				verticesBottom[(pointsNum + 1) * 3 + 2] = points[pointsNum - 1].fXYZ[2];

				verticesBottom[(pointsNum + 1) * 3 + 3] = center.fXYZ[0];
				verticesBottom[(pointsNum + 1) * 3 + 4] = center.fXYZ[1];
				verticesBottom[(pointsNum + 1) * 3 + 5] = center.fXYZ[2];

				verticesBottom[(pointsNum + 1) * 3 + 6] = points[0].fXYZ[0];
				verticesBottom[(pointsNum + 1) * 3 + 7] = points[0].fXYZ[1];
				verticesBottom[(pointsNum + 1) * 3 + 8] = points[0].fXYZ[2];
			}

			pCore->glGenBuffers(1, &VBO);
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesBottom, GL_STATIC_DRAW);

			//draw
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glEnableVertexAttribArray(0);
			pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


			pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
			pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);

			if (ifTrans == GL_TRUE)
			{
				pCore->glDisable(GL_BLEND);
				//pCore->glDepthMask(GL_TRUE);
				pCore->glEnable(GL_DEPTH_TEST);
				
			}
			//delete
			pCore->glDeleteBuffers(1, &VBO);
			delete[] verticesBottom;
		

	}

	//�����м�Բ��
	Ver3D ringCen;
	ringCen.fXYZ[0] = center.fXYZ[0], ringCen.fXYZ[1] = center.fXYZ[1] + height1, ringCen.fXYZ[2] = center.fXYZ[2];
	DrawRing(ringCen, bigR, smallR);

	//���������С��
	{

		if (ifTrans == GL_TRUE)
		{
			pCore->glDisable(GL_DEPTH_TEST);
			//pCore->glDepthMask(GL_FALSE);
			pCore->glEnable(GL_BLEND);//������ɫ���
			pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
		}

		Ver3D sColCen;
		sColCen.fXYZ[0] = center.fXYZ[0], sColCen.fXYZ[1] = center.fXYZ[1] + height1, sColCen.fXYZ[2] = center.fXYZ[2];

		vector<Ver3D> points;
		for (int i = 0; i < pointsNum; i++)
		{
			Ver3D tmp;
			tmp.fXYZ[0] = smallR * cos(2 * 3.14 * i / pointsNum) + sColCen.fXYZ[0];
			tmp.fXYZ[1] = sColCen.fXYZ[1];
			tmp.fXYZ[2] = smallR * sin(2 * 3.14 * i / pointsNum) + sColCen.fXYZ[2];

			points.push_back(tmp);
		}

		//�洢
		if (bOutputK == 0)
		{
			for (int k = 0; k < points.size(); k++)
			{
				allNodes.push_back(points[k]);
			}
		}



		//����
		
			float* vertices = new float[pointsNum * 12];
			for (int i = 0; i < pointsNum - 1; i++)
			{
				vertices[i * 12] = points[i].fXYZ[0];
				vertices[i * 12 + 1] = points[i].fXYZ[1];
				vertices[i * 12 + 2] = points[i].fXYZ[2];

				vertices[i * 12 + 3] = points[i + 1].fXYZ[0];
				vertices[i * 12 + 4] = points[i + 1].fXYZ[1];
				vertices[i * 12 + 5] = points[i + 1].fXYZ[2];

				vertices[i * 12 + 6] = points[i + 1].fXYZ[0];
				vertices[i * 12 + 7] = points[i + 1].fXYZ[1] + height2;
				vertices[i * 12 + 8] = points[i + 1].fXYZ[2];

				vertices[i * 12 + 9] = points[i].fXYZ[0];
				vertices[i * 12 + 10] = points[i].fXYZ[1] + height2;
				vertices[i * 12 + 11] = points[i].fXYZ[2];
			}
			{
				vertices[(pointsNum - 1) * 12] = points[pointsNum - 1].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 1] = points[pointsNum - 1].fXYZ[1];
				vertices[(pointsNum - 1) * 12 + 2] = points[pointsNum - 1].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 3] = points[0].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 4] = points[0].fXYZ[1];
				vertices[(pointsNum - 1) * 12 + 5] = points[0].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 6] = points[0].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 7] = points[0].fXYZ[1] + height2;
				vertices[(pointsNum - 1) * 12 + 8] = points[0].fXYZ[2];

				vertices[(pointsNum - 1) * 12 + 9] = points[pointsNum - 1].fXYZ[0];
				vertices[(pointsNum - 1) * 12 + 10] = points[pointsNum - 1].fXYZ[1] + height2;
				vertices[(pointsNum - 1) * 12 + 11] = points[pointsNum - 1].fXYZ[2];
			}

			pCore->glGenBuffers(1, &VBO);
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

			//draw
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glEnableVertexAttribArray(0);
			pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);

			
			//delete
			pCore->glDeleteBuffers(1, &VBO);
			delete[] vertices;
		


		//���ϵ���Բ(����gl_triangles_fan�ķ�ʽ)   ps:�µ��治��
		
			float* verticesTop = new float[(pointsNum + 1) * 3 + 9];
			//��һ������Բ��
			verticesTop[0] = sColCen.fXYZ[0];
			verticesTop[1] = sColCen.fXYZ[1] + height2;
			verticesTop[2] = sColCen.fXYZ[2];
			for (int i = 0; i < pointsNum; i++)
			{
				verticesTop[3 + i * 3] = points[i].fXYZ[0];
				verticesTop[3 + i * 3 + 1] = points[i].fXYZ[1] + height2;
				verticesTop[3 + i * 3 + 2] = points[i].fXYZ[2];
			}
			//���һ����
			{
				verticesTop[(pointsNum + 1) * 3] = points[pointsNum - 1].fXYZ[0];
				verticesTop[(pointsNum + 1) * 3 + 1] = points[pointsNum - 1].fXYZ[1] + height2;
				verticesTop[(pointsNum + 1) * 3 + 2] = points[pointsNum - 1].fXYZ[2];

				verticesTop[(pointsNum + 1) * 3 + 3] = sColCen.fXYZ[0];
				verticesTop[(pointsNum + 1) * 3 + 4] = sColCen.fXYZ[1] + height2;
				verticesTop[(pointsNum + 1) * 3 + 5] = sColCen.fXYZ[2];

				verticesTop[(pointsNum + 1) * 3 + 6] = points[0].fXYZ[0];
				verticesTop[(pointsNum + 1) * 3 + 7] = points[0].fXYZ[1] + height2;
				verticesTop[(pointsNum + 1) * 3 + 8] = points[0].fXYZ[2];
			}

			pCore->glGenBuffers(1, &VBO);
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesTop, GL_STATIC_DRAW);

			//draw
			pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
			pCore->glEnableVertexAttribArray(0);
			pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



			pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
			pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);

			if (ifTrans == GL_TRUE)
			{
				pCore->glDisable(GL_BLEND);
				//pCore->glDepthMask(GL_TRUE);
				pCore->glEnable(GL_DEPTH_TEST);
				
			}
			//delete
			pCore->glDeleteBuffers(1, &VBO);
			delete[] verticesTop;
		

	}

	



}

void WarheadOGLManager::DrawColumnSide(Ver3D center, float radius, float height, GLboolean ifTrans)
{
	if (ifTrans == GL_TRUE)
	{
		pCore->glDepthMask(GL_FALSE);
		pCore->glEnable(GL_BLEND);//������ɫ���
		pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
	}


	int pointsNum = 50;//һ��Ȧ����ĵ����
	vector<Ver3D> points;
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = radius * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = radius * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		points.push_back(tmp);
	}
	//�洢
	if (bOutputK == 0)
	{
		for (int k = 0; k < points.size(); k++)
		{
			allNodes.push_back(points[k]);
		}
	}


	//����
	
		float* vertices = new float[pointsNum * 12];
		for (int i = 0; i < pointsNum - 1; i++)
		{
			vertices[i * 12] = points[i].fXYZ[0];
			vertices[i * 12 + 1] = points[i].fXYZ[1];
			vertices[i * 12 + 2] = points[i].fXYZ[2];

			vertices[i * 12 + 3] = points[i + 1].fXYZ[0];
			vertices[i * 12 + 4] = points[i + 1].fXYZ[1];
			vertices[i * 12 + 5] = points[i + 1].fXYZ[2];

			vertices[i * 12 + 6] = points[i + 1].fXYZ[0];
			vertices[i * 12 + 7] = points[i + 1].fXYZ[1] + height;
			vertices[i * 12 + 8] = points[i + 1].fXYZ[2];

			vertices[i * 12 + 9] = points[i].fXYZ[0];
			vertices[i * 12 + 10] = points[i].fXYZ[1] + height;
			vertices[i * 12 + 11] = points[i].fXYZ[2];
		}
		{
			vertices[(pointsNum - 1) * 12] = points[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 1] = points[pointsNum - 1].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 2] = points[pointsNum - 1].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 3] = points[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 4] = points[0].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 5] = points[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 6] = points[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 7] = points[0].fXYZ[1] + height;
			vertices[(pointsNum - 1) * 12 + 8] = points[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 9] = points[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 10] = points[pointsNum - 1].fXYZ[1] + height;
			vertices[(pointsNum - 1) * 12 + 11] = points[pointsNum - 1].fXYZ[2];
		}

		pCore->glGenBuffers(1, &VBO);
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

		//draw
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glEnableVertexAttribArray(0);
		pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);


		pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);

		//delete
		pCore->glDeleteBuffers(1, &VBO);
		delete[] vertices;
	


	if (ifTrans == GL_TRUE)
	{
		pCore->glDisable(GL_BLEND);
		pCore->glDepthMask(GL_TRUE);
	}


	//delete
	//pCore->glDeleteBuffers(1, &VBO);
}


void WarheadOGLManager::DrawRoundPConeSide(Ver3D center, float radius, float height, float radius2, GLboolean ifTrans)
{
	




	int pointsNum = 50;//һ��Ȧ����ĵ����

	vector<Ver3D> points;//����Բ
	vector<Ver3D> uppoints;//����Բ

	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = radius * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = radius * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		points.push_back(tmp);


		tmp.fXYZ[0] = radius2 * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1] + height;
		tmp.fXYZ[2] = radius2 * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];
		uppoints.push_back(tmp);
	}

	//�洢
	if (bOutputK == 0)
	{
		for (int k = 0; k < points.size(); k++)
		{
			allNodes.push_back(points[k]);
		}
		for (int k = 0; k < uppoints.size(); k++)
		{
			allNodes.push_back(uppoints[k]);
		}
	}



	//����
	{
		float* vertices = new float[pointsNum * 12];
		for (int i = 0; i < pointsNum - 1; i++)
		{
			vertices[i * 12] = points[i].fXYZ[0];
			vertices[i * 12 + 1] = points[i].fXYZ[1];
			vertices[i * 12 + 2] = points[i].fXYZ[2];

			vertices[i * 12 + 3] = points[i + 1].fXYZ[0];
			vertices[i * 12 + 4] = points[i + 1].fXYZ[1];
			vertices[i * 12 + 5] = points[i + 1].fXYZ[2];

			vertices[i * 12 + 6] = uppoints[i + 1].fXYZ[0];
			vertices[i * 12 + 7] = uppoints[i + 1].fXYZ[1];
			vertices[i * 12 + 8] = uppoints[i + 1].fXYZ[2];

			vertices[i * 12 + 9] = uppoints[i].fXYZ[0];
			vertices[i * 12 + 10] = uppoints[i].fXYZ[1];
			vertices[i * 12 + 11] = uppoints[i].fXYZ[2];
		}
		{
			vertices[(pointsNum - 1) * 12] = points[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 1] = points[pointsNum - 1].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 2] = points[pointsNum - 1].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 3] = points[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 4] = points[0].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 5] = points[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 6] = uppoints[0].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 7] = uppoints[0].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 8] = uppoints[0].fXYZ[2];

			vertices[(pointsNum - 1) * 12 + 9] = uppoints[pointsNum - 1].fXYZ[0];
			vertices[(pointsNum - 1) * 12 + 10] = uppoints[pointsNum - 1].fXYZ[1];
			vertices[(pointsNum - 1) * 12 + 11] = uppoints[pointsNum - 1].fXYZ[2];
		}

		pCore->glGenBuffers(1, &VBO);
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glBufferData(GL_ARRAY_BUFFER, (pointsNum * 12) * sizeof(float), vertices, GL_STATIC_DRAW);

		//draw
		pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		pCore->glEnableVertexAttribArray(0);
		pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



		if (ifTrans == GL_TRUE)//͸����
		{
			pCore->glDepthMask(GL_FALSE);
			pCore->glEnable(GL_BLEND);//������ɫ���
			pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����
		}

		pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);
		if (ifTrans == GL_TRUE)
		{
			pCore->glDisable(GL_BLEND);
			pCore->glDepthMask(GL_TRUE);
		}

		//delete
		pCore->glDeleteBuffers(1, &VBO);
		delete[] vertices;
	}




	


}


void WarheadOGLManager::DrawSphere(VFLOAT centers, float radius)
{
	//pCore->glDisable(GL_DEPTH_TEST);
	//pCore->glDepthMask(GL_FALSE);
	//pCore->glEnable(GL_BLEND);//������ɫ���
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	//���ɺܶ��뾶��ͬ��ԲȦ���,���������������
	int pointsNum = 10;		//ÿһ��ԲȦ��ĸ���
	int angleStep = 20;		//ÿ��ѭ�����ӵĽǶ�

	int ptNum = pointsNum * (180 / angleStep - 1) * 4;
	float* pBallData = new float[ptNum * 3];


	//���ʼ��һ����
	vector<vector<Ver3D>> BallPoints;			//�洢ÿһ��ԲȦ�ϵĵ�
	for (int angle = 0; angle < 180; angle += angleStep)
	{
		BallPoints.resize(180 / angleStep);

		float cirLayR = radius * cos(2 * 3.14 * angle / 180.0);		//ÿһ��ԲȦ�İ뾶
		float cirLayY = radius * sin(2 * 3.14 * angle / 180.0) + centers[1];		//ÿһ��ԲȦ��Բ�ĵ�Yֵ

		for (int i = 0; i < pointsNum; i++)
		{
			Ver3D tmp;
			tmp.fXYZ[0] = cirLayR * cos(2 * 3.14 * i / pointsNum) + centers[0];
			tmp.fXYZ[1] = cirLayY;
			tmp.fXYZ[2] = cirLayR * sin(2 * 3.14 * i / pointsNum) + centers[2];

			BallPoints[angle / angleStep].push_back(tmp);
		}
	}


	//�������εķ�ʽ�洢����
	for (int i = 0; i < BallPoints.size() - 1; i++)
	{
		for (int j = 0; j < pointsNum - 1; j++)
		{
			pBallData[i * pointsNum * 12 + j * 12] = BallPoints[i][j].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 1] = BallPoints[i][j].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 2] = BallPoints[i][j].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 3] = BallPoints[i][j + 1].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 4] = BallPoints[i][j + 1].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 5] = BallPoints[i][j + 1].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 6] = BallPoints[i + 1][j + 1].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 7] = BallPoints[i + 1][j + 1].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 8] = BallPoints[i + 1][j + 1].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 9] = BallPoints[i + 1][j].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 10] = BallPoints[i + 1][j].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 11] = BallPoints[i + 1][j].fXYZ[2];
		}
		//ÿһ������һ����
		{
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12] = BallPoints[i][pointsNum - 1].fXYZ[0];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 1] = BallPoints[i][pointsNum - 1].fXYZ[1];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 2] = BallPoints[i][pointsNum - 1].fXYZ[2];

			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 3] = BallPoints[i][0].fXYZ[0];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 4] = BallPoints[i][0].fXYZ[1];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 5] = BallPoints[i][0].fXYZ[2];

			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 6] = BallPoints[i + 1][0].fXYZ[0];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 7] = BallPoints[i + 1][0].fXYZ[1];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 8] = BallPoints[i + 1][0].fXYZ[2];

			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 9] = BallPoints[i + 1][pointsNum - 1].fXYZ[0];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 10] = BallPoints[i + 1][pointsNum - 1].fXYZ[1];
			pBallData[i * pointsNum * 12 + (pointsNum - 1) * 12 + 11] = BallPoints[i + 1][pointsNum - 1].fXYZ[2];
		}
	}



	//InitStance

	//��ͬʵ�����λ��ƫ�����洢
	float* translations = new float[centers.size()];
	for (int i = 0; i < centers.size() / 3; i++)
	{
		//���ڵ�һ����λ�õ�ƫ����
		translations[i * 3] = centers[i * 3] - centers[0];
		translations[i * 3 + 1] = centers[i * 3 + 1] - centers[1];
		translations[i * 3 + 2] = centers[i * 3 + 2] - centers[2];

	}


	//�����������

	pCore->glGenBuffers(1, &VBO);
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, ptNum * 3 * sizeof(float), pBallData, GL_STATIC_DRAW);

	//���ݶ���gpu��
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//pCore->glBindBuffer(GL_ARRAY_BUFFER, 0);


	////opengl ʵ������ز���
	pCore->glGenBuffers(1, &instanceVBO);
	pCore->glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, centers.size() * sizeof(float), translations, GL_STATIC_DRAW);
	//pCore->glBindBuffer(GL_ARRAY_BUFFER, 0);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	pCore->glEnableVertexAttribArray(1);
	pCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//pCore->glBindBuffer(GL_ARRAY_BUFFER, 0);
	pCore->glVertexAttribDivisor(1, 1);//����Ϊ1�����ݣ�ÿ��ʵ����һ�θı�һ��


	//draw
	//pCore->glBindVertexArray(0);
	pCore->glDrawArraysInstanced(GL_QUADS, 0, ptNum, centers.size() / 3);//ʵ����������
	//pCore->glDrawArrays(GL_QUADS, 0, ptNum);



	//pCore->glDisable(GL_BLEND);
	//pCore->glDepthMask(GL_TRUE);
	//pCore->glEnable(GL_DEPTH_TEST);


	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] pBallData;
	pCore->glDeleteBuffers(1, &instanceVBO);
	delete[] translations;
}

void WarheadOGLManager::DrawColumnRing(Ver3D center, float radius, float height, float radius2)
{
	//����Բ��
	DrawRing(center, radius, radius2);
	center.fXYZ[1] += height;
	DrawRing(center, radius, radius2);

	//����Բ������
	center.fXYZ[1] -= height;
	DrawColumnSide(center, radius, height, GL_FALSE);
	DrawColumnSide(center, radius2, height, GL_FALSE);
}


