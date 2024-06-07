#pragma once
#include<easy2d/easy2d.h>
#include <vector>
#include <array>
#include <queue>
#include <map>

#include"Constant.h"
#include"Sandtrimino.h"
#include"Scoredata.h"

using namespace easy2d;

class SandtrixGame : public easy2d::Node {
public:
	std::array<std::array<SandtriminoColor, COLUMNS* CELL_SIZE>, ROWS* CELL_SIZE> grid = { SandtriminoColor::NONE };
	Sandtrimino currentTetrimino;
	int score = 0;
	double frameAccumulation = 0.0f;
	double fallSpeedMultiplier = 1.0f;

	void onUpdate() override {
		if (frameAccumulation >= 1.0) {
			int n = std::floor(frameAccumulation);
			for (int i = 0; i < n; i++) {
				onFall();
				onScore();
			}
			frameAccumulation -= (double)n;
		}
		frameAccumulation += FALL_SPEED*fallSpeedMultiplier;
		fallSpeedMultiplier = min(1.001 * fallSpeedMultiplier, 1.2);
		onSimulate();
		onControl();
	}

	void onFall() {
		currentTetrimino.y += 1;
		if (checkCollision(currentTetrimino, 0, 0)) {
			currentTetrimino.y -= 1;
			for (size_t y = 0; y < currentTetrimino.shape.size(); ++y) {
				for (size_t x = 0; x < currentTetrimino.shape[y].size(); ++x) {
					if (currentTetrimino.shape[y][x] == 1) {
						grid[currentTetrimino.y + y][currentTetrimino.x + x] = currentTetrimino.color;
					}
				}
			}
			currentTetrimino = Sandtrimino();
			if (checkCollision(currentTetrimino, 0, 0)) {
				Scoredata::save(getScore());
				GAME_OVER = true;
				SceneManager::back();
			}
		}
	}

	void onScore() {
		SandtriminoColor currentColor = SandtriminoColor::NONE;
		for (int y = 0; y < ROWS * CELL_SIZE; y++) {
			if (currentColor != grid[y][0]) {
				currentColor = grid[y][0];
			}
			else {
				continue;
			}

			std::vector<std::pair<int, int>> scoredCells;
			bool found = findScoredCells(scoredCells, y, 0);
			if (found) {
				score += scoredCells.size();
				clearScoredCells(scoredCells);
			}
		}
	}

	void onSimulate() {
		for (int y = grid.size() - 1; y >= 0; y--) {
			for (int x = 0; x < grid[0].size(); x++) {
				if (grid[y][x] != SandtriminoColor::NONE) {
					int l = x - 1;
					int r = x + 1;
					int b = y + 1;
					if (b < grid.size()) {
						if (grid[b][x] == SandtriminoColor::NONE) {
							grid[b][x] = grid[y][x];
							grid[y][x] = SandtriminoColor::NONE;
						}
						if (l >= 0 && grid[b][l] == SandtriminoColor::NONE) {
							grid[b][l] = grid[y][x];
							grid[y][x] = SandtriminoColor::NONE;
						}
						else if (r < grid[0].size() && grid[b][r] == SandtriminoColor::NONE) {
							grid[b][r] = grid[y][x];
							grid[y][x] = SandtriminoColor::NONE;
						}
					}
				}
			}
		}
	}

	void onControl() {
		if (Input::isDown(KeyCode::Left)) {
			if (!checkCollision(currentTetrimino, -1, 0)) {
				currentTetrimino.x -= 1;
			}
		}
		if (Input::isDown(KeyCode::Right)) {
			if (!checkCollision(currentTetrimino, 1, 0)) {
				currentTetrimino.x += 1;
			}
		}
		if (Input::isDown(KeyCode::Down)) {
			if (!checkCollision(currentTetrimino, 0, 1)) {
				currentTetrimino.y += 1;
			}
		}
		if (Input::isPressed(KeyCode::Up)) {
			currentTetrimino.rotate();
			if (checkCollision(currentTetrimino, 0, 0)) {
				currentTetrimino.rotate();
				currentTetrimino.rotate();
				currentTetrimino.rotate();
			}
		}
	}

	int getScore() const {
		return std::ceil(((double)score)*getScoreMultiplier());
	}

