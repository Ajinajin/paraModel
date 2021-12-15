#pragma once
#ifndef WARHEADOGLMANAGER_H
#define WARHEADOGLMANAGER_H

#include <QDockWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTime>
#include <QOpenGLFunctions_4_3_Core>
#include <QMouseEvent>


#include"ParaType.h"
#include<resourcemanager.h>
#include "camera.h"
#include <ShaderProc.h>

typedef vector<QImage> vQImage;

struct Point		//��ṹ(��ά)
{
	int x;
	int y;
	int z;
};
typedef vector<Point> vPoint;


class WarheadOGLManager : public QOpenGLWidget
{
public:
	explicit WarheadOGLManager(QWidget* parent = 0);
	~WarheadOGLManager();


public:
	//����
	
	//��
	void InitAndDrawColumn(Ver3D center, float radius, float height);		//�������������µ㣬�뾶���߶�

	//Բ����
	//�ڶ��������Ǵ�Ȧ�뾶�����ĸ�������СȦ�뾶
	void DrawColumnRing(Ver3D center, float radius, float height,float radius2);		

	//Բ������(���ĸ�����Ϊ�����Ƿ�͸��)
	void DrawColumnSide(Ver3D center, float radius, float height,GLboolean ifTrans);

	//����Բ̨����
	//�ڶ��������ǵ���Բ�뾶�����ĸ������Ƕ���Բ�뾶
	void DrawRoundPConeSide(Ver3D center, float radius, float height,float radius2);

	//����Բ��
	//��һ��������Ȧ�뾶���ڶ�������������Ȧ��СȦ�뾶
	void DrawRing(Ver3D center,float r1,float r2);

	//���Ż���
	//��һ���������µ����ĵ㣬�ڶ����ĸ�������Ȧ��СȦ�뾶����������������Ǵ����߶���С���߶�
	void DrawFuse(Ver3D center,float bigR,float height1,float smallR,float height2);

	//������(����1�������������λ�ü���)
	void DrawSphere(VFLOAT centers, float radius);


	vPoint allNodes;			//�洢��άģ�͵����нڵ���Ϣ


public:
	//�������������ز���
	Camera* camera;
	GLboolean isFirstMouse;

	GLboolean isLeftMousePress;
	GLboolean isRightMousePress;

	GLint lastX;
	GLint lastY;
	QTime time;
	GLfloat deltaTime;
	GLfloat lastFrame;											//��һ֡

	//ģ����ת��ز���
	QMatrix4x4 targetModel;										//ģ�͵�ģ��GLSL����
	QMatrix4x4 targetModelsave;
	QMatrix4x4 targetModeluse;
	QMatrix4x4 view;											//�ڲ���������Ⱦ�۲����
	GLboolean if_change_view;									//���ⲿ������ά��ʾ�۲���� ��־λ

	GLfloat rotateRaw;											//ÿ�ζ�ģ�͵�raw��ת
	GLfloat rotatePitch;										//ÿ�ζ�ģ�͵�pitch��ת

	
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event) override;			//�����¼�
	void mousePressEvent(QMouseEvent* event) override;		//��갴���¼�
	void mouseReleaseEvent(QMouseEvent* event) override;	//����ͷ��¼�
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();



private:

	//opengl ���º���
	void updateGL();

	QOpenGLFunctions_4_3_Core* pCore;


	GLuint uboMatrices;
	GLuint VBO, EBO, instanceVBO;
};



#endif // PARAOGLMANAGER_H
