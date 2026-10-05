#include "agent_logic.h"

#include "history.h"
#include "state.h"

#include <cstdio>
#include <string>

namespace {
void InferBlockedBypass(int obstacleX, int obstacleY) {
    int agentX = obstacleX - 1;
    int agentY = obstacleY;

    bool blockedRight = true;
    bool blockedDown = false;
    bool blockedUp = false;

    if (agentY + 1 >= Height) {
        blockedDown = true;
    } else if (Obstacles[agentY + 1][agentX]) {
        blockedDown = true;
    }

    if (agentY - 1 < 0) {
        blockedUp = true;
    } else if (Obstacles[agentY - 1][agentX]) {
        blockedUp = true;
    }

    std::string rightObs = "ObstacleAt (x: " + std::to_string(obstacleX) + ", y: " + std::to_string(obstacleY) + ")";

    if (blockedRight && blockedDown) {
        CantMoveAroundByDown[agentX] = true;
        AddInference(
            {
                rightObs,
                (agentY + 1 >= Height)
                    ? "Wall (x: " + std::to_string(agentX) + ", y: " + std::to_string(agentY + 1) + ")"
                    : "ObstacleDown/Blocked (x: " + std::to_string(agentX) + ", y: " + std::to_string(agentY + 1) + ")"
            },
            "CantMoveAroundByDown (x: " + std::to_string(agentX) + ")"
        );
    }

    if (blockedRight && blockedUp) {
        CantMoveAroundByUp[agentX] = true;
        AddInference(
            {
                rightObs,
                (agentY - 1 < 0)
                    ? "Wall (x: " + std::to_string(agentX) + ", y: " + std::to_string(agentY - 1) + ")"
                    : "ObstacleUp/Blocked (x: " + std::to_string(agentX) + ", y: " + std::to_string(agentY - 1) + ")"
            },
            "CantMoveAroundByUp (x: " + std::to_string(agentX) + ")"
        );
    }

    if (CantMoveAroundByDown[agentX] && CantMoveAroundByUp[agentX]) {
        CanRemoveObstacle[agentX] = true;
        AddInference(
            {
                "CantMoveAroundByDown (x: " + std::to_string(agentX) + ")",
                "CantMoveAroundByUp (x: " + std::to_string(agentX) + ")"
            },
            "CanRemoveObstacle (x: " + std::to_string(agentX) + ")"
        );
    }
}
}

void InferFromLatestObservation(const std::string& observation) {
    int x = 0;
    int y = 0;

    if (std::sscanf(observation.c_str(), "ObstacleAt (x: %d, y: %d)", &x, &y) == 2) {
        std::string inf = "CantMoveRightFrom (x: " + std::to_string(x - 1) + ", y: " + std::to_string(y) + ")";
        AddInference({observation}, inf);
        CantMoveRightFrom[y][x - 1] = true;
        InferBlockedBypass(x, y);
    } else if (std::sscanf(observation.c_str(), "Wall (x: %d, y: %d)", &x, &y) == 2) {
        if (y >= Height) {
            std::string inf = "CantMoveDownFrom (x: " + std::to_string(x) + ", y: " + std::to_string(y - 1) + ")";
            AddInference({observation}, inf);
            CantMoveDownFrom[y - 1][x] = true;
        } else if (y < 0) {
            std::string inf = "CantMoveUpFrom (x: " + std::to_string(x) + ", y: " + std::to_string(y + 1) + ")";
            AddInference({observation}, inf);
            CantMoveUpFrom[y + 1][x] = true;
        }
    }
}

