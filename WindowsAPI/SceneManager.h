#pragma once
#include "Singleton.h"

enum class ESceneType : uint8
{
	None,
	Development,
	Editor,
};

class Scene;

class SceneManager : public Singleton<SceneManager>
{
public:
	void Initialize();
	void Update();
	void Render(HDC InDC);

public:
	void LoadScene(ESceneType NewType);
	void Clear();
	Scene* GetActiveScene() const { return ActiveScene; }

private:
	Scene* ActiveScene = nullptr;
	ESceneType Type = ESceneType::None;
};