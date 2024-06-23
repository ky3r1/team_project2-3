#pragma once
#include "All.h"

class EnemyMoveSystem
{
private:
    EnemyMoveSystem() {}
    ~EnemyMoveSystem() {}
public:
    //唯一のインスタンス取得
    static EnemyMoveSystem& Instance()
    {
        static EnemyMoveSystem instance;
        return instance;
    }

    void NextMoveArea(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& target_position);
private:
    float heuristic(DirectX::XMFLOAT3 player_position, DirectX::XMFLOAT3 enemy_position) { return abs(enemy_position.x - player_position.x) + abs(enemy_position.y - player_position.y) + abs(enemy_position.z - player_position.z); }
private:
    int category = -10;
    struct  Node
    {
        DirectX::XMFLOAT3 position;
        int cost;
        Node(DirectX::XMFLOAT3 p, int c) :position(p), cost(c) {}
        bool operator<(const Node& other) const { return cost > other.cost; }
    };
};
