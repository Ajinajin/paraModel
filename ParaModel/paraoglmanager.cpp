#include "paraoglmanager.h"
#include <QPalette>

#include <QDebug>

#include <set>
#include <malloc.h>



// 定义全局变量 后期修改
const QVector3D CAMERA_POSITION(40.0f, 30.0f, 50.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

GLboolean targetTexture = GL_FALSE;
float targetTransparent = 0.5f;

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
		this->keys[key] = GL_TRUE;

}

void ParaOGLManager::handleKeyReleaseEvent(QKeyEvent* event)
{
	GLuint key = event->key();
	if (key >= 0 && key <= 1024)
		this->keys[key] = GL_FALSE;
}


void ParaOGLManager::initializeGL()
{
	/*********** OGL核心 ***********/
	pCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glEnable(GL_PROGRAM_POINT_SIZE);


	/*********** 初始化模型细节参数  *************/
	

	/*********** 键鼠响应及时间帧数操作  *************/
	for (GLuint i = 0; i != 1024; ++i) //初始化键盘按键
		keys[i] = GL_FALSE;


	time.start();

	

	/************ 摄像机 ***********/
	camera = new Camera(CAMERA_POSITION);


	//相机视角
	switchView = NONE;

	

	/************ 载入shader ***********/
	//ResourceManager::loadShader("coordinate", ":/shaders/res/shaders/coordinate.vert", ":/shaders/res/shaders/coordinate.frag");
	


	/************ 载入Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");
	

	/***********  cube shader参数 **************/
	QMatrix4x4 model;
	QMatrix4x4 projection, view;
	//给着色器变量赋值
	/*view.translate(QVector3D(0.0f, 0.0f, -3.0f));
	projection.perspective(45.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.0f);
	ResourceManager::getShader("cube").use().setMatrix4f("model", model);
	ResourceManager::getShader("cube").use().setMatrix4f("view", view);
	ResourceManager::getShader("cube").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("cube").use().setInteger("ambientMap", 0);*/

	/***********  GLSL系列（Uniform Buffer） shader参数 **************/
	/*model.setToIdentity();
	model.translate(-0.75f, 0.75f, 0.0f);
	ResourceManager::getShader("GLSL_RED").use().setMatrix4f("model", model);

	model.setToIdentity();
	model.translate(0.75f, 0.75f, 0.0f);
	ResourceManager::getShader("GLSL_GREEN").use().setMatrix4f("model", model);

	model.setToIdentity();
	model.translate(-0.75f, -0.75f, 0.0f);
	ResourceManager::getShader("GLSL_YELLOW").use().setMatrix4f("model", model);

	model.setToIdentity();
	model.translate(0.75f, -0.75f, 0.0f);
	ResourceManager::getShader("GLSL_BLUE").use().setMatrix4f("model", model);*/



	/***********  处理Uniform Buffer相关参数 **************/
	/*
	 * 在version 420以上版本 直接在着色器中进行uniform Block indices与Binding Points绑定，更方便一些
	 * 如果是330版本，则需要以下手动绑定过程。
	*/
	//  GLuint uniformBlockIndexRed = pCore->glGetUniformBlockIndex(ResourceManager::getShader("GLSL_RED").getShaderID(), "Matrices");
	//  GLuint uniformBlockIndexGreen = pCore->glGetUniformBlockIndex(ResourceManager::getShader("GLSL_GREEN").getShaderID(), "Matrices");
	//  GLuint uniformBlockIndexBlue = pCore->glGetUniformBlockIndex(ResourceManager::getShader("GLSL_BLUE").getShaderID(), "Matrices");
	//  GLuint uniformBlockIndexYellow = pCore->glGetUniformBlockIndex(ResourceManager::getShader("GLSL_YELLOW").getShaderID(), "Matrices");

	//  pCore->glUniformBlockBinding(ResourceManager::getShader("GLSL_RED").getShaderID(), uniformBlockIndexRed, 0);
	//  pCore->glUniformBlockBinding(ResourceManager::getShader("GLSL_GREEN").getShaderID(), uniformBlockIndexGreen, 0);
	//  pCore->glUniformBlockBinding(ResourceManager::getShader("GLSL_BLUE").getShaderID(), uniformBlockIndexBlue, 0);
	//  pCore->glUniformBlockBinding(ResourceManager::getShader("GLSL_YELLOW").getShaderID(), uniformBlockIndexYellow, 0);

	//pCore->glGenBuffers(1, &uboMatrices);
	//pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	//pCore->glBufferData(GL_UNIFORM_BUFFER, 2 * 4 * sizeof(QVector4D), NULL, GL_STATIC_DRAW);
	//pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//pCore->glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * 4 * sizeof(QVector4D));

	///************ 背景颜色参数调控 ***********/
	pCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCore->glClearColor(0.5294f, 0.8078f, 0.9215f, 1.0f);
}

void ParaOGLManager::resizeGL(int w, int h)
{
	pCore->glViewport(0, 0, w, h);
}


void ParaOGLManager::paintGL()
{
	pCore->glClear(GL_COLOR_BUFFER_BIT);

	//**********************根据当前场景绘制导弹、目标建筑物****************

	this->updateGL();

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
	

	/*ResourceManager::getShader("boundsBox").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("boundsBox").use().setMatrix4f("view", view);*/

	



	/***********  处理Uniform Buffer相关参数 **************/
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(QVector4D), &projection);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(QVector4D), 4 * sizeof(QVector4D), &view);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ParaOGLManager::mouseMoveEvent(QMouseEvent* event)
{
	GLint xpos = event->pos().x();
	GLint ypos = event->pos().y();
	if (isLeftMousePress)
	{
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
