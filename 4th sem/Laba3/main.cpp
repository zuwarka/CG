#include <iostream>
#include <graphics.h>
#include <cmath>
#include <conio.h>

using namespace std;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

const int N = 5; //для пятиугольника - пять строк координат
const int M = 3;
const float DX = 5.0;
const float DY = 5.0;
const float ALPHA = 0.2;
const float S1 = 1.5;
const float S2 = 0.9;
const int Color = 14; //желтый

////////////////////////////////////////////////////////////////////////////////////////////////

//функция перемножения матриц - немноо меняется относительно перво лабы
void multing(float lin[N][M], float matrix[M][M])
{
    float res[N][M] = {0, 0, 0}; //result

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
    float m_move[M][M] = { {1, 0, 0}, {0, 1, 0}, {dx, dy, 1} }; //матрица перемещения
    multing(lin, m_move); //умножение матрицы отрезка на матрицу перемещения
}

//функция масштабирования (на одинаковый коэффициент)
void scaling(float lin[N][M], float S)
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

    float m_scale[M][M] = { {S, 0, 0}, {0, S, 0}, {0, 0, 1} }; //матрица масштабирования
    float first_rot[M][M] = { {1, 0, 0}, {0, 1, 0}, {-xc, -yc, 1} };
    float second_rot[M][M] = { {1, 0, 0}, {0, 1, 0}, {xc, yc, 1} };
    multing(lin, first_rot);
    multing(lin, m_scale); //умножение матрицы отрезка на матрицу масштабирования
    multing(lin, second_rot);
}

//функция поворота 
void rotating(float lin[N][M], float angle)
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

    float first_rot[M][M] = { {1, 0, 0}, {0, 1, 0}, {-xc, -yc, 1} };
    float second_rot[M][M] = { {1, 0, 0}, {0, 1, 0}, {xc, yc, 1} };
    float m_rotate[M][M] = { {cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1} };
    multing(lin, first_rot);
    multing(lin, m_rotate);
    multing(lin, second_rot);
}

void fill_point(float board_pix, float new_pix, float x_lin, float y_lin)
{	
	if (getpixel(x_lin, y_lin) != board_pix && getpixel(x_lin, y_lin) != new_pix)
	{
		putpixel (x_lin, y_lin, new_pix);
		fill_point(board_pix, new_pix, x_lin+1, y_lin);
		fill_point(board_pix, new_pix, x_lin, y_lin+1);
		fill_point(board_pix, new_pix, x_lin-1, y_lin);
		fill_point(board_pix, new_pix, x_lin, y_lin-1);
	}
}

void bres_line(int x0, int y0, int x1, int y1)
{
	//int x0 = lin[0][0], x1 = lin[1][0], y0 = lin[0][1], y1 = lin[1][1];
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

    //setcolor(Color);
    putpixel(x1, y1, Color);

    while (x0 != x1 || y0 != y1)
    {
        //setcolor(Color);
   	    putpixel(x0, y0, Color);
        int err2 = err * 2;
        if (err2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

//функция рисования фигуры
void draw(float lin[N][M])
{
	setcolor(Color);
	int x0 = lin[0][0], y0 = lin[0][1], x1 = lin[1][0], y1 = lin[1][1];
	bres_line(x0, y0, x1, y1);
	
	int x2 = lin[1][0], y2 = lin[1][1], x3 = lin[2][0], y3 = lin[2][1];
	bres_line(x2, y2, x3, y3);
	
	int x4 = lin[2][0], y4 = lin[2][1], x5 = lin[3][0], y5 = lin[3][1];
	bres_line(x4, y4, x5, y5);
	
	int x6 = lin[3][0], y6 = lin[3][1], x7 = lin[4][0], y7 = lin[4][1];
	bres_line(x6, y6, x7, y7);
	
	int x8 = lin[4][0], y8 = lin[4][1];
	bres_line(x8, y8, x0, y0);
    
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
	
	fill_point(Color, Color, xc, yc);
}

int main(int argc, char* argv[])
{
			initwindow(SCREEN_WIDTH, SCREEN_HEIGHT);
        	//точки идут в матричной форме по однородным координатам
       		float AB[N][M] = { {50, 10, 1}, {70, 30, 1}, {60, 40, 1}, {40, 40, 1}, {30, 30, 1} }; //AB - пятиугольник
            float xc = 0.0, yc = 0.0;

            cout << "Manual:\n\nSDL line (yellow):\nWASD - moving\nQE - rotating\nZ - increase\nC - decrease\n";
            cout << "\n--------------------------------------------------------\n\n";
            cout << "\nExit - 0\n";

			draw(AB);
			

            char c;
            int exit = 1;
            while (exit == 1)
            {
            	
    		for (int i = 0; i < N; i++)
   			{
    			xc += AB[i][0];
			}
   			xc = xc / N;
   			for (int i = 0; i < N; i++)
   			{
    			yc += AB[i][1];
			}
				yc = yc / N;

                c = _getch();
                //cin >> c;
                switch (c)
                {
                case 'w': //w - вверх желтую линию
                    clearviewport();
                    moving(AB, 0, -DY);
                    draw(AB);
                    break;
                case 'a': //a - влево желтую линию
                    clearviewport();
                    moving(AB, -DX, 0);
                    draw(AB);
                    break;
                case 's': //s - вниз желтую
                    clearviewport();
                    moving(AB, 0, DY);
                    draw(AB);
                    break;
                case 'd': //d - вправо желтую
                    clearviewport();
                    moving(AB, DX, 0);
                    draw(AB);
                    break;
                case 'q': //q - поврот желтой
                    clearviewport();
                    rotating(AB, ALPHA);
                    draw(AB);
                    break;
                case 'e': //e - поворот желтой
                    clearviewport();
                    rotating(AB, -ALPHA);
                    draw(AB);
                    break;
                case 'z': //z - масштабирование желтой
                    clearviewport();
                    scaling(AB, S1);
                    draw(AB);
                    break;
                case 'c': //c - то же
                    clearviewport();
                    scaling(AB, S2);
                    draw(AB);
                    break;
                case '0': //выход
                    exit = 0;
                    break;
                }
            }
    closegraph();
    return 0;
}
