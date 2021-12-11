#include "WarheadOglManager.h"
#include <QPalette>

#include <QDebug>

#include <fstream>
#include <set>
#include <malloc.h>
#include <iomanip>



// 定义全局变量 后期修改
const QVector3D CAMERA_POSITION(0.0f, 5.0f, 15.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

const float Pi = 3.141592654;
VFLOAT res;
int initFlag = 0;
fstream outfile("D:/Study/Work/HS/Ajinajin/paraModel/x64/Debug/outtest.txt",fstream::out);

//*********************工具性函数***********************************

//参数 圆心	圆环大圆半径	圆环小圆半径	球半径	圆柱高度
VFLOAT getFragSphere(Ver3D center, float R1, float R2, float r, float h)
{

	

	VFLOAT res;
	//右手坐标系
	//存放计算出的小球的圆心坐标
	float resX;//X轴向右
	float resY;//Y轴向上
	float resZ;//Z轴屏幕向外

	int floor = 0;//层数
	float floorheight = 2*r*sin(acos((R1-R2-2*r)/2*r));//层高
	floor = (h -(2*r- floorheight))/ floorheight;

	//C是一个中间变量
	float C = ((R2 + r) * (R2 + r) + (R1 - r) * (R1 - r) - 4 * r * r) / (2 * (R2 + r) * (R1 - r));

	//计算可以每一层可以装下多少个小球
	int num = 0;

	//Beta是每个球所占用的弧度
	float Beta = acos(C);

	num = 2 * Pi / Beta;

	for (int f = 0; f < floor; f++)
	{
		if (f % 2 == 0)
		{
			for (int i = 0; i < num; i++)
			{
				//该小球与圆心夹角（弧度）
				float angle = i * Beta;

				if (i % 2 == 0)
				{
					//PS:将计算内的大圆、小圆半径减少或者增加0.005，来缓和float型计算时产生的误差

					resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle);
					resY = center.fXYZ[1] + f * floorheight;


					res.push_back(resX);
					res.push_back(resY);
					res.push_back(resZ);
				}
				else
				{


					resX = center.fXYZ[0] + (R2 + r + 0.005) * cos(angle);
					resZ = center.fXYZ[2] + (R2 + r + 0.005) * sin(angle);
					resY = center.fXYZ[1] + f * floorheight;

					

					res.push_back(resX);
					res.push_back(resY);
					res.push_back(resZ);
				}
			}
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				//该小球与圆心夹角（弧度）
				float angle = i * Beta;

				if (i % 2 == 1)
				{
					resX = center.fXYZ[0] + (R1 - r - 0.005) * cos(angle);
					resZ = center.fXYZ[2] + (R1 - r - 0.005) * sin(angle);
					resY = center.fXYZ[1] + f * floorheight;


					res.push_back(resX);
					res.push_back(resY);
					res.push_back(resZ);
				}
				else
				{


					resX = center.fXYZ[0] + (R2 + r + 0.005) * cos(angle);
					resZ = center.fXYZ[2] + (R2 + r + 0.005) * sin(angle);
					resY = center.fXYZ[1] + f * floorheight;


					res.push_back(resX);
					res.push_back(resY);
					res.push_back(resZ);
				}
			}
		}
	}
	return res;
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

	ResourceManager::loadShader("warhead", ":/shaders/res/shaders/WarheadColumn.vert", ":/shaders/res/shaders/WarheadColumn.frag");
	

	/************ 载入Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");


	/***********  cube shader参数 **************/
	QMatrix4x4 model;
	model.setToIdentity();
	QMatrix4x4 projection, view;
	//给着色器变量赋值
	ResourceManager::getShader("warhead").use().setMatrix4f("model", model);
	

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
	
	ResourceManager::getShader("warhead").use();
	ResourceManager::getShader("warhead").use().setFloat("tranS",0.25);
	Ver3D center;
	center.fXYZ[0] = 0.0f, center.fXYZ[1] = 0.0f, center.fXYZ[2] = 0.0f;
	//InitAndDrawColumn(center,1,2);
	//DrawRing(center,4,2);
	//DrawFuse(center,2,2,1,2);
	//DrawRoundPConeSide(center,5,2,3);




	//绘制一个弹
	{
		float CirBottomR = 5.0;		//弹壳最底面圆柱半径
		float CirBottomH = 0.3;		//弹壳最底面圆柱高度

		float CirTopR = 4.5;		//弹壳最上面圆柱半径
		float CirTopH = 0.3;		//弹壳最上面圆柱高度

		float sideR1=4.2, sideR2 =3.8, sideR3 =3.5,sideH = 10.0;			//弹壳侧面三层圆柱半径与高度

		float fragR = 0.15;			//球状弹丸半径

		//弹柱体底面壳
		{
			ResourceManager::getShader("warhead").use().setFloat("R", 227);
			ResourceManager::getShader("warhead").use().setFloat("G", 168);
			ResourceManager::getShader("warhead").use().setFloat("B", 105);


			center.fXYZ[0] = 0.0f, center.fXYZ[1] = 0.0f, center.fXYZ[2] = 0.0f;
			InitAndDrawColumn(center, CirBottomR, CirBottomH);
		}
		//弹柱体上壳
		{
			ResourceManager::getShader("warhead").use().setFloat("R", 255);
			ResourceManager::getShader("warhead").use().setFloat("G", 128);
			ResourceManager::getShader("warhead").use().setFloat("B", 0);

			center.fXYZ[1] = 10.3f;
			InitAndDrawColumn(center, CirTopR, CirTopH);
		}

		//引信
		{
			ResourceManager::getShader("warhead").use().setFloat("R", 255);
			ResourceManager::getShader("warhead").use().setFloat("G", 0);
			ResourceManager::getShader("warhead").use().setFloat("B", 255);

			center.fXYZ[1] = 0.301f;
			DrawFuse(center, 1.5, 2, 0.5, 1);
		}
		

		//三个圆柱侧面外壳
		//第一层壳与第二层壳之间要装破片，所以先将第一层壳设置为透明
		{


			ResourceManager::getShader("warhead").use().setFloat("R", 255);
			ResourceManager::getShader("warhead").use().setFloat("G", 255);
			ResourceManager::getShader("warhead").use().setFloat("B", 0);
			DrawColumnSide(center, sideR1, sideH, GL_TRUE);
			
			ResourceManager::getShader("warhead").use().setFloat("R", 176);
			ResourceManager::getShader("warhead").use().setFloat("G", 224);
			ResourceManager::getShader("warhead").use().setFloat("B", 230);
			DrawColumnSide(center, sideR2, sideH,GL_FALSE);

			ResourceManager::getShader("warhead").use().setFloat("R", 255);
			ResourceManager::getShader("warhead").use().setFloat("G", 192);
			ResourceManager::getShader("warhead").use().setFloat("B", 203);
			DrawColumnSide(center, sideR3, sideH, GL_FALSE);
		}
		

		
		
		//计算得到所有破片球的坐标
		
		center.fXYZ[1] = CirBottomH;
		if (initFlag == 0)
		{
			res = getFragSphere(center, sideR1, sideR2, fragR, sideH);
			initFlag++;
		}
		



		//绘制球状破片
		{
			for (int i = 0; i < res.size()/3; i++)
			{
				ResourceManager::getShader("warhead").use().setFloat("R", 255);
				ResourceManager::getShader("warhead").use().setFloat("G", 0);
				ResourceManager::getShader("warhead").use().setFloat("B", 0);
				InitSphere(res[i * 3], res[i * 3 + 1], res[i * 3 + 2], fragR);
			}
			
		}
	
	}
	// 
	//InitSphere(1, -2, 0, 2);
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

	
	ResourceManager::getShader("warhead").use().setMatrix4f("model", targetModel);
	ResourceManager::getShader("warhead").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("warhead").use().setMatrix4f("view", view);

	


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
		targetModel.translate(0, 5, 0);

		
		GLfloat angle_now = qSqrt(qPow(xoffset, 2) + qPow(yoffset, 2)) / 5;
		targetModel.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModel *= targetModeluse;

		targetModelsave.setToIdentity();
		targetModelsave.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModelsave *= targetModeluse;

		targetModel.translate(-0, -5, -0);
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

	//pCore->glEnable(GL_DEPTH_TEST);
	//pCore->glDepthMask(GL_FALSE);
	//pCore->glEnable(GL_BLEND);//开启颜色混合
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算
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


	//pCore->glDisable(GL_BLEND);
	//pCore->glDepthMask(GL_TRUE);
	//pCore->glDisable(GL_DEPTH_TEST);
	// 
	//delete
	pCore->glDeleteBuffers(1, &VBO);
}

