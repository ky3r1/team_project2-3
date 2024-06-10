#include "StageMapChip.h"

StageMapChip::StageMapChip()
{

}


int StageMapChip::GetMapChipData(int num, int x, int z)
{
    return map_chip_category[num][z][x];
}

int StageMapChip::GetMapChipPosition(DirectX::XMFLOAT3 p)
{
	for (int z = 0; z < MAPMAX_Z; z++)
	{
		for (int x = 0; x < MAPMAX_X; x++)
		{
			DirectX::XMFLOAT3 map_position = position[z][x];
			if (p.x<map_position.x && p.x>map_position.x + 2 && p.z<map_position.z && p.z>map_position.z + 2)
			{
				return map_chip_category[category][z][x];
			}
		}
	}
	return -1;
}

void StageMapChip::SetMapChipPosition(DirectX::XMFLOAT3 p, int x, int z)
{
    position[z][x] = p;
}
