#pragma once
#include "UI.h"

class Panel : public UI
{
	using Super = UI;

public:
	Panel();
	~Panel() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC DeviceContextHandle) override;

	void AddChild(UI* Child);
	bool RemoveChild(UI* Child);

private:
	// 자식 UI를 다 물고 있도록 함
	// 패널이 비활성화되어 있다면 자식의 UI들 모두 비활성화
	vector<UI*> Children;
};
