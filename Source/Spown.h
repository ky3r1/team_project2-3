#pragma once
#include "All.h"
#include "Enemy.h"

class Spown
{
public:
    //C:category �G�̎��
    //P:position ���W
    
    //�B��̃C���X�^���X�擾
    static Spown& Instance()
    {
        static Spown instance;
        return instance;
    }

    void Update(float elapsedTime);

    void render(ID3D11DeviceContext* dc, Shader* shader);
private:

    void MakeWave(int category, int count, Enemy* enemy,DirectX::XMFLOAT2 top, DirectX::XMFLOAT2 middle);

    bool flg[10] = {};
};