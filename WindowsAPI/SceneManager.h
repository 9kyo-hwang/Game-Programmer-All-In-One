#pragma once
#include "Singleton.h"

enum class ESceneType
{
	None,
	Development,
	Gameplay,
};

class SceneManager : public Singleton<SceneManager>
{
public:
	void Initialize();
	void Update();
	void Render(HDC InHandleDeviceContext);

public:
	void LoadScene(ESceneType NewType);
	void Clear();

private:
	class Scene* ActiveScene = nullptr;
	ESceneType Type = ESceneType::None;
};