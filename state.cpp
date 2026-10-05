#include "state.h"

int Width = 0;
int Height = 0;

Position AgentPosition{};
Position DoorPosition{};

std::vector<std::vector<bool>> Obstacles;
std::vector<std::vector<bool>> ObstaclesObserved;
std::vector<std::vector<bool>> CantMoveRightFrom;
std::vector<std::vector<bool>> CantMoveDownFrom;
std::vector<std::vector<bool>> CantMoveUpFrom;

std::vector<bool> CantMoveAroundByDown;
std::vector<bool> CantMoveAroundByUp;
std::vector<bool> CanRemoveObstacle;

void ResizeMapState(int height, int width) {
    Height = height;
    Width = width;

    Obstacles.assign(Height, std::vector<bool>(Width, false));
    ObstaclesObserved.assign(Height, std::vector<bool>(Width, false));
    CantMoveRightFrom.assign(Height, std::vector<bool>(Width, false));
    CantMoveDownFrom.assign(Height, std::vector<bool>(Width, false));
    CantMoveUpFrom.assign(Height, std::vector<bool>(Width, false));

    CantMoveAroundByDown.assign(Width, false);
    CantMoveAroundByUp.assign(Width, false);
    CanRemoveObstacle.assign(Width, false);
}

bool IsAgentAtPosition(int x, int y) {
    return AgentPosition.x == x && AgentPosition.y == y;
}

bool ObstacleAtPosition(int x, int y) {
    return Obstacles[y][x];
}

bool WallRightOfAgent() {
    return AgentPosition.x + 1 >= Width;
}

bool IsAgentAtDoor() {
    return AgentPosition.x == DoorPosition.x && AgentPosition.y == DoorPosition.y;
}
