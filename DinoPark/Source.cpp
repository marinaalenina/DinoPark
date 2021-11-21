#include <iomanip>
#include <string>
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unordered_map>

using namespace std;

class Point {
	int _x, _y;

public:
	Point() {
		_x = 0;
		_y = 0;
	}

	Point(int x, int y) {
		_x = x;
		_y = y;
	}

	int getX() { return _x; }
	int getY() { return _y; }

};

// Отсюда вы можете брать цвета и здесь же создавать свои
class ConsoleColor {
public:
	static COLORREF red() { return RGB(255, 0, 0); }
	static COLORREF blue() { return RGB(0, 0, 255); }
	static COLORREF gray() { return RGB(200, 200, 200); }
	static COLORREF white() { return RGB(255, 255, 255); }
	static COLORREF green() { return RGB(0, 255, 0); }
	static COLORREF black() { return RGB(0, 0, 0); }
	static COLORREF brown() { return RGB(80, 20, 25); }
	static COLORREF yellow() { return RGB(255, 255, 0); }
};

// Это класс, который нужен для рисования. В нем все готово. Ничего менять не нужно. Нужно только пользоваться
class ConsoleDrawer {
	HANDLE _conHandle;
	HWND _hwnd;
	HDC _hdc;
	PAINTSTRUCT _ps;
	unordered_map<COLORREF, HGDIOBJ> _bm;
	unordered_map<COLORREF, HGDIOBJ> _pm;
	HGDIOBJ _curentBrush;
	HGDIOBJ _curentPen;

	int _width;
	int _height;
	int pos_x = 0;
	int pos_y = 0;
	int CellSize;
	string name = "sss";
	bool _isWork = true;
	bool game_over = false;

	void selectBrush(COLORREF color, bool filled = false) {
		if (!filled) {
			SelectObject(_hdc, CreateSolidBrush(NULL_BRUSH));
			return;
		}
		if (_bm.find(color) == _bm.end())
			_bm.insert({ color, CreateSolidBrush(color) });

		if (_curentBrush != _bm[color]) {
			_curentBrush = _bm[color];
			SelectObject(_hdc, _curentBrush);
		}
	}

	void selectPen(COLORREF color) {
		if (_pm.find(color) == _pm.end())
			_pm.insert({ color, CreatePen(PS_SOLID, 1, color) });
		if (_curentPen != _pm[color]) {
			_curentPen = _pm[color];
			SelectObject(_hdc, _curentPen);
		}
	}

public:
	ConsoleDrawer(int _CellSize) {
		setlocale(LC_ALL, "");
		_conHandle = GetStdHandle(STD_INPUT_HANDLE);
		int t = sizeof(HGDIOBJ);
		_hwnd = GetConsoleWindow();
		RECT rc;
		GetClientRect(_hwnd, &rc);
		_width = rc.right;
		_height = rc.bottom;
		CellSize = _CellSize;
		_hdc = GetDC(_hwnd);
	}

	void clearScreen() {
		system("cls");
	}

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	HANDLE get_conHandle() {
		return _conHandle;
	}

	bool gameOver() { return game_over; }

	bool isWork() {

		return _isWork;
	}

