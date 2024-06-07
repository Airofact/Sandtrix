#pragma once
#include<easy2d/easy2d.h>
#include<map>

#include"Constant.h"
#include"GameScene.h"
#include"Scoredata.h"

using namespace easy2d;

class MenuScene : public Scene {
public:
	std::vector<Text*> buttons;
	Text* titleText;
	Text* usernameText;
	Text* highScoreText;
	bool inputting = false;
	std::vector<Text*> diffSelect;
	MenuScene() {
		titleText = gcnew Text("Sandtrix");
		titleText->setPos(Point(WIDTH / 2, HEIGHT / 2-60));
		titleText->setAnchor(0.5, 0.5);
		titleText->setAlignment(TextAlign::Center);
		titleText->setFontSize(60);
		titleText->setFontFamily("DinkieBitmap 7pxDemo");
		addChild(titleText);
		usernameText = gcnew Text("");
		usernameText->setPos(Point(0, 0));
		usernameText->setFontFamily("DinkieBitmap 7pxDemo");
		updateUsernameText(Scoredata::getUserName());
		addChild(usernameText);
		highScoreText = gcnew Text("");
		highScoreText->setPos(Point(Window::getWidth(), 0));
		highScoreText->setAnchor(1.0f,0.0f);
		highScoreText->setAlignment(TextAlign::Right);
		highScoreText->setFontFamily("DinkieBitmap 7pxDemo");
		addChild(highScoreText);
		addButton("Start", Point(WIDTH/2, HEIGHT/2), [](ButtonEvent e) {
			if (e == ButtonEvent::Clicked) {
				SceneManager::enter(gcnew GameScene());
			}
		});
		addButton("Set User Name", Point(WIDTH / 2, HEIGHT / 2+30), [this](ButtonEvent e) {
			if (e == ButtonEvent::Clicked) {
				enableInput(true);
				Scoredata::setUserName("");
			}
		});
		{
			for (int index = 0; index < 4; index++) {
				string diff = DIFFICULTY_INDEX.at(index);
				function<void()> callback =  DIFFICULTY_SETTING.at(diff);
				diffSelect.push_back(addButton(diff, Point(WIDTH / 2, HEIGHT / 2 + 30 * (index+2)), [this, diff, callback, index](ButtonEvent e) {
					if (e == ButtonEvent::Clicked) {
						Scoredata::setDifficulty(diff);
						callback();
						diffSelect[index]->setFillColor(Color::SkyBlue);
						for (auto& diffButton : diffSelect) {
							if (diffButton->getText() == diff) {
								diffButton->setFillColor(Color::SkyBlue);
							}
							else {
								diffButton->setFillColor(Color::White);
							}
						}
					}
				}));
			}
		}
		for (auto& diff : diffSelect) {
			if (diff->getText() == Scoredata::getDifficulty()) {
				diff->setFillColor(Color::SkyBlue);
				DIFFICULTY_SETTING.at(Scoredata::getDifficulty())();
			}
		}
	}

	void onEnter() override {
		Scoredata::loadScores();
	}

	void enableInput(bool enable) {
		inputting = enable;
	}

	void onUpdate() override {
		if (inputting) {
			usernameText->setFillColor(Color::SkyBlue);
			std::string username = Scoredata::getUserName();
			std::map<KeyCode::Value, char> keyMap = {
				{KeyCode::A,'A'},
				{KeyCode::B,'B'},
				{KeyCode::C,'C'},
				{KeyCode::D,'D'},
				{KeyCode::E,'E'},
				{KeyCode::F,'F'},
				{KeyCode::G,'G'},
				{KeyCode::H,'H'},
				{KeyCode::I,'I'},
				{KeyCode::J,'J'},
				{KeyCode::K,'K'},
				{KeyCode::L,'L'},
				{KeyCode::M,'M'},
				{KeyCode::N,'N'},
				{KeyCode::O,'O'},
				{KeyCode::P,'P'},
				{KeyCode::Q,'Q'},
				{KeyCode::R,'R'},
				{KeyCode::S,'S'},
				{KeyCode::T,'T'},
				{KeyCode::U,'U'},
				{KeyCode::V,'V'},
				{KeyCode::W,'W'},
				{KeyCode::X,'X'},
				{KeyCode::Y,'Y'},
				{KeyCode::Z,'Z'},
				{KeyCode::Space,' '}
			};
			std::map<KeyCode::Value, char> lowerKeyMap{};
			for (auto& pair : keyMap) {
				lowerKeyMap.insert({ pair.first, tolower(pair.second) });
			}
			if (Input::isDown(KeyCode::Shift)) {
				for (auto& key : keyMap) {
					if (Input::isPressed(key.first)) {
						username += key.second;
					}
				}
			}
			else {
				for (auto& key : lowerKeyMap) {
					if (Input::isPressed(key.first)) {
						username += key.second;
					}
				}
			}
			if (Input::isPressed(KeyCode::Back)) {
				if (username.size() > 0) {
					username.pop_back();
				}
			}
			updateUsernameText(username);
			Scoredata::setUserName(username);
			if (Input::isPressed(KeyCode::Enter)) {
				Scoredata::loadScores();
				enableInput(false);
			}
		}
		else {
			usernameText->setFillColor(Color::White);
		}
		if (GAME_OVER) {
			titleText->setText("Sandtrix Again!");
		}
		else {
			titleText->setText("Sandtrix");
		}
		highScoreText->setText("HighScore: " + std::to_string(Scoredata::getHighScore()));
	}

	void updateUsernameText(std::string name) {
		usernameText->setText("User Name:" + name);
	}

	Text* addButton(String text,Point pos, std::function<void(ButtonEvent)> callback) {
		Text* textNode = gcnew Text(text);
		textNode->setPos(pos);
		textNode->setAlignment(TextAlign::Center);
		textNode->setAnchor(0.5, 0.5);
		textNode->addListener(gcnew ButtonListener(callback));
		textNode->setFontFamily("DinkieBitmap 7pxDemo");
		this->addChild(textNode);
		buttons.push_back(textNode);
		return textNode;
	}
};