void MakeObservations() {
    if (IsAgentAtDoor()) {
        AddObservation("Agent has reached the door!");
    } else if (AgentPosition.x + 1 >= Width) {
        std::string obs = "Wall (x: " + std::to_string(AgentPosition.x + 1) + ", y: " + std::to_string(AgentPosition.y) + ")";
        AddObservation(obs);
        CantMoveRightFrom[AgentPosition.y][AgentPosition.x] = true;
        InferBlockedBypass(AgentPosition.x + 1, AgentPosition.y);
    } else if (ObstacleAtPosition(AgentPosition.x + 1, AgentPosition.y)) {
        if (!ObstaclesObserved[AgentPosition.y][AgentPosition.x + 1]) {
            std::string obs = "ObstacleAt (x: " + std::to_string(AgentPosition.x + 1) + ", y: " + std::to_string(AgentPosition.y) + ")";
            AddObservation(obs);
            ObstaclesObserved[AgentPosition.y][AgentPosition.x + 1] = true;
        }
    }

    if (AgentPosition.y + 1 >= Height) {
        std::string obs = "Wall (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y + 1) + ")";
        AddObservation(obs);
        CantMoveDownFrom[AgentPosition.y][AgentPosition.x] = true;
    } else if (ObstacleAtPosition(AgentPosition.x, AgentPosition.y + 1)) {
        if (!ObstaclesObserved[AgentPosition.y + 1][AgentPosition.x]) {
            std::string obs = "ObstacleDown (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y + 1) + ")";
            AddObservation(obs);
            ObstaclesObserved[AgentPosition.y + 1][AgentPosition.x] = true;
        }
    }

    if (AgentPosition.y - 1 < 0) {
        std::string obs = "Wall (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y - 1) + ")";
        AddObservation(obs);
        CantMoveUpFrom[AgentPosition.y][AgentPosition.x] = true;
    } else if (ObstacleAtPosition(AgentPosition.x, AgentPosition.y - 1)) {
        if (!ObstaclesObserved[AgentPosition.y - 1][AgentPosition.x]) {
            std::string obs = "ObstacleUp (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y - 1) + ")";
            AddObservation(obs);
            ObstaclesObserved[AgentPosition.y - 1][AgentPosition.x] = true;
        }
    }
}

void MoveAgent() {
    if (CanRemoveObstacle[AgentPosition.x] && !WallRightOfAgent()) {
        Obstacles[AgentPosition.y][AgentPosition.x + 1] = false;
        AddAction("Removed obstacle at (x: " + std::to_string(AgentPosition.x + 1) + ", y: " + std::to_string(AgentPosition.y) + ")");
        CantMoveRightFrom[AgentPosition.y][AgentPosition.x] = false;
        AddInference(
            {"Removed obstacle at (x: " + std::to_string(AgentPosition.x + 1) + ", y: " + std::to_string(AgentPosition.y) + ")"},
            "CanMoveRightFrom (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")"
        );
        CanRemoveObstacle[AgentPosition.x] = false;
    } else if (CanRemoveObstacle[AgentPosition.x] && WallRightOfAgent() &&
               AgentPosition.y + 1 < Height && Obstacles[AgentPosition.y + 1][AgentPosition.x]) {
        Obstacles[AgentPosition.y + 1][AgentPosition.x] = false;
        AddAction("Removed obstacle at (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y + 1) + ")");
        CantMoveDownFrom[AgentPosition.y][AgentPosition.x] = false;
        CantMoveAroundByDown[AgentPosition.x] = false;
        AddInference(
            {"Removed obstacle at (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y + 1) + ")"},
            "CanMoveDownFrom (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")"
        );
        CanRemoveObstacle[AgentPosition.x] = false;
    } else if (CanRemoveObstacle[AgentPosition.x] && WallRightOfAgent() &&
               AgentPosition.y - 1 >= 0 && Obstacles[AgentPosition.y - 1][AgentPosition.x]) {
        Obstacles[AgentPosition.y - 1][AgentPosition.x] = false;
        AddAction("Removed obstacle at (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y - 1) + ")");
        CantMoveUpFrom[AgentPosition.y][AgentPosition.x] = false;
        CantMoveAroundByUp[AgentPosition.x] = false;
        AddInference(
            {"Removed obstacle at (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y - 1) + ")"},
            "CanMoveUpFrom (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")"
        );
        CanRemoveObstacle[AgentPosition.x] = false;
    }

    if (!CantMoveRightFrom[AgentPosition.y][AgentPosition.x]) {
        AgentPosition.x++;
        AddAction("MovedTo (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")");
    } else if (AgentPosition.y + 1 < Height &&
               !CantMoveDownFrom[AgentPosition.y][AgentPosition.x] &&
               !CantMoveAroundByDown[AgentPosition.x]) {
        AgentPosition.y++;
        AddAction("MovedTo (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")");
    } else if (AgentPosition.y - 1 >= 0 &&
               !CantMoveAroundByUp[AgentPosition.x] &&
               !CantMoveUpFrom[AgentPosition.y][AgentPosition.x]) {
        AgentPosition.y--;
        AddAction("MovedTo (x: " + std::to_string(AgentPosition.x) + ", y: " + std::to_string(AgentPosition.y) + ")");
    }
}