void WarheadOGLManager::DrawRing(Ver3D center, float r1, float r2)
{



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


	//delete
	pCore->glDeleteBuffers(1,&VBO);
}

void WarheadOGLManager::DrawFuse(Ver3D center, float bigR, float height1, float smallR, float height2)
{
	int pointsNum = 50;//一个圈上面的点个数


	//pCore->glEnable(GL_DEPTH_TEST);
	//pCore->glDepthMask(GL_FALSE);
	//pCore->glEnable(GL_BLEND);//开启颜色混合
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算
	
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
		//pCore->glEnable(GL_DEPTH_TEST);
		//pCore->glDepthMask(GL_FALSE);
		//pCore->glEnable(GL_BLEND);//开启颜色混合
		//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算


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



	//pCore->glDisable(GL_BLEND);//开启颜色混合
	//pCore->glDepthMask(GL_TRUE);
	//pCore->glDisable(GL_DEPTH_TEST);
	//delete
	pCore->glDeleteBuffers(1,&VBO);
}

void WarheadOGLManager::DrawColumnSide(Ver3D center, float radius, float height, GLboolean ifTrans)
{
	if (ifTrans == GL_TRUE)
	{
		pCore->glDepthMask(GL_FALSE);
		pCore->glEnable(GL_BLEND);//开启颜色混合
		pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算
	}
	

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


	if (ifTrans == GL_TRUE)
	{
		pCore->glDisable(GL_BLEND);
		pCore->glDepthMask(GL_TRUE);
	}
	
	
	//delete
	pCore->glDeleteBuffers(1,&VBO);
}


