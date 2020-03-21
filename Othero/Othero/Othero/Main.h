#pragma once
#include<Siv3D.hpp>

struct GameData {
	
};

using MyApp = SceneManager<String, GameData>;

namespace GameInfo {
	const int32 Width = 600;
	const int32 Height = 750;
	const String Title = U"ÉIÉZÉç";
	const ColorF FadeinColor = Palette::Lightgreen;
}