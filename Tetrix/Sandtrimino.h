#pragma once
#include<easy2d/easy2d.h>
#include <vector>

#include"SandtriminoColor.h"

using namespace easy2d;

std::vector<std::vector<std::vector<int>>> shapes = {
    {
        {1, 1, 1, 1}
    },
    {
        {1, 1, 1},
        {0, 1, 0}
    },
    {
        {1, 1},
        {1, 1}
    },
    {
        {1, 1, 0},
        {0, 1, 1}
    },
    {
        {0, 1, 1},
        {1, 1, 0}
    },
    {
        {1, 1, 1},
        {1, 0, 0}
    },
    {
        {1, 1, 1},
        {0, 0, 1}
    }
};

class Sandtrimino {
public:
    std::vector<std::vector<int>> shape;
    SandtriminoColor color;
    int x, y;

    Sandtrimino() {
        std::vector<std::vector<int>> _shape = shapes[Random::range(0.0, shapes.size() - 1)];
        shape.resize(_shape.size() * CELL_SIZE, std::vector<int>(_shape[0].size() * CELL_SIZE));
        for (size_t i = 0; i < _shape.size(); i++) {
            for (size_t j = 0; j < _shape[i].size(); j++) {
                for (size_t k = 0; k < CELL_SIZE; k++) {
                    for (size_t l = 0; l < CELL_SIZE; l++) {
                        shape[i * CELL_SIZE + k][j * CELL_SIZE + l] = _shape[i][j];
                    }
                }
            }
        }
        color = getRandomColor();
        x = COLUMNS * CELL_SIZE / 2 - shape[0].size() / 2;
        y = INITIAL_Y;
    }

    SandtriminoColor getRandomColor() {
        switch (Random::range(0, 3)) {
        case 0: return SandtriminoColor::BLUE;
        case 1: return SandtriminoColor::RED;
        case 2: return SandtriminoColor::GREEN;
        case 3: return SandtriminoColor::BROWN;
        }
    }

    void rotate() {
        std::vector<std::vector<int>> newShape(shape[0].size(), std::vector<int>(shape.size()));
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[0].size(); ++j) {
                newShape[j][shape.size() - 1 - i] = shape[i][j];
            }
        }
        shape = newShape;
    }
};

