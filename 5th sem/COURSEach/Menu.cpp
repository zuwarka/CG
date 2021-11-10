#include "Menu.h"

int Menu::menu()
{
		Figure paral_1(0), paral_2(1);//Создание обёкта 2 пар-дов, инициализация по умолчанию
		char choice = '0';
		
		do
		{
			system("cls");
			
			cout << "Manual:\n\nThe forth laba of CG!" << endl;
			//cout << "WASD - moving" << endl;
			cout << "Q - rotating ccw Z\nE - rotating cw Z" << endl;
			cout << "W - rotating ccw X\nS - rotating cw X" << endl;
			cout << "D - rotating ccw Y\nA - rotating cw Y" << endl;
			cout << "Z - scaling less\nC - scaling more" << endl;
			cout << "T - moving up Y\nG - moving down Y" << endl;
			cout << "F - moving right X\nH - moving left X" << endl;
			cout << "R - moving forward Z\nY - moving backward Z" << endl;
    		cout << "\n--------------------------------\n\n";

			
			cout << "Please, pick a figure:" << endl;
			cout << "1 - First figure" << endl << "2 - Second figure" << endl;
			cout << "ESC - Back to menu" << endl;
			cout << "0 - Exit from programm" << endl;

			choice = _getch();

			if (choice == '1') //Меню первого параллелепипеда
				menu_par(par1, par2);
			
			if (choice == '2') //Меню второго параллелепипеда
				menu_par(par2, par1);

		} while (choice != '0');
		return 0;
}

void Menu::menu_par(Figure & obg, Figure &par)
{
	initwindow(300, 300);
	par.draw();
	obg.draw();
	
	do {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) //Вращать вокруг х против часовой
		{
			clearviewport();
			obg.roat(1, 0);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) //Вращать вокруг х по часовой
		{
			clearviewport();
			obg.roat(1, 1);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) //Вращать вокруг y по часовой
		{
			clearviewport();
			obg.roat(2, 0);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) //Вращать вокруг y против часовой
		{
			clearviewport();
			obg.roat(2, 1);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) //Вращать вокруг z против часовой
		{
			clearviewport();
			obg.roat(3, 0);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) //Вращать вокруг z по часовой
		{
			clearviewport();
			obg.roat(3, 1);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000) //Масштабирование минус
		{
			clearviewport();
			obg.scale(1);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'C') & 0x8000) //Масштабирование плюс
		{
			clearviewport();
			obg.scale(3);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'R') & 0x8000) //Перемещение по Z вперед
		{
			clearviewport();
			obg.move(9);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'T') & 0x8000) //Перемещение по Y вверх
		{
			clearviewport();
			obg.move(2);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'Y') & 0x8000) //перемещение по Z назад
		{
			clearviewport();
			obg.move(7);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'G') & 0x8000) //Перемещение по Y вниз
		{
			clearviewport();
			obg.move(8);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'F') & 0x8000) //Перемещение по X вправо
		{
			clearviewport();
			obg.move(6);
			par.draw();
			obg.draw();
		}
		if (GetAsyncKeyState((unsigned short)'H') & 0x8000) //перемещение по X влево
		{
			clearviewport();
			obg.move(4);
			par.draw();
			obg.draw();
		}
	} while (1);

	closegraph();
}

