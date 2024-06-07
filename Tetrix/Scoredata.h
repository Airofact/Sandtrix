#pragma once
#include<easy2d/easy2d.h>
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<vector>

#include"Constant.h"

using namespace easy2d;

class Scoredata {
public:
	static void setUserName(String name) {
		Data::saveString("UserName", name);
	}
	static std::string getUserName() {
		return Data::getString("UserName", "Player");
	}
	static void setDifficulty(std::string diff) {
		Data::saveString("Difficulty", diff);
	}
	static std::string getDifficulty() {
		return Data::getString("Difficulty", "Easy");
	}
	static vector<int> scores;
	static void save(int score) {
		std::ofstream file(getScoredataDirectory(), std::ios::app);
		file << getDifficulty() << score << std::endl;
		file.close();
	}
	static void loadScores() {
		std::ifstream file(getScoredataDirectory(), std::ios::app);
		//read a line and remove the difficulty content through regex
		scores.clear();
		std::string line;
		while(std::getline(file, line)){
			std::regex letterRegex("[a-zA-Z]+");
			scores.push_back(std::stoi(std::regex_replace(line, letterRegex, "")));
		}
		file.close();
	}
	static int getHighScore() {
		int highScore = 0;
		for(int score : scores){
			if(score > highScore){
				highScore = score;
			}
		}
		return highScore;
	}
	static std::string getScoredataDirectory() {
		return getUserName()+".txt";
	}
};