#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"
//#include "Player.h"

//タイトルシーン
class SceneResult : public Scene
{
public:
    SceneResult(bool oc) { outcome = oc; }
    ~SceneResult() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

private:
    std::unique_ptr<Sprite> sprite = nullptr;
    std::unique_ptr<AudioSource> audio = nullptr;
    bool outcome = false;
};