#include <iostream>
#include <graphics.h>
#include <math.h>
#include <conio.h>

using namespace std;

int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 600;

const int N = 8; //для параллелепипеда - восемь строк координат, восемь точек
const int M = 4; //три координаты в трехмерном пространстве + одна однородная
const float DX = 5;
const float DY = 5;
const float ALPHA = 0.087;
const float S1 = 1.01;
const float S2 = 0.95;
const int Color = 11; //lightcyan

////////////////////////////////////////////////////////////////////////////////////////////////

void multing(float lin[N][M], float matrix[M][M]);
void moving(float lin[N][M], float dx, float dy);
void scaling(float lin[N][M], float S);
void rotating_z(float lin[N][M], float angle);
void rotating_x(float lin[N][M], float angle);
void rotating_y(float lin[N][M], float angle);
void draw(float fig1[N][M]);


//функция перемножения матриц - немноо меняется относительно перво лабы
void multing(float lin[N][M], float matrix[M][M])
{
    float res[N][M] = {0, 0, 0, 0}; //result

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            for (int k = 0; k < M; k++)
            {
            	res[i][j] += (lin[i][k] * matrix[k][j]);
			}
        }
    }

    //return res
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
        	lin[i][j] = res[i][j];
		}
    }
}

//функция перемещения отрезка
void moving(float lin[N][M], float dx, float dy)
{
	float dz = 5; //dz = dx = dy
    float m_move[M][M] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {dx, dy, dz, 1} }; //матрица перемещения
    multing(lin, m_move); //умножение матрицы отрезка на матрицу перемещения
}

//функция масштабирования (на одинаковый коэффициент)
void scaling(float lin[N][M], float S)
{
    float xc = 0.0, yc = 0.0, zc = 0.0;
    for (int i = 0; i < N; i++)
    {
        xc += lin[i][0];
    }
    xc = xc / N;
    for (int i = 0; i < N; i++)
    {
        yc += lin[i][1];
    }
    yc = yc / N;
    for (int i = 0; i < N; i++)
    {
        zc += lin[i][2];
    }
    zc = zc / N;

    float m_scale[M][M] = { {S, 0, 0, 0}, {0, S, 0, 0}, {0, 0, S, 0}, {xc*(1 - S), yc*(1 - S), zc*(1 - S), 1} }; //матрица масштабирования
    multing(lin, m_scale); //умножение матрицы отрезка на матрицу масштабирования
}

//функция поворота вокруг оси Z
void rotating_z(float lin[N][M], float angle)
{
    float xc = 0.0, yc = 0.0;
    for (int i = 0; i < N; i++)
    {
        xc += lin[i][0];
    }
    xc = xc / N;
    for (int i = 0; i < N; i++)
    {
        yc += lin[i][1];
    }
    yc = yc / N;

    float m_rotate[M][M] = { {cos(angle), sin(angle), 0, 0}, {-sin(angle), cos(angle), 0, 0}, {0, 0, 1, 0}, {xc*(1 - cos(angle)) + yc * sin(angle), yc*(1 - cos(angle)) - xc * sin(angle), 0, 1} };
    multing(lin, m_rotate);
}

//функция поворота вокруг оси X
void rotating_x(float lin[N][M], float angle)
{
    float yc = 0.0, zc = 0.0;
    for (int i = 0; i < N; i++)
    {
        yc += lin[i][1];
    }
    yc = yc / N;
    for (int i = 0; i < N; i++)
    {
        zc += lin[i][2];
    }
    zc = zc / N;

    float m_rotate[M][M] = { {1, 0, 0, 0}, {0, cos(angle), sin(angle), 0}, {0, -sin(angle), cos(angle), 0}, {0, yc*(1 - cos(angle)) + zc * sin(angle), zc*(1 - cos(angle)) - yc * sin(angle), 1} };
    multing(lin, m_rotate);
}

//функция поворота вокруг оси Y
void rotating_y(float lin[N][M], float angle)
{
    float xc = 0.0, zc = 0.0;
    for (int i = 0; i < N; i++)
    {
        xc += lin[i][0];
    }
    xc = xc / N;
    for (int i = 0; i < N; i++)
    {
        zc += lin[i][2];
    }
    zc = zc / N;

    float m_rotate[M][M] = { {cos(angle), 0, -sin(angle), 0}, {0, 1, 0, 0}, {sin(angle), 0, cos(angle), 0}, {xc*(1 - cos(angle)) - zc * sin(angle), 0, zc*(1 - cos(angle)) + xc * sin(angle), 1} };
    multing(lin, m_rotate);
}

//функция рисования фигуры
void draw(float fig1[N][M])
{
	clearviewport();
	setcolor(Color);
	line(fig1[0][0], fig1[0][1], fig1[1][0], fig1[1][1]);
	line(fig1[1][0], fig1[1][1], fig1[2][0], fig1[2][1]);
	line(fig1[2][0], fig1[2][1], fig1[3][0], fig1[3][1]);
	line(fig1[3][0], fig1[3][1], fig1[0][0], fig1[0][1]);
	line(fig1[4][0], fig1[4][1], fig1[5][0], fig1[5][1]);
	line(fig1[5][0], fig1[5][1], fig1[6][0], fig1[6][1]);
	line(fig1[6][0], fig1[6][1], fig1[7][0], fig1[7][1]);
	line(fig1[7][0], fig1[7][1], fig1[4][0], fig1[4][1]);
	line(fig1[0][0], fig1[0][1], fig1[4][0], fig1[4][1]);
	line(fig1[1][0], fig1[1][1], fig1[5][0], fig1[5][1]);
	line(fig1[2][0], fig1[2][1], fig1[6][0], fig1[6][1]);
	line(fig1[3][0], fig1[3][1], fig1[7][0], fig1[7][1]);
}

int main()
{
	initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    //точки идут в матричной форме по однородным координатам
    //параллелепипед
    float AB[N][M] = { {100, 400, 100, 1}, {100, 200, 100, 1}, {400, 200, 100, 1}, {400, 400, 100, 1}, {100, 400, 300, 1}, {100, 200, 300, 1}, {400, 200, 300, 1}, {400, 400, 300, 1} }; 

    cout << "Manual:\n\nThe forth laba of CG!\nWASD - moving\nQE - scaling\nZ - rotating cw Z\nC - rotating cw Y\nX - rotating cw X\nR - rotating ccw Z\nT - rotating ccw X\nY - rotating ccw Y";
    cout << "\n--------------------------------------------------------\n\n";
    cout << "\nExit - esc\n";

	draw(AB);
	
	do {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			moving(AB, 0, -DY);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			moving(AB, 0, DY);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			moving(AB, -DX, 0);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			moving(AB, DX, 0);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			scaling(AB, S1);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
		{
			scaling(AB, S2);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
		{
			rotating_z(AB, ALPHA);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'X') & 0x8000)
		{
			rotating_x(AB, ALPHA);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'C') & 0x8000)
		{
			rotating_y(AB,ALPHA);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
		{
			rotating_z(AB, -ALPHA);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'T') & 0x8000)
		{
			rotating_x(AB, -ALPHA);
			draw(AB);
		}
		if (GetAsyncKeyState((unsigned short)'Y') & 0x8000)
		{
			rotating_y(AB, -ALPHA);
			draw(AB);
		}
	} while (1);
	
    closegraph();
    return 0;
}
