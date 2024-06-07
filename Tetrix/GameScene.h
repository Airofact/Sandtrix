#pragma once
#include<easy2d/easy2d.h>

#include"SandtrixGame.h"
#include"Constant.h"

using namespace easy2d;

class GameScene : public Scene {
public:
	std::vector<Text*> buttons;
	SandtrixGame* game;
	Text* scoreText;
	Text* highScoreText;
	int highScore;
	GameScene() {
		highScore = Scoredata::getHighScore();
		game = gcnew SandtrixGame();
		addChild(game);
		addButton("Save And Return To Title", Point(0, 0), [this](ButtonEvent e) {
			if (e == ButtonEvent::Clicked) {
				Scoredata::save(game->getScore());
				GAME_OVER = true;
				SceneManager::back();
			}
		});
		scoreText = gcnew Text("");
		scoreText->setPos(Point(0, 30));
		scoreText->setFontFamily("DinkieBitmap 7pxDemo");
		addChild(scoreText);
		highScoreText = gcnew Text("");
		highScoreText->setPos(Point(Window::getWidth(), 0));
		highScoreText->setAnchorX(1.0f);
		highScoreText->setFontFamily("DinkieBitmap 7pxDemo");
		addChild(highScoreText);
	}
	void onUpdate() override {
		scoreText->setText("Score:"+std::to_string(game->getScore()));
		highScoreText->setText("HighScore:" +
			std::to_string(max(highScore, game->getScore()))
		);
	}
	void addButton(String text, Point pos, std::function<void(ButtonEvent)> callback) {
		Text* textNode = gcnew Text(text);
		textNode->setPos(pos);
		textNode->setAlignment(TextAlign::Center);
		textNode->addListener(gcnew ButtonListener(callback));
		textNode->setFontFamily("DinkieBitmap 7pxDemo");
		this->addChild(textNode);
		buttons.push_back(textNode);
	}
};