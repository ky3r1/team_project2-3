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

    void NextMoveArea(DirectX::XMFLOAT3& position);
private:
    int category = -10;
};
