#pragma once
#include "Main.h"

class Game :public MyApp::Scene {
private:
	enum color {
		black,
		white,
		null
	};

	Grid<Rect> floor;
	Grid<color> col;
	Array<bool> OK;
	int32 Frame;
	Point now;
	int32 num_black;
	int32 num_white;
	int32 num_null;
	color now_turn;
	Font info;
	Font result;
	RoundRect pass;
	bool ok;
	bool finish;
	bool possible;

public:
	Game(const InitData& init);
	void update() override;
	void draw()const override;
	bool turn(int32 i/*�n�_�̍��W*/, int32 j/*�n�_�̍��W*/, int32 a/*����*/, int32 b/*����*/);
	void next();
	bool can(int32 i, int32 j, int32 a, int32 b);
	bool can(int32 i, int32 j);
	bool is(int32 i, int32 j);
};