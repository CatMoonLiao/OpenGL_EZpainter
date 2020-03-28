#include "pch.h"
#include "GL/glut.h"
#include "iostream"
#include "cmath"
using namespace std;
struct point {
	int x;
	int y;
};
typedef struct object *ptr;
struct object {
	point a;
	point b;
	int type;//dot0 line1 circle2
	ptr link;
};
ptr objects = 0;
ptr now = 0;
bool first_obj = true;
int before_type = -1;
int i = -1;
int temp_type = -1;

void Initial(void)//初始化函数 
{
	glColor3f(0.0f, 0.0f, 0.0f);//绘图颜色为黑色
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);//OpenGL按照三维方式来处理图像，所以需要一个投影变换将三维图形投影到显示器的二维空间中
	gluOrtho2D(0, 500, 500, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//背景顏色
	glClear(GL_COLOR_BUFFER_BIT);//用背景色清空畫布
}

void dot(int x, int y,bool for_r) {
	if (temp_type == 0 && !for_r) {
		ptr temp = new object;
		temp->a.x = x;
		temp->a.y = y;
		temp->type = 0;//dot
		temp->link = 0;
		if (first_obj) {
			first_obj = false;
			objects = temp;
			now = temp;
		}
		else {
			now->link = temp;
			now = now->link;
		}
	}
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	
}

void line(int x1, int y1, int x2, int y2, bool for_r) {
	cout << x1 << " --- " << y1 << endl;
	cout << x2 << " --- " << y2 << endl;
	if (!for_r) {
		ptr temp = new object;
		temp->a.x = x1;
		temp->a.y = y1;
		temp->b.x = x2;
		temp->b.y = y2;
		temp->type = 1;
		temp->link = 0;
		if (first_obj) {
			first_obj = false;
			objects = temp;
			now = temp;
		}
		else {
			now->link = temp;
			now = now->link;
		}
	}
	
	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}

	int tempX = x1, tempY = y1;
	dot(x1, y1,false);
	int a = y2 - y1;
	int b = x1 - x2;
	float s = 0;
	if (b != 0) {
		s = (float)a / (x2 - x1);
	}
	else {
		if (y1 < y2) {
			swap(y1, y2);
			swap(x1, x2);
			tempX = x1;
			tempY = y1;
		}
		while (tempY > y2) {
			tempY--;
			dot(tempX, tempY,false);
		}
		return;
	}

	int d;
	if (s <= 0 && s > -1) {//看起來斜率為正的時候
		d = a - 0.5*b;
		while (tempX < x2) {
			if (d < 0) {//choose E
				tempX++;
				dot(tempX, tempY, false);
				d -= a;
			}
			else {
				tempX++;
				tempY--;
				dot(tempX, tempY, false);
				d -= (a - b);
			}
		}
	}
	else if (s <= -1) { //>45
		d = 0.5*a - b;
		while (tempY > y2) {
			if (d > 0) {//choose  E
				tempX++;
				tempY--;
				dot(tempX, tempY, false);
				d += (a - b);
			}
			else {
				tempY--;
				dot(tempX, tempY, false);
				d -= b;
			}
		}
	}
	else if (s > 0 && s < 1) {
		d = a + 0.5*b;
		while (tempX < x2) {
			if (d < 0) {//choose E
				tempX++;
				dot(tempX, tempY, false);
				d += a;
			}
			else {
				tempX++;
				tempY++;
				dot(tempX, tempY, false);
				d += (a + b);
			}
		}
	}
	else {//斜率為負 >45
		d = 0.5*a + b;
		while (tempY < y2) {
			if (d < 0) {//choose  E
				tempX++;
				tempY++;
				dot(tempX, tempY, false);
				d += (a + b);
			}
			else {
				tempY++;
				dot(tempX, tempY, false);
				d += b;
			}
		}
	}
}

void dot_for_circle(int x, int y, int xc, int yc) {
	int x1 = x - xc;
	int y1 = y - yc;
	dot(x, y, false);
	dot(y1 + xc, x1 + yc, false);
	dot(-x1 + xc, y1 + yc, false);
	dot(y1 + xc, -x1 + yc, false);
	dot(x1 + xc, -y1 + yc, false);
	dot(-y1 + xc, x1 + yc, false);
	dot(-x1 + xc, -y1 + yc, false);
	dot(-y1 + xc, -x1 + yc, false);
}

