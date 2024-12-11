#pragma once
#include "Panel.h"

class TestPanel : public Panel
{
	using Super = Panel;
	using ThisClass = TestPanel;

public:
	TestPanel();
	~TestPanel() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC DeviceContextHandle) override;

	void OnClickStartButton();
	void OnClickEditButton();
	void OnClickExitButton();

	int32 Count = 0;
};

