#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
    if (nextScene != nullptr)
    {
        //古いシーンを終了
        Clear();

        //新しいシーンを設定
        currentScene = nextScene;
        nextScene = nullptr;

        //シーン初期化
        if (currentScene->IsReady() == false)
        {
            currentScene->Initialize();
        }
    }

    if (currentScene != nullptr)
    {
        currentScene->Update(elapsedTime);
    }
}

void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

void SceneManager::Render(Scene* scene)
{
    scene->Render();
}

void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    //新しいシーンを設定
    nextScene = scene;
}

void SceneManager::HoldChangeScene(Scene* scene)
{
    //新しいシーンを設定
    nextScene = scene;
    // 現在のシーンの保持
    destScene = currentScene;
}
