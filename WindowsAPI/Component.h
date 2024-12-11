#pragma once

class AActor;

class Component
{
public:
	Component();
	virtual ~Component();
	virtual void BeginPlay();
	virtual void TickComponent();
	virtual void Render(HDC InDC);

	AActor* GetOwner() const { return Owner; }
	void SetOwner(AActor* NewOwner) { Owner = NewOwner; }

protected:
	AActor* Owner = nullptr;
};

