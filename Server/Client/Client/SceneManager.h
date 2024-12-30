#pragma once
#include "Singleton.h"

enum class ESceneType : uint8
{
	None,
	Development,
	Editor,
};

class Scene;
class ALocalPlayer;

class SceneManager : public Singleton<SceneManager>
{
public:
	~SceneManager() override {}
	void Initialize();
	void Update();
	void Render(HDC InDeviceContextHandle);

public:
	void LoadScene(ESceneType NewType);
	void Clear();

	Scene* GetActiveScene() const { return ActiveScene; }
	Vector2 GetCameraPosition() const { return Camera; }
	void SetCameraPosition(Vector2 Position) { Camera = Position; }
	class DevelopmentScene* GetDevelopmentScene() const;

	ALocalPlayer* GetLocalPlayer() const { return Player; }
	void SetLocalPlayer(ALocalPlayer* InPlayer) { Player = InPlayer; }
	uint64 GetLocalPlayerId() const;

private:
	Scene* ActiveScene = nullptr;
	ESceneType Type = ESceneType::None;
	Vector2 Camera{ 400, 300 };
	ALocalPlayer* Player = nullptr;
};