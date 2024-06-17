#include "StageMapChip.h"

StageMapChip::StageMapChip()
{
}

void StageMapChip::Clear()
{
    for (int z = 0; z < MAPMAX_Z; z++)
    {
        for (int x = 0; x < MAPMAX_X; x++)
        {
			position[z][x] = {};
            cost[z][x] = 0;
			id[z][x] = 0;
			id_count = 0;
        }
    }
}

int StageMapChip::GetCategory( int x, int z)
{
    return category[stagenum][z][x];
}

bool StageMapChip::GetOnMapChip(DirectX::XMFLOAT3 p,DirectX::XMINT2& i)
{
	for (int z = 0; z < MAPMAX_Z; z++)
	{
		for (int x = 0; x < MAPMAX_X; x++)
		{
			DirectX::XMFLOAT3 map_position = position[z][x];
			if (p.x > map_position.x-1.0f && p.x<map_position.x + 1.0f && p.z>map_position.z-1.0f && p.z < map_position.z + 1.0f)
			{
				i = { z,x };
				return true;
			}
		}
	}
	return false;
}

int StageMapChip::GetOnCategory(DirectX::XMFLOAT3 p)
{
	DirectX::XMINT2 xz = {};
	if (GetOnMapChip(p,xz))
	{
		return category[stagenum][xz.x][xz.y];
	}
	return NONE;
}

int StageMapChip::GetOnId(DirectX::XMFLOAT3 p)
{
	DirectX::XMINT2 xz = {};
	if (GetOnMapChip(p, xz))
	{
		return id[xz.x][xz.y];
	}
	return -1;
}

int StageMapChip::GetOnCost(DirectX::XMFLOAT3 p)
{
	DirectX::XMINT2 xz = {};
	if (GetOnMapChip(p, xz))
	{
		return cost[xz.x][xz.y];
	}
	return -1;
}

void StageMapChip::SetData(DirectX::XMFLOAT3 p, int x, int z)
{
    id[z][x] = id_count;
    position[z][x] = p;
	switch (category[stagenum][z][x])
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
	id_count++;
}
