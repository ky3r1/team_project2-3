#include "EnemyMoveSystem.h"
#include "Player.h"
//#include "StageMapChip.h"
#include "Player.h"
#include "Mathf.h"

#define NODERIGHT 0
#define NODELEFT 1
#define NODEUP 2
#define NODEDOWN 3

#define COST 2

void EnemyMoveSystem::NextMoveArea(DirectX::XMFLOAT3& position,DirectX::XMFLOAT3& target_position)
{
    //StageMapChip& map_chip = StageMapChip::Instance();
    //Player& player = Player::Instance();
    //DirectX::XMFLOAT3 enemy_to_player = { Player::Instance().GetPosition().x - position.x, 0.0f, Player::Instance().GetPosition().z - position.z };
    //DirectX::XMFLOAT3 next_move = { };
    //if (enemy_to_player.x > 0.0f)next_move.x = 2;
    //if (enemy_to_player.x <= 0.0f)next_move.x = -2;
    //if (enemy_to_player.z > 0.0f)next_move.z= 2;
    //if (enemy_to_player.z <= 0.0f)next_move.z = -2;
    //if (map_chip.GetOnCost(DirectX::XMFLOAT3(position.x + next_move.x, position.y+next_move.y, position.z + next_move.z)) > 20)
    //{
    //    DirectX::XMFLOAT3 k = {};
    //    float distance=FLT_MAX;
    //    for(int i = 0; i < 4; i++)
    //    {

    //        switch (i)
    //        {
    //        case 0:
    //            //ã
    //            k = {0,0,1};
    //            break;
    //        case 1:
    //            //‰º
    //            k = { 0,0,-1 };
    //            break;
    //        case 2:
    //            //‰E
    //            k = { 1,0,0 };
    //            break;
    //        case 3:
    //            //¶
    //            k = { -1,0,0 };
    //            break;
    //        default:
    //            break;
    //        }
    //        if(map_chip.GetOnCost(DirectX::XMFLOAT3(position.x + k.x * COST, position.y + k.y * COST, position.z + k.z * COST)) > 20)
    //        {
    //            continue;
    //        }
    //        if(Mathf::Pythagoras(player.GetPosition(), DirectX::XMFLOAT3(position.x + k.x * COST, position.y + k.y * COST, position.z + k.z * COST)) < distance)
    //        {
    //            distance= Mathf::Pythagoras(player.GetPosition(), DirectX::XMFLOAT3(position.x + k.x * COST, position.y + k.y * COST, position.z + k.z * COST));
    //            target_position = DirectX::XMFLOAT3(position.x + k.x * COST, position.y + k.y * COST, position.z + k.z * COST);
    //        }
    //    }
    //}
}
