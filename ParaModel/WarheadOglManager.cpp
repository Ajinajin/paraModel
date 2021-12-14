#include "WarheadOglManager.h"
#include <QPalette>

#include <QDebug>

#include <fstream>
#include <set>
#include <malloc.h>
#include <iomanip>



// 定义全局变量 后期修改
const QVector3D CAMERA_POSITION(0.0f, 0.0f, 5.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;


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
	/*********** OGL核心 ***********/
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


	/************ 摄像机 ***********/
	camera = new Camera(CAMERA_POSITION);


	targetModel.setToIdentity();
	targetModelsave.setToIdentity();
	targetModeluse.setToIdentity();


	//
	allNodes.resize(0);


	/************ 载入shader ***********/

	ResourceManager::loadShader("warheadColumn", ":/shaders/res/shaders/WarheadColumn.vert", ":/shaders/res/shaders/WarheadColumn.frag");
	

	/************ 载入Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");


	/***********  cube shader参数 **************/
	QMatrix4x4 model;
	model.setToIdentity();
	QMatrix4x4 projection, view;
	//给着色器变量赋值
	ResourceManager::getShader("warheadColumn").use().setMatrix4f("model", model);
	

	/***********  处理Uniform Buffer相关参数 **************/
	/*
	 * 在version 420以上版本 直接在着色器中进行uniform Block indices与Binding Points绑定，更方便一些
	 * 如果是330版本，则需要以下手动绑定过程。
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

	///************ 背景颜色参数调控 ***********/
	pCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCore->glClearColor(245.0 / 255.0, 222.0 / 255.0, 179.0 / 255.0f, 1.0f);

}

void WarheadOGLManager::resizeGL(int w, int h)
{
	pCore->glViewport(0, 0, w, h);
}


void WarheadOGLManager::paintGL()
{
	pCore->glClear(GL_COLOR_BUFFER_BIT);
	pCore->glClear(GL_DEPTH_BUFFER_BIT);


	/*********** 计算两次帧数之间的时间间隔  ***************/
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//update
	this->updateGL();
	
	ResourceManager::getShader("warheadColumn").use();
	ResourceManager::getShader("warheadColumn").use().setFloat("tranS",0.25);
	Ver3D center;
	center.fXYZ[0] = 0.0f, center.fXYZ[1] = 0.0f, center.fXYZ[2] = 0.0f;
	//InitAndDrawColumn(center,1,2);


	//DrawRing(center,4,2);
	DrawFuse(center,2,2,1,2);


	//绘制一个弹

}



void WarheadOGLManager::updateGL()
{

	QMatrix4x4 projection;
	GLfloat a = width();
	GLfloat b = height();
	projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(),0.1f, 2000.f);
	//projection.ortho(1000,1000,1000,1000,0.1,1000);
	//projection.frustum(1000, 1000, 1000, 1000, 1, 1000);
	view = camera->getViewMatrix();

	
	ResourceManager::getShader("warheadColumn").use().setMatrix4f("model", targetModel);
	ResourceManager::getShader("warheadColumn").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("warheadColumn").use().setMatrix4f("view", view);

	


	/***********  处理Uniform Buffer相关参数 **************/
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(QVector4D), &projection);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(QVector4D), 4 * sizeof(QVector4D), &view);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void WarheadOGLManager::mouseMoveEvent(QMouseEvent* event)
{

	if (isLeftMousePress)//左键则旋转模型
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

		//根据鼠标操作旋转模型矩阵
		targetModel.setToIdentity();
		targetModel.translate(0, 1, 0);

		
		GLfloat angle_now = qSqrt(qPow(xoffset, 2) + qPow(yoffset, 2)) / 5;
		targetModel.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModel *= targetModeluse;

		targetModelsave.setToIdentity();
		targetModelsave.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModelsave *= targetModeluse;

		targetModel.translate(-0, -1, -0);
	}

	
}

void WarheadOGLManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)//注意是button（）不是buttons（）；
		isLeftMousePress = GL_TRUE;

	if (event->button() == Qt::RightButton)//注意是button（）不是buttons（）；
		isRightMousePress = GL_TRUE;
}

void WarheadOGLManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) { //注意是button（）不是buttons（）；
		isLeftMousePress = GL_FALSE;
		isFirstMouse = GL_TRUE;
	}
}

void WarheadOGLManager::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	camera->processMouseScroll(offset.y() / 20.0f);
}

