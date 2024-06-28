#include "Enemy01.h"
#include "StateDerived.h"
//#include "StageMapChip.h"
#include "ProjectileManager.h"

//TODO:弾のDelayTime
#define DELAYAUTOTIME 40

//コンストラクタ
Enemy01::Enemy01(int category)
{
    //TODO:エネミースライムのステータス設定
    model = new Model("Data/Model/Enemy/enemy01.mdl");
    static int id_enemy01 = 0;
    id_enemy01++;
    category_id = id_enemy01;
    name = std::string("Enemy01:") + std::to_string(category_id);
    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.02f;

    radius = 0.5f;//当たり判定の幅、半径
    height = 1.0f;//当たり判定の高さ
    health = 10.0f;//体力

    attack_range = 5.0f;
    this->category = category;
    //ChangeColor(color, category);

#ifdef ENEMYSTATEMACHINE
    // StateMachineを生成し、階層型ステートマシンに対応するように登録ステートを変更していく。
    stateMachine = new StateMachine();
    // ステートマシンにステート登録
    stateMachine->RegisterState(new SearchState(this));
    stateMachine->RegisterState(new BattleState(this));
    // 各親ステートにサブステートを登録
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Search), new DeathState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Search), new IdleState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new PursuitState(this));
    stateMachine->RegisterSubState(static_cast<int>(Enemy01::State::Battle), new AttackState(this));
    // デフォルトステートをセット
    stateMachine->SetState(static_cast<int>(State::Search));
#endif // ENEMYSTATEMACHINE
}

//デストラクタ
Enemy01::~Enemy01()
{
    delete model;
}

//更新処理
void Enemy01::Update(float elapsedTime)
{
    Enemy::Update(elapsedTime);
#ifdef ENEMYSTATEMACHINE
    //ステート更新
    stateMachine->Update(elapsedTime);
#endif // ENEMYSTATEMACHINE

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibleTime(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();
    //モデル行列更新
    model->UpdateTransform(transform);
    model->UpdateAnimation(elapsedTime);

    UpdateDelayTime(projectile_auto.checker, projectile_auto.time, DELAYAUTOTIME);
}

//描画処理
void Enemy01::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model, color);
}

//死亡したときに呼ばれる
void Enemy01::OnDead()
{
    Destroy();
}

//void Enemy01::DrawDebugGUI()
//{
//    //name = std::string("Enemy01:") + std::to_string(category_id);
//
//    //if (ImGui::TreeNode(name.c_str()))
//    //{
//    //    Enemy::DrawDebugGUI();
//    //    std::string p = std::string("position") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(p.c_str(), &position.x, -5, 5);
//    //    std::string s = std::string("scale") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(s.c_str(), &scale.x, 0.01f, 4.0f);
//    //    std::string a = std::string("angle") + std::to_string(category_id);
//    //    ImGui::SliderFloat3(a.c_str(), &angle.x, -3.14f, 3.14f);
//    //    ImGui::TreePop();
//    //}
//}

void Enemy01::DrewDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Enemy::DrewDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, 1.3f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    // 攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attack_range, 1.0f, DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f));
}
