#include "pch.h"
#include "MenuScene.h"
#include "InputManager.h"
#include "MeshLine.h"
#include "SceneManager.h"
#include "ResourceManager.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
}

void MenuScene::Update()
{
	if (InputManager::Get()->GetButtonDown(EKeyCode::E))
	{
		SceneManager::Get()->LoadScene(ESceneType::Gameplay);
	}
}

void MenuScene::Render(HDC InDC)
{
	if (const MeshLine* Menu = ResourceManager::Get()->GetMeshLine(L"Menu"))
	{
		Menu->Render(InDC, Vector{ 0, 0 });
	}
}
