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
#include <conio.h>
#include <ctime>

void setCursor(const int x, const int y)
{
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}


void render(Sudoker::UniquelySolvableSudokuGrid problem, Sudoker::SudokuGrid solution)
{
	setCursor(0, 0);
	for (unsigned int y = 0; y < solution.height; ++y)
	{
		for (unsigned int x = 0; x < solution.width; ++x)
		{
			const unsigned int square_x = x / 3;
			const unsigned int square_y = y / 3;
			if (square_x == square_y || (square_x == 0 && square_y == 2) || (square_x == 2 && square_y == 0))
			{
				if (problem.get(problem.position(x, y)) != 0)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
			}
			else
			{
				if (problem.get(problem.position(x, y)) != 0)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				}
			}

			int value = solution.get(solution.position(x, y));
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
}

int main(int argc, char* argv[])
{
	std::cout << "Generating puzzle...";
	srand(time(NULL));
	Sudoker::UniquelySolvableSudokuGrid problem = Sudoker::generateUnique(Sudoker::DIFFICULTY_HARD);
	Sudoker::SudokuGrid solution = problem;
	unsigned int cursor_x = solution.width/2;
	unsigned int cursor_y = solution.height/2;
	bool victory = false;

	system("CLS");
	bool running = true;
	while (running)
	{
		// Render
		render(problem, solution);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "INSTRUCTIONS:" << std::endl;
		std::cout << "Use WASD to move the cursor." << std::endl;
		std::cout << "Press a number to write it in." << std::endl;
		std::cout << "Press 'V' to solve." << std::endl;
		std::cout << "Press 'Q' to quit." << std::endl;

		// Process input
		char input;
		setCursor(cursor_x*2, cursor_y); // cursor_x is multiplied by 2 because of ' ' seperation between numbers
		input = _getch();
		switch (input)
		{
		case('w') :
		case('W') :
			if (cursor_y > 0)
				--cursor_y;
			break;
		case('s') :
		case('S') :
			if (cursor_y + 1 < solution.height)
				++cursor_y;
			break;
		case('a') :
		case('A') :
			if (cursor_x > 0)
				--cursor_x;
			break;
		case('d') :
		case('D') :
			if (cursor_x + 1 < solution.width)
				++cursor_x;
			break;
		case('v') :
		case('V') :
			solution = Sudoker::solve(problem);
			break;
		case('q') :
		case('Q') :
			running = false;
			break;
		default:
			if (input >= '0' && input <= '9')
			{
				if (problem.get(problem.position(cursor_x, cursor_y)) == 0 && solution.isValid(solution.position(cursor_x, cursor_y), input - '0'))
				{
					solution.set(solution.position(cursor_x, cursor_y), input - '0');
				}
			}
		}

		// Check for victory
		if (solution.isFull())
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
