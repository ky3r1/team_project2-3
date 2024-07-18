#pragma once

#include "Scene.h"

//シーンマネージャー
class SceneManager
{
private:
    SceneManager() {}
    ~SceneManager() {}
public:
    //唯一のインスタンス
    static SceneManager& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render();

    void Render(Scene* scene);
    //シーンクリア
    void Clear();

    //シーン切り替え
    void ChangeScene(Scene* scene);

    void HoldChangeScene(Scene* scene);

    bool GetGameLoop() {
        return game_loop;
    }

    void ChangeGameLoop() { game_loop = !game_loop; }

    Scene* destScene = nullptr;
private:
    bool game_loop = true;
    Scene* currentScene = nullptr;
    Scene* nextScene = nullptr;
};