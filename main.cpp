/*  Sudoker-game is a terminal based implemntation of Sudoku
 *  using the Sudoker library. Works only on Windows systems.
 *	Copyright(C) 2015  Ivan Rubinson
 *
 *	This program is free software : you can redistribute it and / or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <Sudoker/sudoker.h>
#include <Windows.h>
#include <conio.h>

void setCursor(const int x, const int y)
{
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

int main(int argc, char* argv[])
{
	Sudoker::SudokuGrid grid;
	int cursor_x = grid.width/2;
	int cursor_y = grid.height/2;
	bool victory = false;

	system("CLS");
	bool running = true;
	while (running)
	{
		// Render
		setCursor(0, 0);
		for (int y = 0; y < grid.height; ++y)
		{
			for (int x = 0; x < grid.width; ++x)
			{
				int value = grid.get(grid.position(x, y));
				if (value == 0)
				{
					std::cout << "  ";
				}
				else
				{
					std::cout << value << " ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << "INSTRUCTIONS:" << std::endl;
		std::cout << "Use WASD to move the cursor." << std::endl;
		std::cout << "Press a number to write it in." << std::endl;
		std::cout << "Press 'Q' to quit." << std::endl;

		// Process input
		char input;
		setCursor(cursor_x*2, cursor_y); // cursor_x is multiplied by 2 because of ' ' seperation between numbers
		input = _getch();
		switch (input)
		{
		case('w') :
		case('W') :
			if (cursor_y - 1 >= 0)
				--cursor_y;
			break;
		case('s') :
		case('S') :
			if (cursor_y + 1 < grid.height)
				++cursor_y;
			break;
		case('a') :
		case('A') :
			if (cursor_x - 1 >= 0)
				--cursor_x;
			break;
		case('d') :
		case('D') :
			if (cursor_x + 1 < grid.width)
				++cursor_x;
			break;
		case('q') :
		case('Q') :
			running = false;
			break;
		default:
			if (input >= '0' && input <= '9')
			{
				if (grid.isValid(grid.position(cursor_x, cursor_y), input - '0'))
				{
					grid.set(grid.position(cursor_x, cursor_y), input - '0');
				}
			}
		}

		// Check for victory
		if (grid.isFull())
		{
			victory = true;
			running = false;
		}
	}
	system("CLS");
	if (victory)
	{
		std::cout << "You have won!" << std::endl;
	}
	std::cout << "Press any key to continue . . ." << std::endl;
	_getch();
	return 0;
}
