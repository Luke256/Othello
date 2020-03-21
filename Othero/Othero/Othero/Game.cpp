#include <Siv3D.hpp>
#include "Game.h"
#pragma warning(disable:4715)

Game::Game(const InitData& init) :IScene(init) {
	floor = Grid<Rect>(8, 8);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			floor[i][j] = Rect(20 + j * 70, 150 + i * 70, 70);
		}
	}
	col = Grid<color>(8, 8, null);
	col[3][3] = col[4][4] = black;
	col[3][4] = col[4][3] = white;
	Frame = 0;
	num_black = num_white = 2;
	num_null = 60;
	now_turn = black;
	info = Font(40);
	result = Font(100);
	pass = RoundRect(200, 70, 200, 75, 10);
	finish = false;
}

void Game::update() {
	Cursor::RequestStyle(CursorStyle::Arrow);
	if (can(now.x, now.y)) {
		possible = true;
	}
	else {
		possible = false;
	}
	num_black = num_white = num_null = 0;
	ok = false;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (floor[i][j].mouseOver()) {
				now = Point(i, j);
			}
			if (col[i][j] == black)++num_black;
			if (col[i][j] == white)++num_white;
			if (col[i][j] == null) {
				++num_null;
				if (can(i,j)) {
					ok = true;
				}
			}
		}
	}

	if (!ok) {
		next();
		bool ok_ = false;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (col[i][j] == null) {
					if (can(i, j)) {
						ok_ = true;
					}
				}
			}
		}
		next();
		if (!ok_) {
			finish = true;
		}
	}

	if (MouseL.down() && col[now.x][now.y] == null && can(now.x,now.y)) {

		OK = Array<bool>(8, false);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[0] = turn(now.x, now.y, 0, -1);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[1] = turn(now.x, now.y, 0, 1);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[2] = turn(now.x, now.y, 1, -1);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[3] = turn(now.x, now.y, 1, 0);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[4] = turn(now.x, now.y, 1, 1);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[5] = turn(now.x, now.y, -1, -1);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[6] = turn(now.x, now.y, -1, 0);
		col[now.x][now.y] = (now_turn == black ? white : black);
		OK[7] = turn(now.x, now.y, -1, 1);

		if (OK[0] || OK[1] || OK[2] || OK[3] || OK[4] || OK[5] || OK[6] || OK[7]) {
			col[now.x][now.y] = now_turn;
			next();
		}
		else {
			col[now.x][now.y] = null;
		}
	}

	if (pass.leftClicked()) {
		if (finish) {
			num_black = num_white = 2;
			num_null = 60;
			now_turn = black;
			col = Grid<color>(8, 8, null);
			col[3][3] = col[4][4] = black;
			col[3][4] = col[4][3] = white;
			finish = false;
		}
		else if (!finish && !ok) {
			next();
		}
	}


	++Frame;
}

void Game::draw() const {
	pass.draw(ok?Palette::Gray:Palette::Blue);
	pass.drawFrame(3, 0, Palette::Darkblue);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			floor[i][j].draw((floor[i][j].mouseOver() && col[i][j] == null) ? (possible ? ColorF(0, 0, 0, 0.3) : ColorF(1, 0, 0, 0.8)) : ColorF(0, 255, 0));
			floor[i][j].drawFrame(1, 0, Palette::Black);
			if (col[i][j] == black) {
				Circle(floor[i][j].center(), 30).draw(Palette::Black);
			}
			if (col[i][j] == white) {
				Circle(floor[i][j].center(), 30).draw(Palette::White);
				Circle(floor[i][j].center(), 30).drawFrame(2, 0, Palette::Black);
			}
		}
	}
	if (now_turn == black) {
		info(U"黒の番").drawAt(Scene::Width() / 2, 25, Palette::Black);
	}
	else if (now_turn == white) {
		info(U"白の番").drawAt(Scene::Width() / 2, 25, Palette::Orangered);
	}
	Rect(20, 150, 560).drawFrame(0, 1, Palette::Black);
	Circle(160, 290, 5).draw(Palette::Black);
	Circle(440, 290, 5).draw(Palette::Black);
	Circle(160, 570, 5).draw(Palette::Black);
	Circle(440, 570, 5).draw(Palette::Black);

	if (num_null == 0 || finish) {
		info(U"もういちど").drawAt(pass.center());
		Rect(0, 300, 600, 150).draw(ColorF(1, 1, 1, 0.8));
		if (num_black > num_white) {
			result(U"黒の勝ち！").drawAt(Rect(0, 300, 600, 150).center(), Palette::Black);
		}
		else if (num_white > num_black) {
			result(U"白の勝ち！").drawAt(Rect(0, 300, 600, 150).center(), Palette::Orangered);
		}
		else {
			result(U"引き分け！").drawAt(Rect(0, 300, 600, 150).center(), Palette::Blue);
		}
	}
	else {
		info(U"パス").drawAt(pass.center());
	}
	info(U"黒：{}個"_fmt(num_black)).draw(10, 100, Palette::Black);
	info(U"白：{}個"_fmt(num_white)).draw(430, 100, Palette::Orangered);
}

