#include <iostream>
#include <windows.h> // COORD HANDLE SetConsoleTextAttribute SetConsoleCursorPosition
#include <conio.h> // _getch
using namespace std;

enum GameObject : short { HALL, WALL, COIN, ENEMY, MEDICINE };
enum Color : short {
	BLACK, DARKBLUE, DARKGREEN, TURQUOISE, DARKRED,
	PURPLE, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN,
	CYAN, RED, PINK, YELLOW, WHITE
};
enum Key : short {
	LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80,
	ENTER = 13, SPACE = 32, ESCAPE = 27, BACKSPACE = 8
};

int main() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(0));
	rand();
	system("title Bomberman");

	// cursor hide
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 100; // 1-100
	SetConsoleCursorInfo(h, &cursor);

	const int HEIGHT = 25;
	const int WIDTH = 65;
	int map[HEIGHT][WIDTH] = {};
	// ASCII Table
	//for (int i = 0; i < 256; i++)
	//{
	//	cout << (char)i << " - " << i << "\n";
	//}

	// MODEL (логіка заповнення масиву)
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			map[y][x] = rand() % 5; // 0 1 2 3 4 

			// рамка по краях
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				map[y][x] = GameObject::WALL;

			// отвори для входу і виходу
			if (x == 0 && y == 2 ||
				x == 1 && y == 2 ||
				x == 2 && y == 2 ||

				x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3)
				map[y][x] = GameObject::HALL;

			if (map[y][x] == GameObject::ENEMY) {
				int r = rand() % 10; // 0 1 2 3 4 5 6 7 8 9
				if (r != 0) {
					map[y][x] = GameObject::HALL;
				}
			}
			if (map[y][x] == GameObject::MEDICINE) {
				int r = rand() % 10; // 0 1 2 3 4 5 6 7 8 9
				if (r != 0) {
					map[y][x] = GameObject::HALL;
				}
			}
		}
	}

	///////////////////////////////////////////////////////////

	// VIEW (подання - як буде виглядати ігрова локація)
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (map[y][x]) {
			case GameObject::HALL: // 0
				// як показати коридор
				SetConsoleTextAttribute(h, Color::BLACK);
				cout << " ";
				break;
			case GameObject::WALL: // 1
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				// cout << "#";
				cout << (char)178;
				break;
			case GameObject::COIN:
				SetConsoleTextAttribute(h, Color::YELLOW);
				cout << ".";
				break;
			case GameObject::ENEMY:
				SetConsoleTextAttribute(h, Color::RED);
				cout << "O";
				break;
			case GameObject::MEDICINE:
				SetConsoleTextAttribute(h, Color::PINK);
				cout << "+";
				break;
			}

		}
		cout << "\n";
	}

	////////////////////////////////////////////////////////////
	// показ ГГ
	COORD hero;
	hero.X = 0;
	hero.Y = 2;
	SetConsoleCursorPosition(h, hero);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << "O";

	int coins_collected = 0;

	COORD coins_info;
	coins_info.X = WIDTH + 1;
	coins_info.Y = 0;
	SetConsoleCursorPosition(h, coins_info);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << coins_collected;

	int health = 100;

	COORD health_info;
	health_info.X = WIDTH + 1;
	health_info.Y = 2;
	SetConsoleCursorPosition(h, health_info);
	SetConsoleTextAttribute(h, Color::PURPLE);
	cout << "HEALTH: ";
	SetConsoleTextAttribute(h, Color::PINK);
	cout << health;

	// ігровий двигунчик
	while (true) {
		int code = _getch(); // get character
		if (code == 224) {
			code = _getch();
		}

		COORD old_position = hero;
		bool has_been_moved = false; // false - пересування не було

		// cout << code << "\n";
		switch (code) {
		case Key::LEFT:
			if (hero.X > 0 && map[hero.Y][hero.X - 1] != GameObject::WALL &&
				map[hero.Y][hero.X - 1] != GameObject::MEDICINE) {
				has_been_moved = true;
				hero.X--;
			}
			else if (hero.X > 0 && map[hero.Y][hero.X - 1] != GameObject::WALL && health < 100)
			{
				has_been_moved = true;
				hero.X--;
			}
			break;
		case Key::RIGHT:
			if (map[hero.Y][hero.X + 1] != GameObject::WALL &&
				map[hero.Y][hero.X + 1] != GameObject::MEDICINE) {
				has_been_moved = true;
				hero.X++;
			}
			else if (map[hero.Y][hero.X + 1] != GameObject::WALL && health < 100)
			{
				has_been_moved = true;
				hero.X++;
			}
			break;
		case Key::UP:
			if (map[hero.Y - 1][hero.X] != GameObject::WALL &&
				map[hero.Y - 1][hero.X] != GameObject::MEDICINE) {
				has_been_moved = true;
				hero.Y--;
			}
			else if (map[hero.Y - 1][hero.X] != GameObject::WALL && health < 100)
			{
				has_been_moved = true;
				hero.Y--;
			}
			break;
		case Key::DOWN:
			if (map[hero.Y + 1][hero.X] != GameObject::WALL &&
				map[hero.Y + 1][hero.X] != GameObject::MEDICINE) {
				has_been_moved = true;
				hero.Y++;
			}
			else if (map[hero.Y + 1][hero.X] != GameObject::WALL && health < 100)
			{
				has_been_moved = true;
				hero.Y++;
			}
			break;
		}

		if (has_been_moved) {
			// стирання ГГ в "старих" координатах
			SetConsoleCursorPosition(h, old_position);
			SetConsoleTextAttribute(h, Color::BLACK);
			cout << " ";
			// перемальовка ГГ в нових координатах
			SetConsoleCursorPosition(h, hero);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << "O";
		}

		// приклад перевірки на перетинання з якимось об'єктом в лабірінті
		if (map[hero.Y][hero.X] == GameObject::COIN) {
			coins_collected++;
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD coins_info;
			coins_info.X = WIDTH + 1;
			coins_info.Y = 0;
			SetConsoleCursorPosition(h, coins_info);
			SetConsoleTextAttribute(h, Color::DARKYELLOW);
			cout << "COINS: ";
			SetConsoleTextAttribute(h, Color::YELLOW);
			cout << coins_collected;
		}

		if (map[hero.Y][hero.X] == GameObject::MEDICINE) {
			if (health + 25 > 100)
			{
				health = 100;
			}
			else
			{
				health += 25;
			}
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD health_info;
			health_info.X = WIDTH + 1;
			health_info.Y = 2;
			SetConsoleCursorPosition(h, health_info);
			cout << "           ";
			SetConsoleCursorPosition(h, health_info);
			SetConsoleTextAttribute(h, Color::PURPLE);
			cout << "HEALTH: ";
			SetConsoleTextAttribute(h, Color::PINK);
			cout << health;

		}

		if (map[hero.Y][hero.X] == GameObject::ENEMY) {
			health -= 25;
			map[hero.Y][hero.X] = GameObject::HALL;
			COORD health_info;
			health_info.X = WIDTH + 1;
			health_info.Y = 2;
			SetConsoleCursorPosition(h, health_info);
			cout << "           ";
			SetConsoleCursorPosition(h, health_info);
			SetConsoleTextAttribute(h, Color::PURPLE);
			cout << "HEALTH: ";
			SetConsoleTextAttribute(h, Color::PINK);
			cout << health;
		}

		COORD msg_pos;
		msg_pos.X = WIDTH + 1;
		msg_pos.Y = HEIGHT / 2;

		COORD exit_pos;
		exit_pos.X = 0;
		exit_pos.Y = HEIGHT;

		if (hero.Y == HEIGHT - 3 && hero.X == WIDTH - 1)
		{
			SetConsoleCursorPosition(h, msg_pos);
			SetConsoleTextAttribute(h, Color::RED);
			cout << "YOU WON! - you found the exit";
			SetConsoleCursorPosition(h, exit_pos);
			SetConsoleTextAttribute(h, Color::WHITE);
			exit(1);
		}

		if (coins_collected >= 406)
		{
			SetConsoleCursorPosition(h, msg_pos);
			SetConsoleTextAttribute(h, Color::RED);
			cout << "YOU WON! - you collected all coins";
			SetConsoleCursorPosition(h, exit_pos);
			SetConsoleTextAttribute(h, Color::WHITE);
			exit(1);
		}

		if (health <= 0)
		{
			SetConsoleCursorPosition(h, msg_pos);
			SetConsoleTextAttribute(h, Color::RED);
			cout << "YOU LOST! - you have no health";
			SetConsoleCursorPosition(h, exit_pos);
			SetConsoleTextAttribute(h, Color::WHITE);
			exit(1);
		}
	}


	Sleep(INFINITE);
}
