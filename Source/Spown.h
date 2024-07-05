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
    //�G�l�~�[�������_���ŏo��
    Enemy* SpownEnemyCategory();

    void MakeWave(Enemy* enemy,int count, int timer, int flg,int spown_timer);

    bool flg[10];
};