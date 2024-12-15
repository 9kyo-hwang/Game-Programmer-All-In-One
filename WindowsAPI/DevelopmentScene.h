#pragma once
#include "Scene.h"

class DevelopmentScene final : public Scene
{
	using Super = Scene;

public:
	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update(float DeltaTime) override;  // �ʿ信 ���� FixedUpdate�� �ֵ� ��
	void Render(HDC DeviceContextHandle) override;

	bool CanMoveTo(Vector2Int Dest) const;
	Vector2 CellToWorld(Vector2Int Cell) const;

	class TilemapActor* ActiveTilemapActor = nullptr;
};