void WarheadOGLManager::DrawRoundPConeSide(Ver3D center, float radius, float height, float radius2)
{
	int pointsNum = 50;//一个圈上面的点个数

	vector<Ver3D> points;//底面圆
	vector<Ver3D> uppoints;//顶面圆

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



		pCore->glDrawArrays(GL_QUADS, 0, pointsNum * 4);

	}


	//画两个底面圆(按照gl_triangles_fan的方式)

	////circle 1
	//{
	//	float* verticesBottom = new float[(pointsNum + 1) * 3 + 9];
	//	//第一个点是圆心
	//	verticesBottom[0] = center.fXYZ[0];
	//	verticesBottom[1] = center.fXYZ[1];
	//	verticesBottom[2] = center.fXYZ[2];
	//	for (int i = 0; i < pointsNum; i++)
	//	{
	//		verticesBottom[3 + i * 3] = points[i].fXYZ[0];
	//		verticesBottom[3 + i * 3 + 1] = points[i].fXYZ[1];
	//		verticesBottom[3 + i * 3 + 2] = points[i].fXYZ[2];
	//	}
	//	//最后一个面
	//	{
	//		verticesBottom[(pointsNum + 1) * 3] = points[pointsNum - 1].fXYZ[0];
	//		verticesBottom[(pointsNum + 1) * 3 + 1] = points[pointsNum - 1].fXYZ[1];
	//		verticesBottom[(pointsNum + 1) * 3 + 2] = points[pointsNum - 1].fXYZ[2];

	//		verticesBottom[(pointsNum + 1) * 3 + 3] = center.fXYZ[0];
	//		verticesBottom[(pointsNum + 1) * 3 + 4] = center.fXYZ[1];
	//		verticesBottom[(pointsNum + 1) * 3 + 5] = center.fXYZ[2];

	//		verticesBottom[(pointsNum + 1) * 3 + 6] = points[0].fXYZ[0];
	//		verticesBottom[(pointsNum + 1) * 3 + 7] = points[0].fXYZ[1];
	//		verticesBottom[(pointsNum + 1) * 3 + 8] = points[0].fXYZ[2];
	//	}

	//	pCore->glGenBuffers(1, &VBO);
	//	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesBottom, GL_STATIC_DRAW);

	//	//draw
	//	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	pCore->glEnableVertexAttribArray(0);
	//	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//	pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
	//	pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);

	//}

	////circle 2
	//{
	//	float* verticesTop = new float[(pointsNum + 1) * 3 + 9];
	//	//第一个点是圆心
	//	verticesTop[0] = center.fXYZ[0];
	//	verticesTop[1] = center.fXYZ[1] + height;
	//	verticesTop[2] = center.fXYZ[2];
	//	for (int i = 0; i < pointsNum; i++)
	//	{
	//		verticesTop[3 + i * 3] = uppoints[i].fXYZ[0];
	//		verticesTop[3 + i * 3 + 1] = uppoints[i].fXYZ[1];
	//		verticesTop[3 + i * 3 + 2] = uppoints[i].fXYZ[2];
	//	}
	//	//最后一个面
	//	{
	//		verticesTop[(pointsNum + 1) * 3] = uppoints[pointsNum - 1].fXYZ[0];
	//		verticesTop[(pointsNum + 1) * 3 + 1] = uppoints[pointsNum - 1].fXYZ[1];
	//		verticesTop[(pointsNum + 1) * 3 + 2] = uppoints[pointsNum - 1].fXYZ[2];

	//		verticesTop[(pointsNum + 1) * 3 + 3] = center.fXYZ[0];
	//		verticesTop[(pointsNum + 1) * 3 + 4] = center.fXYZ[1] + height;
	//		verticesTop[(pointsNum + 1) * 3 + 5] = center.fXYZ[2];

	//		verticesTop[(pointsNum + 1) * 3 + 6] = uppoints[0].fXYZ[0];
	//		verticesTop[(pointsNum + 1) * 3 + 7] = uppoints[0].fXYZ[1];
	//		verticesTop[(pointsNum + 1) * 3 + 8] = uppoints[0].fXYZ[2];
	//	}

	//	pCore->glGenBuffers(1, &VBO);
	//	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	pCore->glBufferData(GL_ARRAY_BUFFER, ((pointsNum + 1) * 3 + 9) * sizeof(float), verticesTop, GL_STATIC_DRAW);

	//	//draw
	//	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	pCore->glEnableVertexAttribArray(0);
	//	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



	//	pCore->glDrawArrays(GL_TRIANGLE_FAN, 0, (pointsNum + 1));
	//	pCore->glDrawArrays(GL_TRIANGLES, (pointsNum + 1), 3);


	//}


	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);
	pCore->glDisable(GL_DEPTH_TEST);
	//delete
	pCore->glDeleteBuffers(1, &VBO);
}