void circle(int xc, int yc, int x2, int y2,bool for_r) {
	if (!for_r) {
		ptr temp = new object;
		temp->a.x = xc;
		temp->a.y = yc;
		temp->b.x = x2;
		temp->b.y = y2;
		temp->type = 2;//circle
		temp->link = 0;
		if (first_obj) {
			first_obj = false;
			objects = temp;
			now = temp;
		}
		else {
			now->link = temp;
			now = now->link;
		}
	}
	int r = (int)sqrt(pow(xc - x2, 2) + pow(yc - y2, 2));
	int tempX = xc;
	int tempY = yc + r;
	dot_for_circle(tempX, tempY, xc, yc);
	int d = 1 - r;
	int incE = 3;
	int incNE = -2 * r + 5;

	while (tempX - xc < tempY - yc) {
		if (d < 0) {// choose E
			d += incE;
			incNE += 2;
			tempX++;
			dot_for_circle(tempX, tempY, xc, yc);
		}
		else {
			d += incNE;
			incNE += 4;
			tempX++;
			tempY--;
			dot_for_circle(tempX, tempY, xc, yc);
		}
		incE += 2;
	}
}

bool first = true;
point start = { 0 }, before = { 0 };
void mouseFunc(int button, int state, int x, int y)
{
	if (button == 0 && state == 0) {//左鍵點擊
		if (temp_type == 0) { //dot
			dot(x, y, false);
		}
		else if (temp_type == 1) {//line
			if (first) {
				first = false;
				start.x = x;
				start.y = y;
			}
			else {
				first = true;
				line(start.x, start.y, x, y, false);
				start = { 0 };
			}
		}
		else if (temp_type == 2) {//polygon
			if (first) {
				first = false;
				start.x = x;
				start.y = y;
				before.x = x;
				before.y = y;
			}
			else {
				line(before.x, before.y, x, y, false);
				before.x = x;
				before.y = y;
			}
		}
		else if (temp_type == 3) {//circle
			start.x = x;
			start.y = y;
		}
	}
	else if (button == 0 && state == 1) {//左鍵放開
		if (temp_type == 3) {//circle end
			circle(start.x, start.y, x, y, false);
			start = { 0 };
		}
	}
	else if (button == 2 && state == 0) {//右鍵點擊
		if (temp_type == 2) {//polygon end
			line(before.x, before.y, start.x, start.y, false);
			start = { 0 };
			before = { 0 };
			first = true;
		}
	}
	glutPostRedisplay();
}

void printall() {
	ptr go = objects;
	while (go) {
		switch (go->type)
		{
		case 0://dot
			dot(go->a.x, go->a.y, true);
			break;
		case 1://line
			line(go->a.x, go->a.y, go->b.x, go->b.y, true);
			break;
		case 2://cricle
			circle(go->a.x, go->a.y, go->b.x, go->b.y, true);
			break;
		}
		go = go->link;
	}
	glFinish();
}

void keyboardFunc(unsigned char c, int x, int y) {
	first = true;
	start = { 0 };
	before = { 0 };
	switch (c) {
	case 'd'://dot
		temp_type = 0;
		break;
	case 'l'://line
		temp_type = 1;
		break;
	case 'p'://polygon
		temp_type = 2;
		break;
	case 'o'://circle
		temp_type = 3;
		break;
	case 'c'://clear
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//背景顏色
		glClear(GL_COLOR_BUFFER_BIT);//用背景色清空畫布
		glFinish();
		break;

	case 'r'://recover
		printall();
		break;
	case 'q'://quit
		glutDestroyWindow(glutGetWindow());
		break;
	}
}



void mydisplay(void) {
	glFlush();
}
int main(int argc, char * argv[])//这是使用glut库函数进行窗口管理
{
	glutInit(&argc, argv);//使用glut库需要进行初始化	
	glutInitWindowPosition(100, 100);//设定窗口的初始位置，屏幕左上角为原点，单位为像素
	glutInitWindowSize(500, 500);//设定窗口的大小
	glutCreateWindow("2019CG_Lab1_106502512");//创建一个窗口，参数是窗口标题名
	Initial();
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboardFunc);
	glutDisplayFunc(mydisplay);
	glutMainLoop();//使窗口框架运行起来，使显示回调函数开始工作
	return 0;
}