void WarheadOGLManager::InitAndDrawColumn(Ver3D center, float radius, float height)
{

	int pointsNum = 50;//一个圈上面的点个数
	vector<Ver3D> points;
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = radius * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = radius * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		points.push_back(tmp);
	}

	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glDepthMask(GL_FALSE);
	pCore->glEnable(GL_BLEND);//开启颜色混合
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算
	//侧面
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
		
	}
	

	//画两个底面圆(按照gl_triangles_fan的方式)

	//circle 1
	{
		float* verticesBottom = new float[(pointsNum + 1) * 3 + 9];
		//第一个点是圆心
		verticesBottom[0] = center.fXYZ[0];
		verticesBottom[1] = center.fXYZ[1];
		verticesBottom[2] = center.fXYZ[2];
		for (int i = 0; i < pointsNum; i++)
		{
			verticesBottom[3 + i * 3] = points[i].fXYZ[0];
			verticesBottom[3 + i * 3 + 1] = points[i].fXYZ[1];
			verticesBottom[3 + i * 3 + 2] = points[i].fXYZ[2];
		}
		//最后一个面
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
		
	}

	//circle 2
	{
		float* verticesTop = new float[(pointsNum + 1) * 3 + 9];
		//第一个点是圆心
		verticesTop[0] = center.fXYZ[0];
		verticesTop[1] = center.fXYZ[1] + height;
		verticesTop[2] = center.fXYZ[2];
		for (int i = 0; i < pointsNum; i++)
		{
			verticesTop[3 + i * 3] = points[i].fXYZ[0];
			verticesTop[3 + i * 3 + 1] = points[i].fXYZ[1] + height;
			verticesTop[3 + i * 3 + 2] = points[i].fXYZ[2];
		}
		//最后一个面
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
		

	}


	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);
	pCore->glDisable(GL_DEPTH_TEST);
	//delete
	pCore->glDeleteBuffers(1, &VBO);
}

void WarheadOGLManager::DrawRing(Ver3D center, float r1, float r2)
{
	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glDepthMask(GL_FALSE);
	pCore->glEnable(GL_BLEND);//开启颜色混合
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算


	int pointsNum = 50;//一个圈上面的点个数


	vector<Ver3D> Cir1Points;//外圈大圆的点
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = r1 * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = r1 * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		Cir1Points.push_back(tmp);
	}

	vector<Ver3D> Cir2Points;//内圈小圆的点
	for (int i = 0; i < pointsNum; i++)
	{
		Ver3D tmp;
		tmp.fXYZ[0] = r2 * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
		tmp.fXYZ[1] = center.fXYZ[1];
		tmp.fXYZ[2] = r2 * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

		Cir2Points.push_back(tmp);
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
	//最后一个面
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



	pCore->glDisable(GL_BLEND);
	pCore->glDisable(GL_DEPTH_TEST);
	pCore->glDepthMask(GL_TRUE);
	//delete
	pCore->glDeleteBuffers(1,&VBO);
}

void WarheadOGLManager::DrawFuse(Ver3D center, float bigR, float height1, float smallR, float height2)
{
	int pointsNum = 50;//一个圈上面的点个数


	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glDepthMask(GL_FALSE);
	pCore->glEnable(GL_BLEND);//开启颜色混合
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//绘制下面较大柱
	{
		vector<Ver3D> points;
		for (int i = 0; i < pointsNum; i++)
		{
			Ver3D tmp;
			tmp.fXYZ[0] = bigR * cos(2 * 3.14 * i / pointsNum) + center.fXYZ[0];
			tmp.fXYZ[1] = center.fXYZ[1];
			tmp.fXYZ[2] = bigR * sin(2 * 3.14 * i / pointsNum) + center.fXYZ[2];

			points.push_back(tmp);
		}
		//侧面
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

		}


		//画下底面圆(按照gl_triangles_fan的方式)   ps:上底面不画
		{
			float* verticesBottom = new float[(pointsNum + 1) * 3 + 9];
			//第一个点是圆心
			verticesBottom[0] = center.fXYZ[0];
			verticesBottom[1] = center.fXYZ[1];
			verticesBottom[2] = center.fXYZ[2];
			for (int i = 0; i < pointsNum; i++)
			{
				verticesBottom[3 + i * 3] = points[i].fXYZ[0];
				verticesBottom[3 + i * 3 + 1] = points[i].fXYZ[1];
				verticesBottom[3 + i * 3 + 2] = points[i].fXYZ[2];
			}
			//最后一个面
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

		}
	}


	//绘制中间圆环
	Ver3D ringCen;
	ringCen.fXYZ[0] = center.fXYZ[0], ringCen.fXYZ[1] = center.fXYZ[1] + height1, ringCen.fXYZ[2] = center.fXYZ[2];
	DrawRing(ringCen, bigR, smallR);
	
	//绘制上面较小柱
	{
		pCore->glEnable(GL_DEPTH_TEST);
		pCore->glDepthMask(GL_FALSE);
		pCore->glEnable(GL_BLEND);//开启颜色混合
		pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算


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
		//侧面
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

		}


		//画上底面圆(按照gl_triangles_fan的方式)   ps:下底面不画
		{
			float* verticesTop = new float[(pointsNum + 1) * 3 + 9];
			//第一个点是圆心
			verticesTop[0] = sColCen.fXYZ[0];
			verticesTop[1] = sColCen.fXYZ[1] + height2;
			verticesTop[2] = sColCen.fXYZ[2];
			for (int i = 0; i < pointsNum; i++)
			{
				verticesTop[3 + i * 3] = points[i].fXYZ[0];
				verticesTop[3 + i * 3 + 1] = points[i].fXYZ[1] + height2;
				verticesTop[3 + i * 3 + 2] = points[i].fXYZ[2];
			}
			//最后一个面
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

		}
	}



	pCore->glDisable(GL_BLEND);//开启颜色混合
	pCore->glDepthMask(GL_TRUE);
	pCore->glDisable(GL_DEPTH_TEST);
	//delete
	pCore->glDeleteBuffers(1,&VBO);
}








