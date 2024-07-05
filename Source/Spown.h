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
private:
    //�G�l�~�[���o��
    Enemy* SpownEnemyCategory(int category);

    void MakeWave(int category, int count);

    bool flg[10] = {};
};