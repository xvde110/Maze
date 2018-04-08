#pragma once

#include "Camera.h"
#include "Maze.h"
#include "Cube.h"
class Scene
{
public:

	Scene(GLuint w, GLuint h);
	~Scene();
	//初始化场景
	virtual void init();
public:

	//默认渲染流程，一般不需要重载
	virtual void render();

	//绘制物体
	virtual void renderObjects();

	//绘制阴影
	void renderShadow();
public:

	//获取相机
	Camera *getCamera();


public:
	//载入图片
	static GLubyte * loadBMPImage(char *filename, GLint * iWidth, GLint * iHeight, GLint * icomponents, GLenum * eFormat);
	//加载这种纹理
	void loadTexture(char* filename,GLint index);
private:
	//相机指针
	std::shared_ptr<Camera> camera_;
	
	//地图大小
	GLint size_;
	GLint minX, minZ;
	GLint maxX, maxZ;

	//迷宫
	Maze *maze;
	void addWallAt(GLint x, GLint y, direction dir);
	
	//地板
	void addPanel();

	//顶点数据
	GLfloat vertex[100000];
	GLint v_index;

	//shader setting
	GLuint VAO, VBO, programId;
	void initShader();
	void setParameter(bool renderShadow);

	//texture setting
	GLuint textures[20];
	GLuint currentTexture;

	//cube instance
	Cube *cube_instance;

	//shadow mapping
	GLuint depth_texture;
	GLuint depth_fbo;
	GLuint shadowProgramId;
private:
	//窗口大小
	GLuint windowWidth, windowHeight;
	//fog
	bool enableFog;

	//背景图片
	GLubyte *BGmap;
	GLint width, height;
	GLenum eFormat;

	void renderBGmap();

	//辅助函数
	bool judge(GLint x, GLint z,Point3 eyePos);

	//光照位置
	GLfloat lightPos[3];
	//时间
	GLint time;
	GLfloat angle;
	GLfloat centerX;
	GLfloat centerZ;
	GLfloat radius;
	void setTime();
public:
	void setFogStatus();
	//碰撞检测
	bool judgePosition(Point3 eyePos);
};

