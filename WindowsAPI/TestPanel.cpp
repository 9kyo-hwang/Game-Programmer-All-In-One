#include "pch.h"
#include "TestPanel.h"

#include "Button.h"
#include "ResourceManager.h"

TestPanel::TestPanel()
{
	{
		Button* Start = new Button();
		//Start->SetSprite(ResourceManager::Get()->GetSprite(L"Start"), EButtonState::Default);
		Start->Position = { 400, 200 };
		Start->Size = { 650, 300 };
		AddChild(Start);
	}
	{
		Button* StartOnOff = new Button();
		StartOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Start_Off"), EButtonState::Default);
		StartOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Start_On"), EButtonState::Clicked);
		StartOnOff->Position = { 200, 200 };

		StartOnOff->AddOnClickDelegate(this, &ThisClass::OnClickStartButton);
		AddChild(StartOnOff);
	}
	{
		Button* EditOnOff = new Button();
		EditOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Edit_Off"), EButtonState::Default);
		EditOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Edit_On"), EButtonState::Clicked);
		EditOnOff->Position = { 400, 200 };

		EditOnOff->AddOnClickDelegate(this, &ThisClass::OnClickEditButton);
		AddChild(EditOnOff);
	}
	{
		Button* ExitOnOff = new Button();
		ExitOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Exit_Off"), EButtonState::Default);
		ExitOnOff->SetSprite(ResourceManager::Get()->GetSprite(L"Exit_On"), EButtonState::Clicked);
		ExitOnOff->Position = { 600, 200 };

		ExitOnOff->AddOnClickDelegate(this, &ThisClass::OnClickExitButton);
		AddChild(ExitOnOff);
	}
}

TestPanel::~TestPanel()
{
}

void TestPanel::BeginPlay()
{
	Super::BeginPlay();
}

void TestPanel::Tick()
{
	Super::Tick();
}

void TestPanel::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	wstring Text = std::format(L"Count: {0}", Count);
	Utils::DrawTextW(DeviceContextHandle, { 100, 0 }, Text);
}

void TestPanel::OnClickStartButton()
{
	Count++;
}

void TestPanel::OnClickEditButton()
{
	Count--;
}

void TestPanel::OnClickExitButton()
{
	Count = 0;
}
