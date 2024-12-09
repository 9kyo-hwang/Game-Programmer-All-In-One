#pragma once
#include "Singleton.h"

enum class ESceneType : uint8
{
	None,
	Development,
	Editor,
	Menu,
	Gameplay,
};

class SceneManager : public Singleton<SceneManager>
{
public:
	void Initialize();
	void Update();
	void Render(HDC InDC);

public:
	void LoadScene(ESceneType NewType);
	void Clear();

private:
	class Scene* ActiveScene = nullptr;
	ESceneType Type = ESceneType::None;
};