#include "EnemyMoveSystem.h"
#include "Player.h"
#include "StageMapChip.h"
#include "Player.h"
#include "Mathf.h"

#define NODERIGHT 0
#define NODELEFT 1
#define NODEUP 2
#define NODEDOWN 3

void EnemyMoveSystem::NextMoveArea(DirectX::XMFLOAT3& position)
{
    StageMapChip& map_chip = StageMapChip::Instance();
    DirectX::XMFLOAT3 player_position = Player::Instance().GetPosition();
    bool checker = false;
    int count = 0;
    std::vector<float> cost = {};
    DirectX::XMFLOAT3 away_pos = {};
    DirectX::XMFLOAT3 away_distance = {};
    while (checker)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int index = 0; index < count; index++)
            {
                //awaydistanceæ‚ÌˆÊ’u
                away_pos = map_chip.GetPosGetAway(position, away_distance);
                //awaydistanceæ‚ÌˆÊ’u‚ÆplayerˆÊ’u‚Ì‹——£
                float dir = Mathf::Pythagoras(player_position, away_pos);
                cost.push_back(map_chip.GetOnCost(away_pos) + dir);
            }
        }
        count++;
    }
}
