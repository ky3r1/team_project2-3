#include "StageMapChip.h"

StageMapChip::StageMapChip()
{

}


int StageMapChip::GetMapChipCategory( int x, int z)
{
    return map_chip_category[stagenum][z][x];
}

int StageMapChip::GetMapChipPosition(DirectX::XMFLOAT3 p)
{
	for (int z = 0; z < MAPMAX_Z; z++)
	{
		for (int x = 0; x < MAPMAX_X; x++)
		{
			DirectX::XMFLOAT3 map_position = position[z][x];
			if (p.x > map_position.x && p.x<map_position.x + 2.5f && p.z>map_position.z && p.z < map_position.z + 2.5f)
			{
				return map_chip_category[stagenum][z][x];
			}
		}
	}
	return -2;
}

void StageMapChip::SetMapChipData(DirectX::XMFLOAT3 p, int x, int z)
{
    position[z][x] = p;
	switch (map_chip_category[stagenum][z][x])
	{
	case WALL:
		cost[z][x] = 100;
		break;
	case FLOOR:
		cost[z][x] = 0;
		break;
	case HOLE:
		cost[z][x] = 100;
		break;
	case SPIKE:
		cost[z][x] = 50;
		break;
	default:
		break;
	}
}
