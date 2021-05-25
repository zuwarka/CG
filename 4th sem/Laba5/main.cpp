#include <iostream>
#include <graphics.h>
#include <math.h>
#include <conio.h>

using namespace std;

#define MAXARR 300
#define MAXLST 300

float* Py;
float* Px;

static int KOD, NWER;
static float* pt_X;
static float* pt_Y;
static int ntek;
static int idlspi;
static int IYREB[MAXLST];
static float RXREB[MAXLST];
static float RPRIR[MAXLST];

int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 600;

const int N = 8; //для параллелепипеда - восемь строк координат, восемь точек
const int M = 4; //три координаты в трехмерном пространстве + одна однородная
const float DX = 5;
const float DY = 5;
const float ALPHA = 0.087;
const float S1 = 1.01;
const float S2 = 0.95;
const int Color = 5; //lightcyan

////////////////////////////////////////////////////////////////////////////////////////////////

void FILSTR(int kod, int iy, float ixn, float ixk);
void SORT(int n, float *iarr);
static void OBRREB(int isd);
void V_FP0(int pixel, int kol, float* Px, float* Py);

void multing(float lin[N][M], float matrix[M][M]);
void moving(float lin[N][M], float dx, float dy);
void scaling(float lin[N][M], float S);
void rotating_z(float lin[N][M], float angle);
void rotating_x(float lin[N][M], float angle);
void rotating_y(float lin[N][M], float angle);
void draw(float fig1[N][M]);
void painter(float fig[N][M]);

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
	setcolor(Color + 1);
	line(fig1[4][0], fig1[4][1], fig1[5][0], fig1[5][1]);
	line(fig1[5][0], fig1[5][1], fig1[6][0], fig1[6][1]);
	line(fig1[6][0], fig1[6][1], fig1[7][0], fig1[7][1]);
	line(fig1[7][0], fig1[7][1], fig1[4][0], fig1[4][1]);
	setcolor(Color + 2);
	line(fig1[0][0], fig1[0][1], fig1[4][0], fig1[4][1]);
	line(fig1[1][0], fig1[1][1], fig1[5][0], fig1[5][1]);
	line(fig1[2][0], fig1[2][1], fig1[6][0], fig1[6][1]);
	line(fig1[3][0], fig1[3][1], fig1[7][0], fig1[7][1]);
	painter(fig1);
}

void V_FP0(int pixel, int kol, float* Px, float* Py)

{
int ii, jj, kk;
int iymin;
int iymax;
int iysled;
int iytek;
int ikledg;
int ibgind;
int iedg[MAXARR];
int inom[MAXARR];
float irabx[MAXLST];
KOD= pixel;
NWER= kol;
pt_X= Px;
pt_Y= Py;

for (int i = 1; i <= kol; ++i) 
{
iedg[i] = Py[i];
inom[i] = i;
}

for (int i = 1; i <= kol; ++i) 
{
iymin = iedg[i];
ntek = i;
for (int j = i + 1; j <= kol; ++j)
{
if (iedg[j] < iymin) 
{
iymin = iedg[j];
ntek = j;
}
}

if (ntek != i) 
{
iedg[ntek] = iedg[i];
iedg[i] = iymin;
iymin = inom[ntek];
inom[ntek] = inom[i];
inom[i] = iymin;
}
}

idlspi = 0;
ibgind = 1;
iytek = iedg[1];
iymax = iedg[kol];

for (;;) 
{

ikledg = 0;
for (int i = ibgind; i <= kol; ++i)
{
if (iedg[i] != iytek)
break;
else
ikledg++;
}

for (int i = 1; i <= ikledg; ++i) 
{
ntek = inom[ibgind + i - 1];
OBRREB(-1);
OBRREB(+1);
}

if (!idlspi) break;
ii = ibgind + ikledg;
iysled = iymax;
if (ii < kol)
iysled = iedg[ii];

for (double i = iytek; i <= iysled; ++i) 
{
for (int j = 1; j <= idlspi; ++j)
irabx[j] = RXREB[j];
SORT(idlspi, irabx + 1);
for (int j = 1; j <= idlspi - 1; j += 2)
FILSTR(pixel, i, round(irabx[j]), round(irabx[j + 1]));
if (i == iysled) continue;
for (int j = 1; j <= idlspi; ++j)
RXREB[j] = RXREB[j] + RPRIR[j];
}

if (iysled == iymax) break;
for (ii = 0; ii <= idlspi; ii++)
{
if (IYREB[ii] != iysled) continue;
--idlspi;
for (jj = ii; jj <= idlspi; ++jj) 
{
IYREB[jj] = IYREB[kk = jj + 1];
RXREB[jj] = RXREB[kk];
RPRIR[jj] = RPRIR[kk];
}
}

ibgind += ikledg;

iytek = iysled;

}

}

static void OBRREB(int isd)

