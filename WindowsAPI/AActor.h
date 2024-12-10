#pragma once

class AActor
{
public:
	AActor();
	virtual ~AActor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC InDC);

	Vector2Int GetPosition() const { return Position; }
	void SetPosition(Vector2Int NewPosition) { Position = NewPosition; }

protected:
	// 씬에 배치 가능하므로 좌표 필요
	Vector2Int Position{ 0, 0 };
};

