#include "paraoglmanager.h"
#include <QPalette>

#include <QDebug>

#include <set>
#include <malloc.h>



// 定义全局变量 后期修改
const QVector3D CAMERA_POSITION(0.0f, 2.0f, 5.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

ParaOGLManager::ParaOGLManager(QWidget* parent) : QOpenGLWidget(parent)
{
	this->setGeometry(10, 0, OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT);
}

ParaOGLManager::~ParaOGLManager() {
	if (camera)
		delete camera;

}

void ParaOGLManager::handleKeyPressEvent(QKeyEvent* event)
{
	GLuint key = event->key();
	if (key >= 0 && key <= 1024)
	{
		this->keys[key] = GL_TRUE;
	}

}

void ParaOGLManager::handleKeyReleaseEvent(QKeyEvent* event)
{
	GLuint key = event->key();
	if (key >= 0 && key <= 1024)
	{
		this->keys[key] = GL_FALSE;
	}
}


void ParaOGLManager::initializeGL()
{
	/*********** OGL核心 ***********/
	pCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glEnable(GL_PROGRAM_POINT_SIZE);
	

	/*********** 键鼠响应及时间帧数操作  *************/
	for (GLuint i = 0; i != 1024; ++i) //初始化键盘按键
	{
		keys[i] = GL_FALSE;
	}

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	isFirstMouse = GL_TRUE;
	isLeftMousePress = GL_FALSE;
	//lastX = width() / 2.0f;
	//lastY = height() / 2.0f;

	lastX = 0;
	lastY = 0;

	rotateRaw = 0.0f;
	rotatePitch = 0.0f;

	time.start();


	/************ 摄像机 ***********/
	camera = new Camera(CAMERA_POSITION);


	//相机视角
	switchView = NONE;

	

	/************ 载入shader ***********/
	ResourceManager::loadShader("yellow", ":/shaders/res/shaders/GLSL_YELLOW.vert", ":/shaders/res/shaders/GLSL_YELLOW.frag");
	ResourceManager::loadShader("red", ":/shaders/res/shaders/GLSL_RED.vert", ":/shaders/res/shaders/GLSL_RED.frag");
	ResourceManager::loadShader("green", ":/shaders/res/shaders/GLSL_GREEN.vert", ":/shaders/res/shaders/GLSL_GREEN.frag");
	ResourceManager::loadShader("blue", ":/shaders/res/shaders/GLSL_BLUE.vert", ":/shaders/res/shaders/GLSL_BLUE.frag");
	ResourceManager::loadShader("coordinate", ":/shaders/res/shaders/coordinate.vert", ":/shaders/res/shaders/coordinate.frag");
	


	/************ 载入Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");
	

	/***********  cube shader参数 **************/
	QMatrix4x4 model;
	QMatrix4x4 projection, view;
	//给着色器变量赋值
	ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
	
	/***********  GLSL系列（Uniform Buffer） shader参数 **************/
	model.setToIdentity();
	//model.translate(-0.75f, 0.75f, 0.0f);
	ResourceManager::getShader("red").use().setMatrix4f("model", model);

	//model.setToIdentity();
	//model.translate(0.75f, 0.75f, 0.0f);
	ResourceManager::getShader("green").use().setMatrix4f("model", model);

	//model.setToIdentity();
	//model.translate(-0.75f, -0.75f, 0.0f);
	ResourceManager::getShader("yellow").use().setMatrix4f("model", model);

	//model.setToIdentity();
	//model.translate(0.75f, -0.75f, 0.0f);
	ResourceManager::getShader("blue").use().setMatrix4f("model", model);



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
	pCore->glClearColor(1.0f, 0.9725f, 0.8627f, 1.0f);
	//pCore->glClearColor(1.0f, 1.0f, 1.0, 1.0f);
}

void ParaOGLManager::resizeGL(int w, int h)
{
	pCore->glViewport(0, 0, w, h);
}


void ParaOGLManager::paintGL()
{
	pCore->glClear(GL_COLOR_BUFFER_BIT);

	/*********** 计算两次帧数之间的时间间隔  ***************/
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	this->processInput(deltaTime);
	//update
	this->updateGL();

	//画个柱
	
	ResourceManager::getShader("coordinate").use();
	//ResourceManager::getShader("coordinate").use().setMatrix4f("model",model);
	InitAndDrawColumn(1,2.5);

	

	

}

void ParaOGLManager::processInput(GLfloat dt)
{
	if (keys[Qt::Key_W])
		camera->processKeyboard(FORWARD, dt);
	if (keys[Qt::Key_S])
		camera->processKeyboard(BACKWARD, dt);
	if (keys[Qt::Key_A])
		camera->processKeyboard(LEFT, dt);
	if (keys[Qt::Key_D])
		camera->processKeyboard(RIGHT, dt);
	if (keys[Qt::Key_E])
		camera->processKeyboard(UP, dt);
	if (keys[Qt::Key_Q])
		camera->processKeyboard(DOWN, dt);

}



void ParaOGLManager::updateGL()
{
	
	QMatrix4x4 projection, view;
	projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 2000.f);

	
	
	switch (switchView)
	{
	case FRONT_VIEW:
		view.lookAt(QVector3D(0, 0, 5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//前视图
		break;
	case BACK_VIEW:
		view.lookAt(QVector3D(0, 0, -5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//后视图
		break;
	case LEFT_VIEW:
		view.lookAt(QVector3D(-5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//左视图
		break;
	case RIGHT_VIEW:
		view.lookAt(QVector3D(5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//右视图
		break;
	case TOP_VIEW:
		view.lookAt(QVector3D(0, 5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, -90.0f));					//俯视图
		break;
	case BOTTOM_VIEW:
		view.lookAt(QVector3D(0, -5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 90.0f));					//底视图
		break;
	default:
		view = camera->getViewMatrix();
		break;
	}
	

	//(模型根据鼠标操作旋转)
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	QMatrix4x4 model;
	model.setToIdentity();
	//model.rotate(rotateRaw,0.0,1.0,0.0);
	//model.rotate(-rotatePitch, 1.0, 0.0, 0.0);
	model.rotate(currentFrame * 5, QVector3D(0.0f, 0.0f, 1.0f));
	ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
	ResourceManager::getShader("coordinate").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("coordinate").use().setMatrix4f("view", view);

	



	/***********  处理Uniform Buffer相关参数 **************/
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(QVector4D), &projection);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(QVector4D), 4 * sizeof(QVector4D), &view);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ParaOGLManager::mouseMoveEvent(QMouseEvent* event)
{
	
	if (isLeftMousePress)//左键则旋转模型
	{
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


		xoffset *= 0.2;//根据鼠标灵敏度转换具体偏移角度
		yoffset *= 0.2;

		rotateRaw += xoffset;
		rotatePitch += yoffset;

	}
	
	if (isRightMousePress)//右键则旋转相机
	{
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
		camera->processMouseMovement(xoffset, yoffset);
			
	}
}

void ParaOGLManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)//注意是button（）不是buttons（）；
		isLeftMousePress = GL_TRUE;

	if (event->button() == Qt::RightButton)//注意是button（）不是buttons（）；
		isRightMousePress = GL_TRUE;
}

void ParaOGLManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) { //注意是button（）不是buttons（）；
		isLeftMousePress = GL_FALSE;
		isFirstMouse = GL_TRUE;
	}
}

void ParaOGLManager::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	camera->processMouseScroll(offset.y() / 20.0f);
}


void ParaOGLManager::InitAndDrawColumn(float radius, float height)
{
	//圆柱通过两个圆上下连接实现

	int pointNum = 4320;//每个圆的点个数（精度）

	float* vertices = new float[pointNum * 6];

	for (int i = 0; i < pointNum; i++)
	{
		vertices[i * 6 + 0] = radius * cos(2 * 3.14 * i / pointNum);
		vertices[i * 6 + 1] = 0;
		vertices[i * 6 + 2] = radius * sin(2 * 3.14 * i / pointNum);

		vertices[i * 6 + 3] = vertices[i * 6];
		vertices[i * 6 + 4] = vertices[i * 6 + 1] + height;
		vertices[i * 6 + 5] = vertices[i * 6 + 2];
	}


	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, pointNum * 6 * sizeof(float), vertices, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	pCore->glDepthMask(GL_FALSE);//取消深度缓冲
	pCore->glEnable(GL_BLEND);//开启颜色混合
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alpha值运算

	pCore->glDrawArrays(GL_LINES, 0, pointNum * 2);

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);

	

	
	


	//delete
	pCore->glDeleteBuffers(1, &VBO);
}