{

int inext, iyt, ixt;

float xt, xnext, dy;

inext = ntek + isd;

if (inext < 1)

inext = NWER;

if (inext > NWER)

inext = 1;

dy = pt_Y[inext] - pt_Y[ntek];

if (dy >= 0) {

xnext = pt_X[inext];

xt = pt_X[ntek];

if (dy != 0) {

idlspi++;

IYREB[idlspi] = pt_Y[inext];

RXREB[idlspi] = xt;

RPRIR[idlspi] = (xnext - xt) / dy;

}

else {

iyt = pt_Y[ntek];

inext = xnext;

ixt = xt;

FILSTR(KOD, iyt, round(inext), round(ixt));

}

}

}

void SORT(int n, float *iarr)

{

int l;

float k, min;

for (int i = 0; i < n; ++i) {

min = iarr[l = i];

for (int j = i + 1; j < n; ++j)

if ((k = iarr[j]) < min) {

l = j;

min = k;

}

if (l != i) {

iarr[l] = iarr[i];

iarr[i] = min;

}

}

}

void FILSTR(int kod, int iy, float ixn, float ixk){

while (ixn <= ixk) putpixel(ixn++, iy,kod);

}

//алгоритм удаления с зед-буфером
void painter(float fig[N][M]){

float Pol[6] = {}, Pol1[7] = {}, min=0;

int ntek, inom[7] = {};

//??????? ???????? ???????

Pol[0] = (fig[0][2] + fig[1][2] + fig[2][2] + fig[3][2])/4; //ABCD

Pol[1] = (fig[4][2] + fig[5][2] + fig[6][2] + fig[7][2])/4; //A1B1C1D1

Pol[2] = (fig[0][2] + fig[4][2] + fig[3][2] + fig[7][2])/4; //AA1DD1

Pol[3] = (fig[7][2] + fig[3][2] + fig[2][2] + fig[6][2])/4; //CC1DD1

Pol[4] = (fig[1][2] + fig[5][2] + fig[2][2] + fig[6][2])/4; //BB1CC1

Pol[5] = (fig[1][2] + fig[4][2] + fig[0][2] + fig[5][2])/4; //AA1BB1

for (int i = 1; i <= 6; ++i) {

Pol1[i] = Pol[i - 1];

inom[i] = i;

}

for (int i = 1; i <= 6; ++i) {

min = Pol1[i];

ntek = i;

for (int j = i + 1; j <= 6; ++j)

if (Pol1[j] <= min) {

min = Pol1[j];

ntek = j;

}

if (ntek != i) {

Pol1[ntek] = Pol1[i];

Pol1[i] = min;

min = inom[ntek];

inom[ntek] = inom[i];

inom[i] = min;

}

}

int num;

for (int i = 1; i <= 7; i++) {

Px = new float[4 + 1];

Py = new float[4 + 1];

num = inom[i];

switch (num)

{

case 1:

Px[1] = fig[0][0];

Py[1] = fig[0][1];

Px[2] = fig[1][0];

Py[2] = fig[1][1];

Px[3] = fig[2][0];

Py[3] = fig[2][1];

Px[4] = fig[3][0];

Py[4] = fig[3][1];

V_FP0(Color, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

case 2:

Px[1] = fig[4][0];

Py[1] = fig[4][1];

Px[2] = fig[5][0];

Py[2] = fig[5][1];

Px[3] = fig[6][0];

Py[3] = fig[6][1];

Px[4] = fig[7][0];

Py[4] = fig[7][1];

V_FP0(Color+1, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

case 3:

Px[1] = fig[0][0];

Py[1] = fig[0][1];

Px[2] = fig[3][0];

Py[2] = fig[3][1];

Px[3] = fig[7][0];

Py[3] = fig[7][1];

Px[4] = fig[4][0];

Py[4] = fig[4][1];

V_FP0(Color+2, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

case 4:

Px[1] = fig[7][0];

Py[1] = fig[7][1];

Px[2] = fig[3][0];

Py[2] = fig[3][1];

Px[3] = fig[2][0];

Py[3] = fig[2][1];

Px[4] = fig[6][0];

Py[4] = fig[6][1];

V_FP0(Color+3, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

case 5:

Px[1] = fig[1][0];

Py[1] = fig[1][1];

Px[2] = fig[5][0];

Py[2] = fig[5][1];

Px[3] = fig[6][0];

Py[3] = fig[6][1];

Px[4] = fig[2][0];

Py[4] = fig[2][1];

V_FP0(Color+4, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

case 6:

Px[1] = fig[0][0];

Py[1] = fig[0][1];

Px[2] = fig[4][0];

Py[2] = fig[4][1];

Px[3] = fig[5][0];

Py[3] = fig[5][1];

Px[4] = fig[1][0];

Py[4] = fig[1][1];

V_FP0(Color+5, 4, Px, Py);

delete[] Px;

delete[] Py;

break;

}

}

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