bool Game::turn(int32 i, int32 j, int32 a, int32 b) {
	if (col[i][j] == black) {
		if (i + a < 0 || i + a>7 ||
			j + b < 0 || j + b>7) {
			return false;
		}
		else if (col[i + a][j + b] == null) {
			return false;
		}
		else if (col[i + a][j + b] == black) {
			if (turn(i + a, j + b, a, b)) {
				col[i][j] = white;
				return true;
			}
			else {
				return false;
			}
		}
		else if (col[i + a][j + b] == white) {
			col[i][j] = white;
			return true;
		}
	}
	if (col[i][j] == white) {
		if (i + a < 0 || i + a>7 ||
			j + b < 0 || j + b>7) {
			return false;
		}
		else if (col[i + a][j + b] == null) {
			return false;
		}
		else if (col[i + a][j + b] == white) {
			if (turn(i + a, j + b, a, b)) {
				col[i][j] = black;
				return true;
			}
			else {
				return false;
			}
		}
		else if (col[i + a][j + b] == black) {
			col[i][j] = black;
			return true;
		}
	}
	return false;
}

void Game::next() {
	if (now_turn == black) {
		now_turn = white;
	}
	else if (now_turn == white) {
		now_turn = black;
	}
}

bool Game::can(int32 i, int32 j)/*そのマスに置けるかどうか*/ {
	if ((is(i - 1, j - 1) ? (col[i - 1][j - 1] == (now_turn == black ? white : black) && can(i, j, -1, -1)) : false) ||
		(is(i - 1, j) ? (col[i - 1][j] == (now_turn == black ? white : black) && can(i, j, -1, 0)) : false) ||
		(is(i - 1, j + 1) ? (col[i - 1][j + 1] == (now_turn == black ? white : black) && can(i, j, -1, 1)) : false) ||
		(is(i, j - 1) ? (col[i][j - 1] == (now_turn == black ? white : black) && can(i, j, 0, -1)) : false) ||
		(is(i, j + 1) ? (col[i][j + 1] == (now_turn == black ? white : black) && can(i, j, 0, 1)) : false) ||
		(is(i + 1, j - 1) ? (col[i + 1][j - 1] == (now_turn == black ? white : black) && can(i, j, 1, -1)) : false) ||
		(is(i + 1, j) ? (col[i + 1][j] == (now_turn == black ? white : black) && can(i, j, 1, 0)) : false) ||
		(is(i + 1, j + 1) ? (col[i + 1][j + 1] == (now_turn == black ? white : black) && can(i, j, 1, 1)) : false)
		) {
		return true;
	}
	return false;
}

bool Game::can(int32 i, int32 j, int32 a, int32 b)/*そのマス、その向きで返せるかどうか*/ {
	if (col[i][j] == black||(col[i][j]==null&&now_turn==white)) {
		if (i + a < 0 || i + a>7 ||
			j + b < 0 || j + b>7) {
			return false;
		}
		else if (col[i + a][j + b] == null) {
			return false;
		}
		else if (col[i + a][j + b] == black) {
			if (can(i + a, j + b, a, b)) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (col[i + a][j + b] == white) {
			return true;
		}
	}
	if (col[i][j] == white || (col[i][j] == null && now_turn == black)) {
		if (i + a < 0 || i + a>7 ||
			j + b < 0 || j + b>7) {
			return false;
		}
		else if (col[i + a][j + b] == null) {
			return false;
		}
		else if (col[i + a][j + b] == white) {
			if (can(i + a, j + b, a, b)) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (col[i + a][j + b] == black) {
			return true;
		}
	}
}

bool Game::is(int32 i, int32 j) {
	if (i < 0 || i > 7 || j < 0 || j > 7) {
		return false;
	}
	return true;
}