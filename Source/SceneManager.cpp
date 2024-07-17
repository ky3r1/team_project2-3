#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
    if (nextScene != nullptr)
    {
        //�Â��V�[�����I��
        Clear();

        //�V�����V�[����ݒ�
        currentScene = nextScene;
        nextScene = nullptr;

        //�V�[��������
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
    //�V�����V�[����ݒ�
    nextScene = scene;
}

void SceneManager::HoldChangeScene(Scene* scene)
{
    //�V�����V�[����ݒ�
    nextScene = scene;
    // ���݂̃V�[���̕ێ�
    destScene = currentScene;
}
