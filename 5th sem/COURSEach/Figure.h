#pragma once
#include"Vertex.h"
#include<graphics.h>
#include<cmath>
#include<iostream>
#include<stack>
#include<conio.h>

using namespace std;

class Figure
{
	float A[8][4];//ћатрица с точками параллелипипида

	float Matr_of_flat_equation[4][6];//ћатрица уравнений плоскости

	float mull_equ_and_light[6];// ћатрица произведений уравнений плоскости на вектор источника света
	float mull_equ_and_observer[6];//ћатрица произведений уравнений плоскости на вектор наблюдател€

	int observer[4];//“очка наблюдени€, задаЄтс€ в конструкторе
	int light[4];//“очка источника света(в бесконечности,типа =))), задаЄтс€ в конструкторе

	float center[3];

	int y_floor;
public:

	Figure(int flag);

	Figure(const Figure & F);
	Figure & operator=(Figure & f);
	Figure & operator*(Vertex & Bb);
	~Figure();

	//void drow_line(float x1, float y1, float x2, float y2, int color_1 = 0, int color_2 = 0, int color_3 = 0);//flag = 1 закрашиваем, 0 - стираем
	void draw_figure(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, int flag, int flag2);//+
	void del_invisible_line();//-
	void equation_of_flat(int num_of_flat, int T1, int T2, int T3, int T4);//num_of_flat - номер плоскости, куда будет выводитс€ результат, t1 t2 t3 пор€док точек в матрице ј +
	void draw();//+
	int fill(int x_pixel, int y_pixel, int  flag, bool **array_of_paint_pixel); // flag = 0 в тени, flag = 1 в цвете +
	void draw_shadow();//-

	void move(int flag);//ƒобавить дл€ всех провеку на выход за границы экрана, если выход, то совершаем обратное преобразование 
	void scale(int flag);
	void roat(int flag, int flag2);

	bool going_abroad();
};
