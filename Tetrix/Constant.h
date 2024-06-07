#pragma once
#include<map>
#include <string>

using namespace std;

const int GRID_SIZE = 6;
const int COLUMNS = 9;
const int ROWS = 20;
const int CELL_SIZE = 8;

const int WIDTH = COLUMNS * GRID_SIZE * CELL_SIZE;
const int HEIGHT = ROWS * GRID_SIZE * CELL_SIZE;

double FALL_SPEED = 1.0;

int INITIAL_Y = 0;

const map<string, function<void()>> DIFFICULTY_SETTING = {
	{"Easy", []() {
		FALL_SPEED = 1.0;
		INITIAL_Y = 0;
	}},
	{"Normal", []() {
		FALL_SPEED = 1.2;
		INITIAL_Y = 0;
	}},
	{"Hard", []() {
		FALL_SPEED = 1.8;
		INITIAL_Y = CELL_SIZE;
	}},
	{"Lunatic", []() {
		FALL_SPEED = 3.0;
		INITIAL_Y = 3 * CELL_SIZE;
	}}
};

const vector<string> DIFFICULTY_INDEX = {
	"Easy",
	"Normal",
	"Hard",
	"Lunatic"
};

bool GAME_OVER = false;