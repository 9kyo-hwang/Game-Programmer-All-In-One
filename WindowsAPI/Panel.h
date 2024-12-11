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
	// �ڽ� UI�� �� ���� �ֵ��� ��
	// �г��� ��Ȱ��ȭ�Ǿ� �ִٸ� �ڽ��� UI�� ��� ��Ȱ��ȭ
	vector<UI*> Children;
};
