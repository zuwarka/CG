#pragma once
#include "Figure.h"

class Menu
{
	Figure par1, par2;

public:
	Menu() : par1(0), par2(1) {};

	int menu();
	void menu_par(Figure &par, Figure &par2);
};

