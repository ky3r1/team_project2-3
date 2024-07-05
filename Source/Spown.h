#pragma once
#include "All.h"
#include "Enemy.h"

class Spown
{
public:
    //C:category 敵の種類
    //P:position 座標
    
    //唯一のインスタンス取得
    static Spown& Instance()
    {
        static Spown instance;
        return instance;
    }

    void Update(float elapsedTime);
private:
    //エネミーを出す
    Enemy* SpownEnemyCategory(int category);

    void MakeWave(int category, int count);

    bool flg[10] = {};
};