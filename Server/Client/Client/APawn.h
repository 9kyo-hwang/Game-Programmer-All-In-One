#pragma once
#include "UObject.h"

class APawn : public UObject
{
	using Super = UObject;

public:
	APawn();
	~APawn() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

	virtual void TakeDamage(APawn* Instigator);

protected:
	void OnTickIdle(float DeltaTime) override {}
	void OnTickMove(float DeltaTime) override {}
	void OnTickAttack(float DeltaTime) override {}
	void UpdateAnimation() override {}

	UAttributeSet& GetAttributeSet() { return AttributeSet; }
	void SetAttributeSet(UAttributeSet NewAttributeSet) { AttributeSet = NewAttributeSet; }

protected:
	UAttributeSet AttributeSet;
};

