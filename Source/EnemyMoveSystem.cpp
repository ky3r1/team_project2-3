#include "EnemyMoveSystem.h"
#include "Player.h"
#include "StageMapChip.h"
#include "Player.h"

#define NODERIGHT 0
#define NODELEFT 1
#define NODEUP 2
#define NODEDOWN 3

void EnemyMoveSystem::NextMoveArea(DirectX::XMFLOAT3& position)
{
    bool checker = false;
    int count = 0;
    bool node_checker[4] = {};
    std::vector<int> i;
    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
    while (checker)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int index = 0; index < count; index++)
            {

            }
        }
        count++;
    }
}