	void prestop(int score) {


		int mid_h = _height / 2;
		int mid_w = _width / 2;

		////Таблица для ника
		//drawRect(Point(200 - 5, 80 - 5), Point(440 + 5, 200 + 5), ConsoleColor::red(), true);
		//drawRect(Point(200, 80), Point(440, 200), ConsoleColor::green(), true);

		drawRect(Point(mid_w - 3 * CellSize, 0), Point(mid_w + 3 * CellSize, 3 * CellSize), ConsoleColor::red(), true);
		drawRect(Point(mid_w - 3 * CellSize + (int)(0.125 * CellSize), 0 + (int)(0.125 * CellSize)), Point(mid_w + 3 * CellSize - (int)(0.125 * CellSize), 3 * CellSize - (int)(0.125 * CellSize)), ConsoleColor::green(), true);

		//Поля имени
		drawRect(Point(mid_w - (int)(2.5 * CellSize), CellSize), Point(mid_w - (int)(1.5 * CellSize), 2 * CellSize), ConsoleColor::red(), true);
		drawDigit('s', Point(mid_w - (int)(2.5 * CellSize), CellSize), ConsoleColor::white(), ConsoleColor::red());

		drawRect(Point(mid_w - (int)(0.5 * CellSize), CellSize), Point(mid_w + (int)(0.5 * CellSize), 2 * CellSize), ConsoleColor::red(), true);
		drawDigit('s', Point(mid_w - (int)(0.5 * CellSize), CellSize), ConsoleColor::white(), ConsoleColor::red());

		drawRect(Point(mid_w + (int)(1.5 * CellSize), CellSize), Point(mid_w + (int)(2.5 * CellSize), 2 * CellSize), ConsoleColor::red(), true);
		drawDigit('s', Point(mid_w + (int)(1.5 * CellSize), CellSize), ConsoleColor::white(), ConsoleColor::red());

		drawRect(Point(220, 120), Point(260, 160), ConsoleColor::red(), true);
		drawRect(Point(300, 120), Point(340, 160), ConsoleColor::red(), true);
		drawRect(Point(380, 120), Point(420, 160), ConsoleColor::red(), true);


//Стрелочки
		drawTriangle(Point(mid_w - (int)(2.5 * CellSize) + (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w - (int)(1.5 * CellSize) - (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w - 2 * CellSize, int(0.25 * CellSize)), ConsoleColor::red(), true);
		drawTriangle(Point(mid_w - (int)(2.5 * CellSize) + (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w - (int)(1.5 * CellSize) - (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w - 2 * CellSize, 3 * CellSize - int(0.25 * CellSize)), ConsoleColor::red(), true);

		drawTriangle(Point(mid_w - (int)(0.5 * CellSize) + (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w + (int)(0.5 * CellSize) - (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w, int(0.25 * CellSize)), ConsoleColor::red(), true);
		drawTriangle(Point(mid_w - (int)(0.5 * CellSize) + (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w + (int)(0.5 * CellSize) - (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w, 3 * CellSize - int(0.25 * CellSize)), ConsoleColor::red(), true);

		drawTriangle(Point(mid_w + (int)(1.5 * CellSize) + (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w + (int)(2.5 * CellSize) - (int)(0.125 * CellSize), int(0.75 * CellSize)), Point(mid_w + 2 * CellSize, int(0.25 * CellSize)), ConsoleColor::red(), true);
		drawTriangle(Point(mid_w + (int)(1.5 * CellSize) + (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w + (int)(2.5 * CellSize) - (int)(0.125 * CellSize), 3 * CellSize - int(0.75 * CellSize)), Point(mid_w + 2 * CellSize, 3 * CellSize - int(0.25 * CellSize)), ConsoleColor::red(), true);

		//Таблица счета
		drawRect(Point(_width - 5 * CellSize, _height - 2 * CellSize), Point(_width, _height), ConsoleColor::red(), true);
		drawRect(Point(_width - 5 * CellSize + (int)(0.1 * CellSize), _height - 2 * CellSize + (int)(0.1 * CellSize)), Point(_width - (int)(0.1 * CellSize), _height - (int)(0.1 * CellSize)), ConsoleColor::green(), true);

		//best score
		drawRect(Point(0, _height - 3 * CellSize), Point(5 * CellSize, _height), ConsoleColor::red(), true);
		drawRect(Point(0 + (int)(0.1 * CellSize), _height - 3 * CellSize + (int)(0.1 * CellSize)), Point(5 * CellSize - (int)(0.1 * CellSize), _height - (int)(0.1 * CellSize)), ConsoleColor::green(), true);
		string best = "best";
		int digit_x = 3 * CellSize;
		for (int i = 3; i >= 0; i--) {
			drawDigit(best[i], Point(digit_x, _height - 3 * CellSize), ConsoleColor::red(), ConsoleColor::green());
			digit_x -= CellSize;
		}


		//Вывод счета
		digit_x = _width - CellSize;
		if (score == 0)
			drawDigit(score, Point(digit_x, _height - CellSize), ConsoleColor::red(), ConsoleColor::green());
		while (score > 0) {
			drawDigit(score % 10, Point(digit_x, _height - CellSize), ConsoleColor::red(), ConsoleColor::green());
			score /= 10;
			digit_x -= CellSize;
		}

		digit_x = _width - CellSize;
		char str[] = { 's', 'c', 'o', 'r', 'e' };
		for (int i = 4; i >= 0; i--) {
			drawDigit(str[i], Point(digit_x, _height - 2 * CellSize), ConsoleColor::red(), ConsoleColor::green());
			digit_x -= CellSize;
		}


		//KEY_EVENT_RECORD key;
		//INPUT_RECORD irec[100];
		//DWORD cc;
		//string symbol = "score";
		//vector<Point> position = {Point(mid_w - (int)(2.5*CellSize), CellSize), Point(mid_w - (int)(0.5*CellSize), CellSize), Point(mid_w + (int)(1.5*CellSize), CellSize)};
		//int pos_x = 0;
		//int pos_y = 0;
		//ReadConsoleInput(get_conHandle(), irec, 100, &cc);
		//for (DWORD j = 0; j < cc; ++j) {
		//	if (irec[j].EventType == KEY_EVENT && irec[j].Event.KeyEvent.bKeyDown) {
		//		// Проверяем, какая именно кнопка была нажата
		//		switch (irec[j].Event.KeyEvent.wVirtualKeyCode) {
		//		case VK_LEFT:
		//			if (pos_x != 0)
		//				pos_x--;
		//			break;
		//		case VK_RIGHT:
		//			if (pos_x != 2)
		//				pos_x++;
		//			break;
		//		case VK_UP:
		//			if (++pos_y > 5)
		//				pos_y = 0;
		//			drawDigit(symbol[pos_y], position[pos_x], ConsoleColor::red(), ConsoleColor::green());
		//			break;
		//		case VK_DOWN:
		//			if (--pos_y < 0)
		//				pos_y = 4;
		//			drawDigit(symbol[pos_y], position[pos_x], ConsoleColor::red(), ConsoleColor::green());
		//			break;
		//		}
		//	}

		//}

		_isWork = false;
	}

	void finish(int score) {
		int high_score;
		string best_player;
		ifstream ifs("input.txt");
		cin.rdbuf(ifs.rdbuf());
		cin >> high_score >> best_player;
		if (best_player == "") {
			best_player = "bos";
			high_score = 0;
		}
		ifs.close();

		int digit_x = 4 * CellSize;
		for (int i = 3; i >= 0; i--) {
			drawDigit(best_player[i], Point(digit_x, _height - 2 * CellSize), ConsoleColor::red(), ConsoleColor::green());
			digit_x -= CellSize;
		}
		digit_x = 4 * CellSize;
		if (high_score == 0) {
			drawDigit(high_score, Point(digit_x, _height - CellSize), ConsoleColor::red(), ConsoleColor::green());
		}
		while (high_score > 0) {
			drawDigit(high_score % 10, Point(digit_x, _height - CellSize), ConsoleColor::red(), ConsoleColor::green());
			high_score /= 10;
			digit_x -= CellSize;
		}

		int mid_h = _height / 2;
		int mid_w = _width / 2;
		KEY_EVENT_RECORD key;
		INPUT_RECORD irec[100];
		DWORD cc;
		string symbol = "scoretbi";
		vector<Point> position = { Point(mid_w - (int)(2.5 * CellSize), CellSize), Point(mid_w - (int)(0.5 * CellSize), CellSize), Point(mid_w + (int)(1.5 * CellSize), CellSize) };
		ReadConsoleInput(get_conHandle(), irec, 100, &cc);
		for (DWORD j = 0; j < cc; ++j) {
			if (irec[j].EventType == KEY_EVENT && irec[j].Event.KeyEvent.bKeyDown) {
				// Проверяем, какая именно кнопка была нажата
				switch (irec[j].Event.KeyEvent.wVirtualKeyCode) {
				case VK_LEFT:
					name[pos_x] = symbol[pos_y];
					--pos_x;
					pos_y = 0;
					if (pos_x < 0)
						pos_x = 0;
					break;
				case VK_RIGHT:
					name[pos_x] = symbol[pos_y];
					pos_x++;
					pos_y = 0;
					if (pos_x > 2)
						pos_x = 2;
					break;
				case VK_UP:
					if (++pos_y > 7)
						pos_y = 0;
					drawDigit(symbol[pos_y], position[pos_x], ConsoleColor::white(), ConsoleColor::red());
					break;
				case VK_DOWN:
					if (--pos_y < 0)
						pos_y = 7;
					drawDigit(symbol[pos_y], position[pos_x], ConsoleColor::white(), ConsoleColor::red());
					break;
				case VK_RETURN:
					name[pos_x] = symbol[pos_y];

					if (score > high_score) {
						ofstream ofs("input.txt");
						ofs << score << " " << name;
						ofs.close();
					}
					game_over = true;
				}
			}
		}
	}

	void drawBackground(COLORREF color = ConsoleColor::gray()) {
		RECT rc;
		GetClientRect(_hwnd, &rc);
		drawRect(Point(), Point(_width, _height), color, true);
	}

	void drawPoint(Point p, COLORREF color) {
		SetPixel(_hdc, p.getX(), p.getY(), color);
	}

	void drawLine(Point p1, Point p2, COLORREF color) {
		selectPen(color);
		//SelectObject(_hdc, CreatePen(PS_SOLID, 1, color));
		//SetDCPenColor(_hdc, color);
		MoveToEx(_hdc, p1.getX(), p1.getY(), NULL);
		LineTo(_hdc, p2.getX(), p2.getY());
	}

	// Рисует круг или окружность по центру и радиусу
	// filled - нужно ли закрашивать внутренность
	void drawCircle(Point c, int radius, COLORREF color, bool filled = false) {
		selectPen(color);
		selectBrush(color, filled);
		//SetDCBrushColor(_hdc, color);
		Ellipse(_hdc, c.getX() - radius, c.getY() - radius, c.getX() + radius, c.getY() + radius);

	}

	// Рисует прямоугольник по двум точкам
	// p1 - нижний левый угол
	// p2 - верхний правый угол
	void drawRect(Point p1, Point p2, COLORREF color, bool filled = false) {
		selectPen(color);
		selectBrush(color, filled);
		Rectangle(_hdc, p1.getX(), p1.getY(), p2.getX(), p2.getY());
	}

	//рисует цифру
	void drawDigit(int digit, Point p, COLORREF color_general, COLORREF color) {
		int a = CellSize;
		switch (digit)
		{
		case 0:
			drawRect(Point(p.getX() + int(a / 4.), p.getY() + int(a / 8.)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 1:
			drawRect(Point(p.getX() + int(29. / 40. * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.875 * a), p.getY() + int(0.875 * a)), color_general, true);
			break;
		case 2:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 3:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 4:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(0.875 * a)), color, true);
			break;
		case 5:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(17. / 40. * a)), color, true);
			break;
		case 6:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 7:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(0.875 * a)), color, true);
			break;
		case 8:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 9:
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		default:
			break;
		}
	}


	void drawDigit(char sym, Point p, COLORREF color_general, COLORREF color) {
		int a = CellSize;

		switch (sym)
		{
		case 's':
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(17. / 40. * a)), color, true);
			break;
		case 'c':
			drawRect(Point(p.getX() + int(a / 4.), p.getY() + int(a / 8.)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 'o':
			drawRect(Point(p.getX() + int(a / 4.), p.getY() + int(a / 8.)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 'r':
			drawRect(Point(p.getX() + int(a / 4.), p.getY() + int(a / 8.)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawTriangle(Point(p.getX() + int(0.4 * a), p.getY() + int(25. / 40. * a)), Point(p.getX() + int(0.4 * a), p.getY() + int(0.875 * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(0.875 * a)), color, true);
			drawTriangle(Point(p.getX() + int(0.54 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.8 * a), p.getY() + int(0.875 * a)), color, true);
			break;
		case 'e':
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.75 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 't':
			drawRect(Point(p.getX() + int(0.1 * a), p.getY() + int(0.1 * a)), Point(p.getX() + int(0.9 * a), p.getY() + int(0.9 * a)), color, true);
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(11. / 40. * a)), color_general, true);
			drawRect(Point(p.getX() + int(17. / 40. * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(23. / 40. * a), p.getY() + int(0.875 * a)), color_general, true);
			break;
		case 'b':
			drawRect(Point(p.getX() + int(0.25 * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(0.75 * a), p.getY() + int(0.875 * a)), color_general, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(11. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(17. / 40. * a)), color, true);
			drawRect(Point(p.getX() + int(0.4 * a), p.getY() + int(23. / 40. * a)), Point(p.getX() + int(0.6 * a), p.getY() + int(29. / 40. * a)), color, true);
			break;
		case 'i':
			drawRect(Point(p.getX(), p.getY()), Point(p.getX() + int(0.9 * a), p.getY() + int(0.9 * a)), color, true);
			drawRect(Point(p.getX() + int(17. / 40. * a), p.getY() + int(a * 0.125)), Point(p.getX() + int(23. / 40. * a), p.getY() + int(0.875 * a)), color_general, true);
			break;
		default:
			break;
		}
	}

	// Рисует треугольник по трем точкам
	void drawTriangle(Point p1, Point p2, Point p3, COLORREF color, bool filled = false) {
		selectPen(color);
		selectBrush(color, filled);
		POINT apt[3];
		apt[0].x = p1.getX(); apt[0].y = p1.getY();
		apt[1].x = p2.getX(); apt[1].y = p2.getY();
		apt[2].x = p3.getX(); apt[2].y = p3.getY();
		Polygon(_hdc, apt, 3);
	}
};

// Это самый базовый класс, от которого наследуются все объекты, которые появляются на поле
class ParkObject {
protected:
	int _cellX, _cellY, _cellSize;

public:
	ParkObject(int cellX, int cellY, int cellSize) {
		_cellX = cellX;
		_cellY = cellY;
		_cellSize = cellSize;
	}

	bool isInside(int cellX, int cellY) {
		return (_cellX == cellX) && (_cellY == cellY);
	}

	int getCellX() {
		return _cellX;
	}

	int getCellY() {
		return _cellY;
	}

	int getX() {
		return _cellX * _cellSize;
	}

	int getY() {
		return _cellY * _cellSize;
	}

};


class water : public ParkObject {
public:
	water(int cellX, int cellY, int cellSize) : ParkObject(cellX, cellY, cellSize) { }

	void draw(ConsoleDrawer& cd) {
		//Переходим от ячеек к координатам пикселей
		int x = getX();
		int y = getY();

		double d = _cellSize / 100.0;

		cd.drawRect(Point(x + _cellSize, y + _cellSize), Point(x, y), ConsoleColor::blue(), true);
	}
};



class SnowMan : public ParkObject {

public:
	SnowMan(int cellX, int cellY, int cellSize) : ParkObject(cellX, cellY, cellSize) { }

	void draw(ConsoleDrawer& cd) {
		//Переходим от ячеек к координатам пикселей
		int x = getX();
		int y = getY();

		double d = _cellSize / 100.0;
		cd.drawCircle(Point(x + _cellSize / 2, y + _cellSize - 20 * d), 20 * d, ConsoleColor::white(), true);
		cd.drawCircle(Point(x + _cellSize / 2, y + _cellSize - 55 * d), 15 * d, ConsoleColor::white(), true);
		cd.drawCircle(Point(x + _cellSize / 2, y + _cellSize - 80 * d), 10 * d, ConsoleColor::white(), true);
		cd.drawRect(Point(x + _cellSize / 2 - 5 * d, y + 10 * d), Point(x + _cellSize / 2 + 5 * d, y + d), ConsoleColor::brown(), true);
	}
};

class Present : public ParkObject {

public:
	Present(int cellX, int cellY, int cellSize) : ParkObject(cellX, cellY, cellSize) {
	}

	void draw(ConsoleDrawer& cd) {
		//Переходим от ячеек к координатам пикселей
		int x = getX();
		int y = getY();

		double d = _cellSize / 100.0;
		cd.drawRect(Point(x + 20 * d, y + _cellSize), Point(x + 80 * d, y + 40 * d), ConsoleColor::red(), true);
		cd.drawRect(Point(x + 40 * d, y + _cellSize), Point(x + 60 * d, y + 40 * d), ConsoleColor::yellow(), true);
		cd.drawCircle(Point(x + 35 * d, y + 20 * d), 10 * d, ConsoleColor::yellow(), true);
		cd.drawCircle(Point(x + 65 * d, y + 20 * d), 10 * d, ConsoleColor::yellow(), true);
		cd.drawTriangle(Point(x + 50 * d, y + 40 * d), Point(x + 35 * d, y + 30 * d), Point(x + 45 * d, y + 20 * d), ConsoleColor::yellow(), true);
		cd.drawTriangle(Point(x + 50 * d, y + 40 * d), Point(x + 65 * d, y + 30 * d), Point(x + 55 * d, y + 20 * d), ConsoleColor::yellow(), true);
	}
};

class Tree : public ParkObject {
	//vector<COLORREF> colors = { RGB(255, 0, 0) ,RGB(0, 0, 255) ,RGB(200, 200, 200) ,RGB(255, 255, 255) ,RGB(0, 255, 0) ,RGB(0, 0, 0) ,RGB(80, 20, 25),RGB(255, 255, 0) };
public:
	Tree(int cellX, int cellY, int cellSize) : ParkObject(cellX, cellY, cellSize) {}
	int color = (int)rand() % 7;
	void draw(ConsoleDrawer& cd) {
		//Переходим от ячеек к координатам пикселей
		int x = getX();
		int y = getY();

		int d = _cellSize / 10;
		cd.drawRect(Point(x + 4 * d, y + _cellSize), Point(x + 6 * d, y + _cellSize - 2 * d), ConsoleColor::brown(), true);


		switch (color) {
		case 0:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::blue(), true); break;
		case 1:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::green(), true); break;
		case 2:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::red(), true); break;
		case 3:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::yellow(), true); break;
		case 4:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::white(), true); break;
		case 5:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::brown(), true); break;
		case 6:
			cd.drawTriangle(Point(x + 2 * d, y + _cellSize - 2 * d), Point(x + _cellSize / 2, y + d), Point(x + _cellSize - 2 * d, y + _cellSize - 2 * d), ConsoleColor::black(), true); break;
		}
	}
};

class HappyDino : public ParkObject {

public:
	HappyDino(int cellX, int cellY, int cellSize) : ParkObject(cellX, cellY, cellSize) {}

	int revers = 1;
	int speed = 1;
	int count_in_revers = 0;
	int count_speed_step = 0;

	void draw(ConsoleDrawer& cd) {

		//Переходим от ячеек к координатам пикселей
		int x = getX();
		int y = getY();
		COLORREF color = ConsoleColor::green();

		double d = _cellSize / 125.0;
		cd.drawRect(Point(x + d, y + 63 * d), Point(x + 24 * d, y + 53 * d), color, true);
		cd.drawRect(Point(x + 75 * d, y + 63 * d), Point(x + 98 * d, y + 53 * d), color, true);
		cd.drawRect(Point(x + 27 * d, y + 93 * d), Point(x + 72 * d, y + 35 * d), color, true);//body
		cd.drawRect(Point(x + 27 * d, y + 124 * d), Point(x + 42 * d, y + 94 * d), color, true);
		cd.drawRect(Point(x + 57 * d, y + 124 * d), Point(x + 72 * d, y + 94 * d), color, true);
		cd.drawRect(Point(x + 27 * d, y + 32 * d), Point(x + 88 * d, y + 2 * d), color, true);//head
		cd.drawCircle(Point(x + 42 * d, y + 12 * d), 8 * d, ConsoleColor::gray(), true);
		cd.drawTriangle(Point(x + 27 * d, y), Point(x + 49 * d, y - 45 * d), Point(x + 72 * d, y), ConsoleColor::red(), true);
		cd.drawCircle(Point(x + 49 * d, y - 50 * d), 10 * d, ConsoleColor::white(), true);

	}

	void step(int direction) {
		if (direction == 0)
			_cellX -= 1 * revers * speed;
		if (direction == 1)
			_cellY -= 1 * revers * speed;
		if (direction == 2)
			_cellX += 1 * revers * speed;
		if (direction == 3)
			_cellY += 1 * revers * speed;
	}
};

class DinoPark {
	int _cellsXCount;
	int count_steps_without_tree = 0;
	int score = 50;
	int _cellsYCount;
	vector<water> _water;
	int _cellSize;
	vector<Present> _podarochek;
	vector<SnowMan> _snowmen;
	vector<Tree> _trees;
	vector<HappyDino> _hDino;

public:
	DinoPark(int width, int height, int cellSize) {
		_cellsXCount = width;
		_cellsYCount = height;
		_cellSize = cellSize;
	}

	int getScore() { return score; }

	// Этот метод проверяет, что находится в клетке:
	// -1 - снеговик, вода, конец поля
	//  0 - пусто
	//  1 - дерево, подарочек
	//  2 - динозавр

	int find(int x, int y) {
		for (int i = 0; i < _snowmen.size(); i++) {
			if (_snowmen[i].isInside(x, y))
				return -1;
		}
		for (int i = 0; i < _trees.size(); i++) {
			if (_trees[i].isInside(x, y))
				return 1;
		}
		for (int i = 0; i < _hDino.size(); i++) {
			if (_hDino[i].isInside(x, y))
				return 2;
		}
		for (int i = 0; i < _podarochek.size(); i++) {
			if (_podarochek[i].isInside(x, y))
				return 2;
		}
		for (int i = 0; i < _water.size(); i++) {
			if (_water[i].isInside(x, y))
				return -1;
		}
		if (x > _cellsXCount - 1 || y > _cellsYCount - 1 || x < 0 || y < 0)
			return -1;
		return 0;
	}

	// Метод для отрисовки всего поля
	void draw(ConsoleDrawer& cd) {

		// Рисуем сетку
		for (int i = 0; i <= _cellsXCount; i++) {
			int x = i * _cellSize;
			int y = _cellsYCount * _cellSize;
			cd.drawLine(Point(x, 0), Point(x, y), ConsoleColor::black());
		}
		for (int i = 0; i <= _cellsYCount; i++) {
			int x = _cellsXCount * _cellSize;
			int y = i * _cellSize;
			cd.drawLine(Point(0, y), Point(x, y), ConsoleColor::black());
		}

		// Рисуем снеговиков
		for (int i = 0; i < _snowmen.size(); i++) {
			_snowmen[i].draw(cd);
		}

		// Рисуем деревья
		for (int i = 0; i < _trees.size(); i++) {
			_trees[i].draw(cd);
		}

		//Рисуем podarochek
		for (int i = 0; i < _podarochek.size(); i++) {
			_podarochek[i].draw(cd);
		}

		// Рисуем динозавра
		for (int i = 0; i < _hDino.size(); i++) {
			_hDino[i].draw(cd);
		}
		//Рисуем воду
		for (int i = 0; i < _water.size(); i++) {
			_water[i].draw(cd);
		}
	}

	//Пытаемся подложить подарочек под елочку)))
	void put_podarochek() {
		if (_podarochek.size() == 0) {
			int random_tree = (int)rand() % _trees.size();
			if (find(_trees[random_tree].getCellX(), _trees[random_tree].getCellY() + 1) == 0)
				addPodarochek(_trees[random_tree].getCellX(), _trees[random_tree].getCellY() + 1);
		}
	}

	// Метод обработки вашего хода
	void step(ConsoleDrawer& cd) {

		// Пока ходит только счастливый динозавр

		// Ловим нажатие на клавиатуру
		KEY_EVENT_RECORD key;
		INPUT_RECORD irec[100];
		DWORD cc;

		ReadConsoleInput(cd.get_conHandle(), irec, 100, &cc);
		for (DWORD j = 0; j < cc; ++j) {
			if (irec[j].EventType == KEY_EVENT && irec[j].Event.KeyEvent.bKeyDown) {

				for (int i = 0; i < _hDino.size(); i++) {
					// Смотрим по сторонам
					// -1 - снеговик, вода
					//  0 - пусто
					//  1 - дерево, podarochek
					//  2 - динозавр

					vector<int> res = {
						lookLeft(_hDino[i].getCellX(), _hDino[i].getCellY()),
						lookUp(_hDino[i].getCellX(), _hDino[i].getCellY()),
						lookRight(_hDino[i].getCellX(), _hDino[i].getCellY()),
						lookDown(_hDino[i].getCellX(), _hDino[i].getCellY())
					};
					// Проверяем, какая именно кнопка была нажата
					switch (irec[j].Event.KeyEvent.wVirtualKeyCode) {
					case VK_LEFT:
						// если путь свободен, идем
						if (res[0] != -1) {
							_hDino[i].step(0);
							score -= 5;

							if (score < 0) {
								cd.prestop(0);
							}

							//Пытаемся подложить подарочек под елочку)))
							put_podarochek();

							if (_hDino[0].count_in_revers++ == 5) {
								_hDino[0].count_in_revers = 0;
								_hDino[0].revers = 1;
							}
							if (_hDino[0].count_speed_step++ == 5) {
								_hDino[0].count_speed_step = 0;
								_hDino[0].speed = 1;
							}

							count_steps_without_tree = (res[0] != 1) ? count_steps_without_tree + 1 : 0;
							if (count_steps_without_tree == 10) {
								count_steps_without_tree = 0;
								score -= 15;
							}
							// если мы на что-то наступили (пока что только дерево), обновляем картину
							if (res[0] != 0) {
								refresh(cd);
							}
						}
						break;
					case VK_UP:
						if (res[1] != -1) {
							_hDino[i].step(1);
							score -= 5;

							if (score < 0) {
								cd.prestop(0);
							}

							//Пытаемся подложить подарочек под елочку)))
							put_podarochek();

							if (_hDino[0].count_in_revers++ == 5) {
								_hDino[0].count_in_revers = 0;
								_hDino[0].revers = 1;
							}
							if (_hDino[0].count_speed_step++ == 5) {
								_hDino[0].count_speed_step = 0;
								_hDino[0].speed = 1;
							}

							count_steps_without_tree = (res[1] != 1) ? count_steps_without_tree + 1 : 0;
							if (count_steps_without_tree == 10) {
								count_steps_without_tree = 0;
								score -= 15;
							}
							if (res[1] != 0) {
								refresh(cd);
							}
						}
						break;
					case VK_RIGHT:
						if (res[2] != -1) {
							_hDino[i].step(2);
							score -= 5;

							if (score < 0) {
								cd.prestop(0);
							}

							//Пытаемся подложить подарочек под елочку))
							put_podarochek();

							if (_hDino[0].count_in_revers++ == 5) {
								_hDino[0].count_in_revers = 0;
								_hDino[0].revers = 1;
							}
							if (_hDino[0].count_speed_step++ == 5) {
								_hDino[0].count_speed_step = 0;
								_hDino[0].speed = 1;
							}

							count_steps_without_tree = (res[2] != 1) ? count_steps_without_tree + 1 : 0;
							if (count_steps_without_tree == 10) {
								count_steps_without_tree = 0;
								score -= 15;
							}
							if (res[2] != 0) {
								refresh(cd);
							}
						}
						break;
					case VK_DOWN:
						if (res[3] != -1) {
							_hDino[i].step(3);
							score -= 5;

							if (score < 0) {
								cd.prestop(0);
							}

							//Пытаемся подложить подарочек под елочку)))
							put_podarochek();

							if (_hDino[0].count_in_revers++ == 5) {
								_hDino[0].count_in_revers = 0;
								_hDino[0].revers = 1;
							}
							if (_hDino[0].count_speed_step++ == 5) {
								_hDino[0].count_speed_step = 0;
								_hDino[0].speed = 1;
							}

							count_steps_without_tree = (res[3] != 1) ? count_steps_without_tree + 1 : 0;
							if (count_steps_without_tree == 10) {
								count_steps_without_tree = 0;
								score -= 15;
							}
							if (res[3] != 0) {
								refresh(cd);
							}
						}
						break;
					case VK_ESCAPE:
						cd.prestop(score);
						break;
					}

				}

			}
		}
	}

	void addSnowMan(int x, int y) {
		_snowmen.push_back(SnowMan(x, y, _cellSize));
	}

	void addTree(int x, int y) {
		_trees.push_back(Tree(x, y, _cellSize));
	}

	void addTree(Tree tr) {
		_trees.push_back(tr);
	}

	void addPodarochek(int x, int y) {
		_podarochek.push_back(Present(x, y, _cellSize));
	}

	void addHappyDino(int x, int y) {
		_hDino.push_back(HappyDino(x, y, _cellSize));
	}
	void addWater(int x, int y) {
		_water.push_back(water(x, y, _cellSize));
	}

	// Взгляд на клетку вверх
	int lookUp(int cellX, int cellY) {
		if (cellY == 0)
			return -1;
		return find(cellX, cellY - 1 * _hDino[0].revers * _hDino[0].speed);
	}

	// Взгляд на клетку вниз
	int lookDown(int cellX, int cellY) {
		if (cellY == _cellsYCount - 1)
			return -1;
		return find(cellX, cellY + 1 * _hDino[0].revers * _hDino[0].speed);
	}

	// Взгляд на клетку вправо
	int lookRight(int cellX, int cellY) {
		if (cellX == _cellsXCount - 1)
			return -1;
		return find(cellX + 1 * _hDino[0].revers * _hDino[0].speed, cellY);
	}

	// Взгляд на клетку влево
	int lookLeft(int cellX, int cellY) {
		if (cellX == 0)
			return -1;
		return find(cellX - 1 * _hDino[0].revers * _hDino[0].speed, cellY);
	}

	// Обновляем картину
	void refresh(ConsoleDrawer& cd) {
		for (int i = 0; i < _hDino.size(); i++) {
			// Смотрим, где стоит динозавр
			int x = _hDino[i].getCellX();
			int y = _hDino[i].getCellY();
			// Находим то дерево, на котором он стоит
			for (int j = 0; j < _trees.size(); j++) {
				if (_trees[j].isInside(x, y)) {
					//Градация ёлочек по цвету
					score += _trees[j].color * 10 + 10;
					// Переносим дерево в другое место
					// Цикл нужен, чтобы не попасть в место, которое уже занято
					for (int k = 0; k < 100; k++) {
						int _x = rand() % _cellsXCount;
						int _y = rand() % _cellsYCount;
						if (find(_x, _y) == 0) {
							_trees[j] = Tree(_x, _y, _cellSize);
							break;
						}
					}
				}
			}
			//Бонусы,которые могут выпасть в подарочке
			if (_podarochek[0].isInside(x, y)) {
				switch ((int)rand() % 3)
				{
				case 0:
					_hDino[0].revers = -1;
					_hDino[0].count_in_revers = 0;
					break;
				case 1:
					_hDino[0].speed = 2;
					_hDino[0].count_speed_step = 0;
					break;
				case 2:
					score += (int)rand() % 51;
					break;
				}
				int random_tree = (int)rand() % _trees.size();
				_podarochek.pop_back();
				//Добавляем подарок под елку, если есть пустая клетка
				if (find(_trees[random_tree].getCellX(), _trees[random_tree].getCellY() + 1) == 0 && _trees[random_tree].getCellY() + 1 < _cellsYCount)
					addPodarochek(_trees[random_tree].getCellX(), _trees[random_tree].getCellY() + 1);
			}
		}
	}

};

int main() {

	const int CellSize = 30;

	ConsoleDrawer cd(CellSize);
	srand(time(0));

	int width = cd.getWidth() / CellSize;
	int height = cd.getHeight() / CellSize;
	DinoPark dinoPark(width, height, CellSize);

	int random_x = (int)rand() % width;
	int random_y = (int)rand() % height;
	if (dinoPark.find(random_x, random_y) == 0) {
		dinoPark.addHappyDino(random_x, random_y);
	}


	for (int i = 0; i < 15; i++) {
		int random_x = (int)rand() % width;
		int random_y = (int)rand() % height;
		if (dinoPark.find(random_x, random_y) == 0) {
			dinoPark.addWater(random_x, random_y);
		}
	}

	for (int i = 0; i < 15; i++) {
		int random_x = (int)rand() % width;
		int random_y = (int)rand() % height;
		if (dinoPark.find(random_x, random_y) == 0) {
			dinoPark.addSnowMan(random_x, random_y);
		}
	}

	for (int i = 0; i < 26; i++) {
		int random_x = (int)rand() % width;
		int random_y = (int)rand() % height;
		if (dinoPark.find(random_x, random_y) == 0) {
			dinoPark.addTree(random_x, random_y);
		}
	}

	int _x = (int)(rand() % width);
	int _y = (int)(rand() % height);
	if (dinoPark.find(_x, _y) == 0) {
		dinoPark.addPodarochek(_x, _y);
	}

	cd.clearScreen();

	while (cd.isWork()) {
		cd.drawBackground();
		dinoPark.draw(cd);
		dinoPark.step(cd);
	}
	while (!cd.gameOver()) {
		cd.finish(dinoPark.getScore());
	}
	return 0;
}