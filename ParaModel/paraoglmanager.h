#pragma once
#ifndef PARAOGLMANAGER_H
#define PARAOGLMANAGER_H

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

struct Solid
{
	int idx[2][4];
};
typedef vector<Solid> vSolid;

//�������Ĺ�����ԪIdȥ������������ģ�Ͳ���
BasicUnit findUnit(int idx, VUNITTABLE oglUnitTable);

class ParaOGLManager : public QOpenGLWidget
{
public:
	explicit ParaOGLManager(QWidget* parent = 0);
	~ParaOGLManager();

	void handleKeyPressEvent(QKeyEvent* event);
	//void keyPressEvent(QKeyEvent* event)override;			//���̰����¼�

	void handleKeyReleaseEvent(QKeyEvent* event);
	//void keyReleaseEvent(QKeyEvent* event)override;		//�����ͷ��¼�


	GLboolean keys[1024];			// ��ȡ���̰�����ʵ�ֶ������

	enum SwitchView //����ӽ�ö�٣�������ͼ��һ��Ĭ����ͼ��������׼��ͼ��
	{
		NONE,
		FRONT_VIEW,
		BACK_VIEW,
		LEFT_VIEW,
		RIGHT_VIEW,
		TOP_VIEW,
		BOTTOM_VIEW
	};
	SwitchView switchView;


public:
	//����
	// 
	//��
	void InitAndDrawColumn(int x, int y, int z, int radius, int height);//����������Բ��xz���뾶���߶�
	
																		
	//�������(����),����λ����㷨�д�����(openglֻ�ܻ���͹�����)
	void InitAndDrawPolygonPortrait(VINT data, int begin_y, int height);
	//�������(����)
	void InitAndDrawPolygonHorizontal(VINT data, int length);
	
	//�����壨ǽ����ȣ�x\y\z Ϊ����������ĵ�
	void InitAndDrawCuboid(int x, int y, int z, int length, int thickness, int height, int type);
	
	void outputKFile();
	//����������ͼͼƬ
	
	VTOPOTABLE oglTopTable;		//������������˽ṹ��
	VUNITTABLE oglUnitTable;	//��������Ľṹ��Ԫ��
	
	vPoint allNodes;			//�洢��άģ�͵����нڵ���Ϣ
	vSolid allSolids;			//�洢��άģ���ڵ�����solid��Ϣ
	int outFlag = 0;			//��ʱ����������ֻ���һ��K�ļ�


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
	//��������̴�������
	void processInput(GLfloat dt);
	//opengl ���º���
	void updateGL();

	QOpenGLFunctions_4_3_Core* pCore;







	GLuint uboMatrices;
	GLuint VBO, EBO;
};



#endif // PARAOGLMANAGER_H
