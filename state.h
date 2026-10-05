#pragma once

#include "types.h"

#include <vector>

extern int Width;
extern int Height;

extern Position AgentPosition;
extern Position DoorPosition;

extern std::vector<std::vector<bool>> Obstacles;
extern std::vector<std::vector<bool>> ObstaclesObserved;
extern std::vector<std::vector<bool>> CantMoveRightFrom;
extern std::vector<std::vector<bool>> CantMoveDownFrom;
extern std::vector<std::vector<bool>> CantMoveUpFrom;

extern std::vector<bool> CantMoveAroundByDown;
extern std::vector<bool> CantMoveAroundByUp;
extern std::vector<bool> CanRemoveObstacle;

void ResizeMapState(int height, int width);
bool IsAgentAtPosition(int x, int y);
bool ObstacleAtPosition(int x, int y);
bool WallRightOfAgent();
bool IsAgentAtDoor();
