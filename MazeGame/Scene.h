#pragma once

#include "Camera.h"
#include "Maze.h"
#include "Cube.h"
class Scene
{
public:

	Scene(GLuint w, GLuint h);
	~Scene();
	//��ʼ������
	virtual void init();
public:

	//Ĭ����Ⱦ���̣�һ�㲻��Ҫ����
	virtual void render();

	//��������
	virtual void renderObjects();

	//������Ӱ
	void renderShadow();
public:

	//��ȡ���
	Camera *getCamera();


public:
	//����ͼƬ
	static GLubyte * loadBMPImage(char *filename, GLint * iWidth, GLint * iHeight, GLint * icomponents, GLenum * eFormat);
	//������������
	void loadTexture(char* filename,GLint index);
private:
	//���ָ��
	std::shared_ptr<Camera> camera_;
	
	//��ͼ��С
	GLint size_;
	GLint minX, minZ;
	GLint maxX, maxZ;

	//�Թ�
	Maze *maze;
	void addWallAt(GLint x, GLint y, direction dir);
	
	//�ذ�
	void addPanel();

	//��������
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
	//���ڴ�С
	GLuint windowWidth, windowHeight;
	//fog
	bool enableFog;

	//����ͼƬ
	GLubyte *BGmap;
	GLint width, height;
	GLenum eFormat;

	void renderBGmap();

	//��������
	bool judge(GLint x, GLint z,Point3 eyePos);

	//����λ��
	GLfloat lightPos[3];
	//ʱ��
	GLint time;
	GLfloat angle;
	GLfloat centerX;
	GLfloat centerZ;
	GLfloat radius;
	void setTime();
public:
	void setFogStatus();
	//��ײ���
	bool judgePosition(Point3 eyePos);
};

