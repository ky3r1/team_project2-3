#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//スライム
class Enemy01 : public Enemy
{
public:
	Enemy01(int category);
    ~Enemy01() override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    //弾丸入力処理
    void InputProjectile()override;

protected:
    //死亡したときに呼ばれる
    void OnDead() override;

    //デバッグ用GUI
    //void DrawDebugGUI() override;

	//デバッグオブジェクト表示
	void DrewDebugPrimitive()override;

	////前方弾発射
	//void ProjectileStraightFront(int category, float angle)override;

private:
    //Model* model = nullptr;
};