	double getScoreMultiplier() const {
		string diff = Scoredata::getDifficulty();
		if (diff == "Easy") {
			return 1;
		}
		else if (diff == "Normal") {
			return 1.08;
		}
		else if (diff == "Hard") {
			return 1.2;
		}
		else if (diff == "Lunatic") {
			return 1.5;
		}
		else {
			return 1;
		}
	}

	bool checkCollision(const Sandtrimino& tetrimino, int offsetX, int offsetY) {
		for (size_t y = 0; y < tetrimino.shape.size(); ++y) {
			for (size_t x = 0; x < tetrimino.shape[y].size(); ++x) {
				if (tetrimino.shape[y][x] == 1) {
					int newX = tetrimino.x + x + offsetX;
					int newY = tetrimino.y + y + offsetY;
					if (
						newX < 0 ||
						newX >= COLUMNS * CELL_SIZE ||
						newY >= ROWS * CELL_SIZE ||
						(newY >= 0 && grid[newY][newX] != SandtriminoColor::NONE))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool findScoredCells(std::vector<std::pair<int, int>>& foundCells, int beginY, int beginX) {
		std::queue<std::pair<int, int>> checkQueue;
		std::map<std::pair<int, int>, bool> checked;
		checkQueue.push(std::make_pair(beginY, beginX));
		SandtriminoColor color = grid[beginY][beginX];
		if (color == SandtriminoColor::NONE) {
			return false;
		}
		bool found = false;
		while (!checkQueue.empty()) {
			std::pair<int, int> pos = checkQueue.front();
			checkQueue.pop();
			if (pos.second >= COLUMNS * CELL_SIZE && (!found)) {
				found = true;
			}
			if (
				pos.first < 0 ||
				pos.first >= ROWS * CELL_SIZE ||
				pos.second < 0 ||
				pos.second >= COLUMNS * CELL_SIZE
				) {
				continue;
			}
			if (checked[pos]) {
				continue;
			}
			if (grid[pos.first][pos.second] != color) {
				continue;
			}
			foundCells.push_back(pos);
			checked[pos] = true;
			checkQueue.push(std::make_pair(pos.first - 1, pos.second - 1));
			checkQueue.push(std::make_pair(pos.first - 1, pos.second));
			checkQueue.push(std::make_pair(pos.first - 1, pos.second + 1));
			checkQueue.push(std::make_pair(pos.first, pos.second + 1));
			checkQueue.push(std::make_pair(pos.first + 1, pos.second + 1));
			checkQueue.push(std::make_pair(pos.first + 1, pos.second));
			checkQueue.push(std::make_pair(pos.first + 1, pos.second - 1));
			checkQueue.push(std::make_pair(pos.first, pos.second - 1));
		}
		return found;
	}

	void clearScoredCells(std::vector<std::pair<int, int>>& scoredCells) {
		for (std::pair<int, int> cell : scoredCells) {
			grid[cell.first][cell.second] = SandtriminoColor::NONE;
		}
	}

	void onRender() override {
		for (int y = 0; y < ROWS * CELL_SIZE; ++y) {
			for (int x = 0; x < COLUMNS * CELL_SIZE; ++x) {
				drawRect(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, grid[y][x]);
			}
		}

		for (size_t y = 0; y < currentTetrimino.shape.size(); ++y) {
			for (size_t x = 0; x < currentTetrimino.shape[y].size(); ++x) {
				if (currentTetrimino.shape[y][x] == 1) {
					drawRect((currentTetrimino.x + x) * GRID_SIZE, (currentTetrimino.y + y) * GRID_SIZE, GRID_SIZE, GRID_SIZE, currentTetrimino.color);
				}
			}
		}
		
		drawAlarmLine();
	}

	void drawRect(int x, int y, int width, int height, const Color& ecolor) {
		auto renderTarget = Renderer::getRenderTarget();
		auto color = D2D1::ColorF(
			ecolor.r, ecolor.g, ecolor.b, ecolor.a
		);
		ID2D1SolidColorBrush* brush;
		renderTarget->CreateSolidColorBrush(color, &brush);

		renderTarget->FillRectangle(
			D2D1::RectF(x, y, x + width, y + height),
			brush
		);
	}

	void drawAlarmLine() {
		drawRect(0, INITIAL_Y*GRID_SIZE, COLUMNS * CELL_SIZE * GRID_SIZE, GRID_SIZE, Color::Red);
	}
};