#include "stdafx.h"
#include "Scene.h"

#include <Windows.h>
#include "shader.h"

#define PI 3.1415926

Scene::Scene(GLuint w,GLuint h) :camera_(new Camera()), enableFog(false), size_(30), minX(0), minZ(0),
	maxX(300), maxZ(300),v_index(0),windowWidth(w),windowHeight(h)
{

	
	time = 0;
	angle = PI/4;
	centerX = (maxX - minX) / 2;
	centerZ = (maxZ - minZ) / 2;
	radius = 2.2 * centerX;

	cube_instance = Cube::getInstance();

	//加载背景图片
	char filename[] = "texture//sky.bmp";
	GLubyte *pImage = NULL;
	GLint  component;
	BGmap = loadBMPImage(filename, &width, &height, &component, &eFormat);

	//加载纹理
	loadTexture("texture//snow.bmp", 0);
	loadTexture("texture//wood.bmp", 1);
	loadTexture("texture//stone.bmp", 2);
	currentTexture = 2;

	//创建迷宫
	maze = new Maze((maxX - minX) / size_, (maxZ - minZ) / size_);
	for (int i = minX; i < maxX; i += size_) {
		for (int k = minZ; k < maxZ; k += size_) {
			int indexI = i / size_;
			int indexK = k / size_;
			if (maze->map[indexI][indexK].door[0] == 1) {
				addWallAt(i, k, S);
			}
			if (maze->map[indexI][indexK].door[1] == 1) {
				addWallAt(i, k, W);
			}
			if (maze->map[indexI][indexK].door[2] == 1) {
				addWallAt(i, k, N);
			}
			if (maze->map[indexI][indexK].door[3] == 1) {
				addWallAt(i, k, E);
			}
		}
	}
	addPanel();
}


Scene::~Scene()
{
	delete maze;

}

void Scene::init()
{

	//背景白色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//打开深度测试
	glEnable(GL_DEPTH_TEST);


	//关闭背面剔除
	glDisable(GL_CULL_FACE);

	// 打开颜色融合与设置融合函数：这样颜色的Alpha通道才会有作用
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	initShader();
}

//每帧都要调用
void Scene::render() 
{
	glShadeModel(GL_SMOOTH);
	//清楚颜色缓冲区，深度缓冲区，模板缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDepthMask(GL_FALSE);
	renderBGmap();
	glDepthMask(GL_TRUE);

	this->setTime();

	this->renderShadow();

	this->renderObjects();

	glutSwapBuffers();
	
}

//每帧都要调用
void Scene::renderObjects()
{
	glBindVertexArray(VAO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(programId);
	setParameter(false);

	glDrawArrays(GL_QUADS, 0, v_index);
	glUseProgram(NULL);
}

//每帧都要调用
void Scene::renderShadow()
{
	glBindVertexArray(VAO);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glUseProgram(shadowProgramId);
	setParameter(true);

	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_QUADS, 0, v_index);
	//及时将生成的深度图赋值
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glUniform1i(glGetUniformLocation(programId, "shadowTexture"), 0);

	glUseProgram(NULL);
}
//改变光照角度  //每帧都要调用
void Scene::setTime()
{
	lightPos[0] = centerX + cos(angle)*radius;
	lightPos[1] = sin(angle)*radius;
	lightPos[2] = centerZ;
	//angle+=0.001;
}

//每帧不会特意调用
Camera *Scene::getCamera()
{
	return camera_.get();
}




//每帧不会特意调用
GLubyte * Scene::loadBMPImage(char * filename, GLint * iWidth, GLint * iHeight, GLint * icomponents, GLenum * eFormat)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
		return NULL;
	if (fseek(fp, sizeof(BITMAPFILEHEADER), 0))
		return NULL;
	BITMAPINFOHEADER infoHead;
	fread(&infoHead, sizeof(BITMAPINFOHEADER), 1, fp);
	*iWidth = infoHead.biWidth;
	*iHeight = infoHead.biHeight;
	GLint linebyte = (*iWidth * 24 / 8 + 3) / 4 * 4;
	GLubyte *pBits = new unsigned char[linebyte*(*iHeight)];
	fread(pBits, sizeof(char), linebyte*(*iHeight), fp);
	fclose(fp);

	*icomponents = GL_RGB8;
	*eFormat = GL_BGR_EXT;

	return pBits;
}

