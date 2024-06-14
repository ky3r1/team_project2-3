#include "EnemyMoveSystem.h"
#include "Player.h"
#include "StageMapChip.h"

void EnemyMoveSystem::NextMoveArea(DirectX::XMFLOAT3& position)
{
    if (category != StageMapChip::Instance().GetOnCategory(position))
    {

    }
    category = StageMapChip::Instance().GetOnCategory(position);
}
