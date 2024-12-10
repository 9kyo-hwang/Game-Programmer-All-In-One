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
	// ���� ��ġ �����ϹǷ� ��ǥ �ʿ�
	Vector2Int Position{ 0, 0 };
};