//每帧不会特意调用
void Scene::loadTexture(char * filename, GLint index)
{
	//图片名称
	//char filename[] = "texture//stone.bmp";

	//载入纹理
	glGenTextures(1, textures+index);
	glBindTexture(GL_TEXTURE_2D, textures[index]);

	GLubyte *pImage = NULL;
	GLint  width, height, component;
	GLenum eFormat;
	pImage = loadBMPImage(filename, &width, &height, &component, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	//std::cout << (pImage == NULL) << std::endl;

	free(pImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//每帧不会特意调用
void Scene::addWallAt(GLint x, GLint y, direction dir)
{
	std::vector<GLfloat> pos = cube_instance->getFace(Point3(x + size_ / 2, 0, y + size_ / 2), size_ / 2, dir);
	for (auto num : pos) {
		vertex[v_index++] = num;
	}
}

//添加地板数据 //每帧不会特意调用
void Scene::addPanel()
{
	for (int i = minX; i < maxX; i += size_) {
		for (int k = minZ; k < maxZ; k += size_) {

			//vertex data
			vertex[v_index++] = i;
			vertex[v_index++] = 0;
			vertex[v_index++] = k;			 
			//texture coord data
			vertex[v_index++] = 1.0; vertex[v_index++] = 1.0;			
			//normal data
			vertex[v_index++] = 0.0; vertex[v_index++] = 1.0; vertex[v_index++] = 0.0;


			vertex[v_index++] = i + size_;
			vertex[v_index++] = 0;
			vertex[v_index++] = k;
			vertex[v_index++] = 1.0; vertex[v_index++] = 0.0;		
			vertex[v_index++] = 0.0; vertex[v_index++] = 1.0; vertex[v_index++] = 0.0;


			vertex[v_index++] = i + size_;
			vertex[v_index++] = 0;
			vertex[v_index++] = k + size_;
			vertex[v_index++] = 0.0; vertex[v_index++] = 0.0;
			vertex[v_index++] = 0.0; vertex[v_index++] = 1.0; vertex[v_index++] = 0.0;

			vertex[v_index++] = i;
			vertex[v_index++] = 0;
			vertex[v_index++] = k + size_;
			vertex[v_index++] = 0.0; vertex[v_index++] = 1.0;
			vertex[v_index++] = 0.0; vertex[v_index++] = 1.0; vertex[v_index++] = 0.0;
		}
	}
}

// 初始化shader //每帧不会特意调用
void Scene::initShader()
{
	//vertex内存布局
	//  3*float+2*float+3*float
	// point.x,point.y,point.z,texcoord.s,texcoord.t,normal.x,normal.y,normal.z 


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v_index, vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
	glEnableVertexAttribArray(2);

	std::vector<GLuint> idVector;
	idVector.push_back(Shader::createShader(GL_VERTEX_SHADER, "glsl//vertexShaderStr.glsl"));
	idVector.push_back(Shader::createShader(GL_FRAGMENT_SHADER, "glsl//fragmentShaderStr.glsl"));
	programId = Shader::createProgram(idVector);

	//创建深度映射（用纹理代替）
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);


	//创建FBO渲染深度信息
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);

	//将深度纹理关联到FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	idVector.clear();
	idVector.push_back(Shader::createShader(GL_VERTEX_SHADER, "glsl//shadowVertexShader.glsl"));
	idVector.push_back(Shader::createShader(GL_FRAGMENT_SHADER, "glsl//shadowFragmentShader.glsl"));
	shadowProgramId = Shader::createProgram(idVector);
}

//设置glsl中参数 //每帧都要调用
void Scene::setParameter(bool renderShadow)
{
	//设置矩阵
	GLfloat projectionMatix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatix);
	GLfloat viewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);

	GLfloat mat[16];
	Camera::LookAtMatrix(Vector3(lightPos[0], lightPos[1], lightPos[2]), Vector3(maxX / 2, 0, maxZ / 2), Vector3(0, 1, 0), mat);


	//camera_.get()->set()
	if (!renderShadow)
	{
		//programId setting
		glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, projectionMatix);
		glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, viewMatrix);

		//设置纹理
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);
		glUniform1i(glGetUniformLocation(programId, "texture1"), 1);


		//设置光照
		GLfloat ambient[] = { 0.5,0.5,0.5 };
		glUniform3f(glGetUniformLocation(programId, "ambient"), ambient[0], ambient[1], ambient[2]);
		GLfloat lightColor[] = { 150 / 255.0,150 / 255.0,0 / 255.0 };
		glUniform3f(glGetUniformLocation(programId, "lightColor"), lightColor[0], lightColor[1], lightColor[2]);
		glUniform3f(glGetUniformLocation(programId, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

		//设置阴影转换矩阵
		glUniformMatrix4fv(glGetUniformLocation(programId, "shadowModelViewMatrix"), 1, GL_FALSE, mat);

	}
	if (renderShadow)
	{
		//shadow programId setting
		//设置矩阵
		glUniformMatrix4fv(glGetUniformLocation(shadowProgramId, "projectionMatrix"), 1, GL_FALSE, projectionMatix);
		glUniformMatrix4fv(glGetUniformLocation(shadowProgramId, "viewMatrix"), 1, GL_FALSE, mat);
		glUniform3f(glGetUniformLocation(shadowProgramId, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);
	}
}

//碰撞检测辅助函数 //每帧不会特意调用
bool Scene::judge(GLint x, GLint z,Point3 eyePos)
{
	bool b = true;
	if (x < 0 || x >= ((maxX - minX) / size_))
		return false;
	if (z < 0 || z >= ((maxZ - minZ) / size_))
		return false;
	if (maze->hasWall(x, z, N))
	{
		b = b && !cube_instance->judgeInside(Point3(x*size_ + size_ / 2, 0, z*size_ + size_ / 2), size_ / 2, N, eyePos);
		//std::cout << "N" << std::endl;
	}
	if (maze->hasWall(x, z, S))
	{
		b = b && !cube_instance->judgeInside(Point3(x*size_ + size_ / 2, 0, z*size_ + size_ / 2), size_ / 2, S, eyePos);
		//std::cout << "S" << std::endl;
	}
	if (maze->hasWall(x, z, E))
	{
		b = b && !cube_instance->judgeInside(Point3(x*size_ + size_ / 2, 0, z*size_ + size_ / 2), size_ / 2, E, eyePos);
		//std::cout << "E" << std::endl;
	}
	if (maze->hasWall(x, z, W))
	{
		b = b && !cube_instance->judgeInside(Point3(x*size_ + size_ / 2, 0, z*size_ + size_ / 2), size_ / 2, W, eyePos);;
		//std::cout << "W" << std::endl;
	}
	return b;
}


//每帧不会特意调用
void Scene::setFogStatus()
{
	enableFog = !enableFog;
}

//碰撞检测 //每帧不会特意调用
bool Scene::judgePosition(Point3 eyePos)
{
	int x_ = eyePos.x / size_, z_ = eyePos.z / size_;
	if (!judge(x_, z_, eyePos)) return false;
	return true;
}

//每帧都要调用
void Scene::renderBGmap()
{
	GLint iViewport[4];
	glGetIntegerv(GL_VIEWPORT, iViewport);
	glPixelZoom((GLfloat)iViewport[2] / (GLfloat)width, (GLfloat)iViewport[3] / (GLfloat)height);
	glDrawPixels(width, height, eFormat, GL_UNSIGNED_BYTE, BGmap);
}


