#include <iostream>
#include <cmath>
#include <conio.h>
#include <SDL.h>

using namespace std;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

const int N = 2;
const int M = 3;
const float DX = 5.0;
const float DY = 5.0;
const float ALPHA = 0.1;
const float S1 = 1.5;
const float S2 = 0.9;

////////////////////////////////////////////////////////////////////////////////////////////////

//функция брезенхэма, передает матрицу координат концов отрезка, указатель на рендер (рабочее окно)
void bres_line(float lin[N][M], SDL_Renderer *render)
{
    int x0 = lin[0][0], x1 = lin[1][0], y0 = lin[0][1], y1 = lin[1][1];
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

    SDL_SetRenderDrawColor(render, 0, 139, 139, 255); //цвет пикселя - темный циан (dark cyan)
    SDL_RenderDrawPoint(render, x1, y1);

    while (x0 != x1 || y0 != y1)
    {
        SDL_SetRenderDrawColor(render, 0, 139, 139, 255); //цвет пикселя - темный циан (dark cyan)
        SDL_RenderDrawPoint(render, x0, y0);
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

//функция ввода координат концов отрезка линии
void input(float lin[N][M])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M - 1; j++)
        {
            cin >> lin[i][j];
        }
    }
}

//функция перемножения матриц
void multing(float lin[N][M], float matrix[M][M])
{
    float res[N][M] = { {0, 0, 0}, {0, 0, 0} }; //result

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
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
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

//функция удаления поверхности (рендера) и создания новой после выполнения той или иной функции
void draw_and_view(SDL_Renderer *ren, float AB[N][M], float CD[N][M])
{
    //три строки - рисуем поверхность заново
    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    //три строки - рисуем линии заново и отображаем их
    SDL_RenderDrawLine(ren, AB[0][0], AB[0][1], AB[1][0], AB[1][1]);
    bres_line(CD, ren);
    SDL_RenderPresent(ren);
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Window* win = NULL;
        SDL_Renderer* ren = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &win, &ren) == 0)
        {
            SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);

            //отрезки идут в матричной форме по однородным координатам
            float AB[N][M] = { {0, 0, 1}, {0, 0, 1} }; //AB - отрезок встроенной функции
            float CD[N][M] = { {0, 0, 1}, {0, 0, 1} }; //CD - отрезок брезенхэма

            cout << "Input coordinates of AB-line" << endl; //sdl line
            input(AB);
            cout << "Input coordinates of CD-line" << endl; //bresenham line
            input(CD);
            system("cls");

            cout << "Manual:\n\nSDL line (yellow):\nWASD - moving\nQE - rotating\nZ - increase\nC - decrease\n";
            cout << "\n--------------------------------------------------------\n\n";
            cout << "Bresenham line (dark cyan):\nTFGH - moving\nRY - rotating\nV - increase\nN - decrease\n";
            cout << "\n---------------------------------------------------------\n";
            cout << "\nExit - 0\n";

            char c; //ЗДЕСЬ МНЕ СТЫДНО ЗА ИСПОЛЬЗОВАНИЕ ЧАРА
            int exit = 1;
            while (exit == 1)
            {
                SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
                SDL_RenderDrawLine(ren, AB[0][0], AB[0][1], AB[1][0], AB[1][1]);
                bres_line(CD, ren);
                SDL_RenderPresent(ren);

                C = _getch();
                
                switch (c)
                {
                case 'w': //w - вверх желтую линию
                    SDL_RenderClear(ren);
                    moving(AB, 0, -DY);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'a': //a - влево желтую линию
                    SDL_RenderClear(ren);
                    moving(AB, -DX, 0);
                    draw_and_view(ren, AB, CD);
                    break;
                case 's': //s - вниз желтую
                    SDL_RenderClear(ren);
                    moving(AB, 0, DY);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'd': //d - вправо желтую
                    SDL_RenderClear(ren);
                    moving(AB, DX, 0);
                    draw_and_view(ren, AB, CD);
                    break;
                case 't': //t - вверх циан
                    SDL_RenderClear(ren);
                    moving(CD, 0, -DY);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'f': //f - влево циан
                    SDL_RenderClear(ren);
                    moving(CD, -DX, 0);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'g': //g - вниз циан
                    SDL_RenderClear(ren);
                    moving(CD, 0, DY);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'h': //h - вправо циан
                    SDL_RenderClear(ren);
                    moving(CD, DX, 0);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'q': //q - поврот желтой
                    SDL_RenderClear(ren);
                    rotating(AB, ALPHA);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'e': //e - поворот желтой
                    SDL_RenderClear(ren);
                    rotating(AB, -ALPHA);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'r': //r - поворот циан
                    SDL_RenderClear(ren);
                    rotating(CD, ALPHA);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'y': //y - поворот циан
                    SDL_RenderClear(ren);
                    rotating(CD, -ALPHA);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'z': //z - масштабирование желтой
                    SDL_RenderClear(ren);
                    scaling(AB, S1);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'c': //c - то же
                    SDL_RenderClear(ren);
                    scaling(AB, S2);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'v': //v - масштабирование циан
                    SDL_RenderClear(ren);
                    scaling(CD, S1);
                    draw_and_view(ren, AB, CD);
                    break;
                case 'n': //n - то же
                    SDL_RenderClear(ren);
                    scaling(CD, S2);
                    draw_and_view(ren, AB, CD);
                    break;
                case '0': //выход
                    exit = 0;
                    break;
                }
            }
        }

        if (ren) 
        {
            SDL_DestroyRenderer(ren);
        }
        if (win) 
        {
            SDL_DestroyWindow(win);
        }
    }
    SDL_Quit();
    return 0;
}
