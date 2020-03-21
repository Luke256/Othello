#include <Siv3D.hpp>
#include"Main.h"
#include"Game.h"

void Main()
{
	Scene::SetBackground(Palette::Lightgreen);
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(GameInfo::Width, GameInfo::Height);
	Window::Resize(GameInfo::Width, GameInfo::Height, WindowResizeOption::KeepSceneSize);
	Window::SetTitle(GameInfo::Title);

	MyApp manager;
	manager.setFadeColor(GameInfo::FadeinColor);
	manager.add<Game>(U"Game");

	Cursor::RequestStyle(CursorStyle::Arrow);

	while (System::Update()) {
		if (!manager.update())break;
	}
}
