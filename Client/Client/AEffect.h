#pragma once
#include "UObject.h"

class AEffect : public UObject
{
	using Super = UObject;

public:
	AEffect();
	~AEffect() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	void UpdateAnimation() override;

protected:

};

