// MazeGame.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>
#include <time.h>
#include "Scene.h"

GLint mouseX = 0;
GLint mouseY = 0;
GLint windowWidth = 1366;
GLint windowHeight = 705;
Scene *g_scene = NULL;
void init()
{
	srand(time(0));

	POINT p;
	GetCursorPos(&p);
	mouseX = p.x;
	mouseY = p.y;
	ShowCursor(false);

	g_scene->init();
}

void renderScene()
{
	g_scene->render();

	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	GLfloat k = 0.03;
	g_scene->getCamera()->turnLeftAndRight((x - mouseX)*k);
	mouseX = x;
	g_scene->getCamera()->turnUpAndDown((y - mouseY)*k);
	mouseY = y;

	if (x >= 900 || y >= 600 ||
		x <= 300 || y <= 300) {
		SetCursorPos(windowWidth / 2, windowHeight / 2);
		mouseX = windowWidth / 2;
		mouseY = windowHeight / 2;
	}
}

//keydown
void keyEvents(unsigned char key, int x, int y)
{
	float k = 2;
	Point3 p;
	switch (key) {
	//	//控制四个方向（固定Y轴）
	//	case 'd': g_scene->getCamera()->slide(k, 0, 0); break;
	//	case 'a': g_scene->getCamera()->slide(-k, 0, 0); break;
	//	case 's': g_scene->getCamera()->slide(0, 0, k); break;
	//	case 'w': g_scene->getCamera()->slide(0, 0, -k); break;
	case 'd':
		g_scene->getCamera()->panelSlide(k, 0, 0);
		p = g_scene->getCamera()->getEyePosition();
		if (!g_scene->judgePosition(p)) {
			g_scene->getCamera()->panelSlide(-k, 0, 0);
		}
		break;
	case 'a':
		g_scene->getCamera()->panelSlide(-k, 0, 0);
		p = g_scene->getCamera()->getEyePosition();
		if (!g_scene->judgePosition(p)) {
			g_scene->getCamera()->panelSlide(k, 0, 0);
		}
		break;
	case 's':
		g_scene->getCamera()->panelSlide(0, 0, k);
		p = g_scene->getCamera()->getEyePosition();
		if (!g_scene->judgePosition(p)) {
			g_scene->getCamera()->panelSlide(0, 0, -k);
		}
		break;
	case 'w':
		g_scene->getCamera()->panelSlide(0, 0, -k);
		p = g_scene->getCamera()->getEyePosition();
		if (!g_scene->judgePosition(p)) {
			g_scene->getCamera()->panelSlide(0, 0, k);
		}
		break;

	case 'h':g_scene->setFogStatus(); break;

	case 'q': exit(0);
	default:
		break;
	}
}
int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("glut windows");


	glewInit();

	g_scene = new Scene(windowWidth,windowHeight);

	init();

	glutDisplayFunc(renderScene);

	glutPassiveMotionFunc(mouseMove);


	glutKeyboardFunc(keyEvents);

	glutMainLoop();

	return 0;
}