void WarheadOGLManager::InitSphere(float centerX, float centerY, float centerZ, float radius)
{
	//球由很多层半径不同的圆圈组成,球面由三角面组成

	int pointsNum = 10;		//每一层圆圈点的个数
	int angleStep = 20;		//每次循环增加的角度

	int ptNum = pointsNum * (180 / angleStep - 1) * 4;
	float* pBallData = new float[ptNum * 3];

	
	vector<vector<Ver3D>> BallPoints;			//存储每一层圆圈上的点

	for (int angle = 0; angle < 180; angle += angleStep)
	{
		BallPoints.resize(180 / angleStep);

		float cirLayR = radius * cos(2 * 3.14 * angle / 180.0);		//每一层圆圈的半径
		float cirLayY = radius * sin(2 * 3.14 * angle / 180.0) + centerY;		//每一层圆圈的圆心的Y值
		
		for (int i = 0; i < pointsNum; i++)
		{
			Ver3D tmp;
			tmp.fXYZ[0] = cirLayR * cos(2 * 3.14 * i / pointsNum) + centerX;
			tmp.fXYZ[1] = cirLayY;
			tmp.fXYZ[2] = cirLayR * sin(2 * 3.14 * i / pointsNum) + centerZ;

			BallPoints[angle / angleStep].push_back(tmp);
		}
	}

	//按画矩形的方式存储数据
	for (int i = 0; i < BallPoints.size() - 1; i++)
	{
		for (int j = 0; j < pointsNum - 1; j++)
		{
			pBallData[i * pointsNum * 12 + j * 12] = BallPoints[i][j].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 1] = BallPoints[i][j].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 2] = BallPoints[i][j].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 3] = BallPoints[i][j+1].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 4] = BallPoints[i][j+1].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 5] = BallPoints[i][j+1].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 6] = BallPoints[i+1][j + 1].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 7] = BallPoints[i+1][j + 1].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 8] = BallPoints[i+1][j + 1].fXYZ[2];

			pBallData[i * pointsNum * 12 + j * 12 + 9] = BallPoints[i + 1][j].fXYZ[0];
			pBallData[i * pointsNum * 12 + j * 12 + 10] = BallPoints[i + 1][j].fXYZ[1];
			pBallData[i * pointsNum * 12 + j * 12 + 11] = BallPoints[i + 1][j].fXYZ[2];
		}
		//每一层的最后一个面
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
	

	pCore->glGenBuffers(1, &VBO);
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, ptNum * 3 * sizeof(float), pBallData, GL_STATIC_DRAW);

	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



	pCore->glDrawArrays(GL_QUADS, 0, ptNum);
	

	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] pBallData;


	//********************************************************************************************


	//float nStep = 30;
	//int nLoopNum = 360 / nStep;
	//double fPI = 3.14159265;
	//int nPixelNum = 12;
	//int nPtNum = nLoopNum * nLoopNum / 2 * nPixelNum;

	//float* pSphereSurfaceData = new float[1000 * 3];//绘制球需要的数据集，用作根据三角面画球面


	////直接将点集内存更新
	//int i = 0, j = 0;
	//int nLineNum = nLoopNum * nPixelNum;
	//double fA1, fA2, fA3, fA4;
	//double fSin1, fCos1, fSin2, fCos2, temp, temp1;
	//int nRatio = 1;
	//// 生成球数据
	//for (i = 0; i < nLoopNum / 2; i++)
	//{
	//	fA1 = i * nStep * fPI / 180.0;
	//	fA3 = (i + 1) * nStep * fPI / 180.0;

	//	fSin1 = sin(fA1) / nRatio;
	//	fCos1 = cos(fA1) / nRatio;

	//	for (j = 0; j < nLoopNum; j++)
	//	{
	//		float angle = 60.0;
	//		fA2 = j * nStep * fPI / angle;
	//		fSin2 = fSin1 * sin(fA2);
	//		fCos2 = fSin1 * cos(fA2);

	//		// 			pSphereData[i*nLineNum + j + 0] = fCos1;
	//		// 			pSphereData[i*nLineNum + j + 1] = fSin2;
	//		// 			pSphereData[i*nLineNum + j + 2] = fCos2;
	//					/*pSphereData[i*nLineNum + j * nPixelNum + 0] = centerX;
	//					pSphereData[i*nLineNum + j * nPixelNum + 1] = centerY;
	//					pSphereData[i*nLineNum + j * nPixelNum + 2] = centerZ;
	//					pSphereData[i*nLineNum + j * nPixelNum + 3] = fCos1;
	//					pSphereData[i*nLineNum + j * nPixelNum + 4] = fSin2;
	//					pSphereData[i*nLineNum + j * nPixelNum + 5] = fCos2;*/


	//					//pSphereData[i * nLineNum + j + 6] = j % 8 + 1;


	//					//组织球面三角面的各个顶点
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 0] = radius * fCos1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 1] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 2] = radius * fCos2 + centerZ;

	//		pSphereSurfaceData[i * nLineNum + j * 18 + 9] = radius * fCos1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 10] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 11] = radius * fCos2 + centerZ;

	//		fA2 = (j + 1) * nStep * fPI / angle;
	//		fSin2 = fSin1 * sin(fA2);
	//		fCos2 = fSin1 * cos(fA2);
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 15] = radius * fCos1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 16] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 17] = radius * fCos2 + centerZ;

	//		temp = sin(fA3) / nRatio;
	//		temp1 = cos(fA3) / nRatio;
	//		fA2 = (j)*nStep * fPI / angle;
	//		fSin2 = temp * sin(fA2);
	//		fCos2 = temp * cos(fA2);
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 3] = radius * temp1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 4] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 5] = radius * fCos2 + centerZ;

	//		fA2 = (j + 1) * nStep * fPI / angle;
	//		fSin2 = temp * sin(fA2);
	//		fCos2 = temp * cos(fA2);
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 6] = radius * temp1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 7] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 8] = radius * fCos2 + centerZ;

	//		pSphereSurfaceData[i * nLineNum + j * 18 + 12] = radius * temp1 + centerX;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 13] = radius * fSin2 + centerY;
	//		pSphereSurfaceData[i * nLineNum + j * 18 + 14] = radius * fCos2 + centerZ;


	//	}
	//}


	////绘制球面三角面
	//pCore->glGenBuffers(1, &VBO);

	//pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//pCore->glBufferData(GL_ARRAY_BUFFER, nPtNum * sizeof(float), pSphereSurfaceData, GL_STATIC_DRAW);



	////根据三角面片画从圆心爆炸的球
	//pCore->glEnableVertexAttribArray(0);
	//pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));

	//int nNum = 72 * 72;

	//pCore->glDepthMask(GL_FALSE);//取消深度缓冲
	//pCore->glEnable(GL_BLEND);//开启颜色混合
	//pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算

	//pCore->glDrawArrays(GL_TRIANGLES, 0, nNum);

	/*pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);*/


	//delete
	/*pCore->glDeleteBuffers(1, &VBO);
	delete[] pSphereSurfaceData;*/
}

void WarheadOGLManager::DrawColumnRing(Ver3D center, float radius, float height, float radius2)
{
	//两个圆环
	DrawRing(center, radius, radius2);
	center.fXYZ[1] += height;
	DrawRing(center, radius, radius2);

	//两个圆柱侧面
	center.fXYZ[1] -= height;
	DrawColumnSide(center, radius,height,GL_FALSE);
	DrawColumnSide(center, radius2, height, GL_FALSE);
}



