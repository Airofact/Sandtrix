#include <easy2d/easy2d.h>

#include"MenuScene.h"
#include"Scoredata.h"

using namespace easy2d;

vector<int> Scoredata::scores;

int main() {
    Logger::showConsole(false);
    if (Game::init()) {
        Scoredata::loadScores();
        Window::setTitle("Sandtrix");
        Window::setSize(COLUMNS * GRID_SIZE * CELL_SIZE, ROWS * GRID_SIZE * CELL_SIZE);
        SceneManager::enter(gcnew MenuScene());
        Game::start(30);
    }
    return 0;
}
