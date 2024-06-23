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

DirectX::XMFLOAT3 StageMapChip::GetIDPosition(int id)
{
	for (int z = 0; z < MAPMAX_Z; z++)
	{
		for (int x = 0; x < MAPMAX_X; x++)
		{
			if (id == this->id[z][x])
			{
				return position[z][x];
			}
		}
	}
	return DirectX::XMFLOAT3(100, 100, 100);
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

DirectX::XMFLOAT3 StageMapChip::GetPosGetAway(DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 dir)
{
	DirectX::XMFLOAT3 position = p;
	position.x += dir.x;
	position.y += dir.y;
	position.z += dir.z;
	return position;
}

void StageMapChip::SetData(DirectX::XMFLOAT3 p, int x, int z)
{
    id[z][x] = id_count;
    position[z][x] = p;
	if(category[stagenum][z][x]!=FLOOR)		cost[z][x] = 10000;
	else		cost[z][x] = 0;
	id_count++;
}
