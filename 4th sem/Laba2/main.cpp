#include <iostream>
#include <cmath>
#include <conio.h>
#include <SDL.h>

using namespace std;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

const int N = 5; //для пятиугольника - пять строк координат
const int M = 3;
const float DX = 5.0;
const float DY = 5.0;
const float ALPHA = 0.1;
const float S1 = 1.5;
const float S2 = 0.9;

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

//функция удаления поверхности (рендера) и создания новой после выполнения той или иной функции
void draw_and_view(SDL_Renderer *ren, float AB[N][M])
{
    //три строки - рисуем поверхность заново
    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    //три строки - рисуем линии заново и отображаем их
    SDL_RenderDrawLine(ren, AB[0][0], AB[0][1], AB[1][0], AB[1][1]);
    SDL_RenderPresent(ren);
}

//функция рисования фигуры
void draw_pentagram(float lin[N][M], SDL_Renderer* render)
{
    SDL_RenderDrawLine(render, lin[0][0], lin[0][1], lin[1][0], lin[1][1]);
    SDL_RenderDrawLine(render, lin[1][0], lin[1][1], lin[2][0], lin[2][1]);
    SDL_RenderDrawLine(render, lin[2][0], lin[2][1], lin[3][0], lin[3][1]);
    SDL_RenderDrawLine(render, lin[3][0], lin[3][1], lin[4][0], lin[4][1]);
    SDL_RenderDrawLine(render, lin[4][0], lin[4][1], lin[0][0], lin[0][1]);
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

            //точки идут в матричной форме по однородным координатам
            float AB[N][M] = { {50, 10, 1}, {70, 30, 1}, {60, 40, 1}, {40, 40, 1}, {30, 30, 1} }; //AB - пятиугольник

            cout << "Manual:\n\nSDL line (yellow):\nWASD - moving\nQE - rotating\nZ - increase\nC - decrease\n";
            cout << "\n--------------------------------------------------------\n\n";
            cout << "\nExit - 0\n";

            char c;
            int exit = 1;
            while (exit == 1)
            {
                SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
                draw_pentagram(AB, ren);
                SDL_RenderPresent(ren);

                c = _getch();
                switch (c)
                {
                case 'w': //w - вверх желтую линию
                    SDL_RenderClear(ren);
                    moving(AB, 0, -DY);
                    draw_and_view(ren, AB);
                    break;
                case 'a': //a - влево желтую линию
                    SDL_RenderClear(ren);
                    moving(AB, -DX, 0);
                    draw_and_view(ren, AB);
                    break;
                case 's': //s - вниз желтую
                    SDL_RenderClear(ren);
                    moving(AB, 0, DY);
                    draw_and_view(ren, AB);
                    break;
                case 'd': //d - вправо желтую
                    SDL_RenderClear(ren);
                    moving(AB, DX, 0);
                    draw_and_view(ren, AB);
                    break;
                case 'q': //q - поврот желтой
                    SDL_RenderClear(ren);
                    rotating(AB, ALPHA);
                    draw_and_view(ren, AB);
                    break;
                case 'e': //e - поворот желтой
                    SDL_RenderClear(ren);
                    rotating(AB, -ALPHA);
                    draw_and_view(ren, AB);
                    break;
                case 'z': //z - масштабирование желтой
                    SDL_RenderClear(ren);
                    scaling(AB, S1);
                    draw_and_view(ren, AB);
                    break;
                case 'c': //c - то же
                    SDL_RenderClear(ren);
                    scaling(AB, S2);
                    draw_and_view(ren, AB);
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
