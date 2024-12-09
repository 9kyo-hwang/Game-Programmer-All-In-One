#pragma once
#include "Singleton.h"

class UIManager : public Singleton<UIManager>
{
public:
	void Initialize();
	void Render(HDC InDC);

private:
	void				RenderBackground(HDC InDC);
	void				RenderWind(HDC InDC);
	void				RenderPower(HDC InDC);
	void				RenderStamina(HDC InDC);
	void				RenderTime(HDC InDC);
	void				RenderAngle(HDC InDC);
	void				RenderWeaponChoice(HDC InDC);
	void				RenderMiniMap(HDC InDC);

public:
	void				SetWindPercent(float windPercent) { WindPercent = windPercent; }
	void				SetPowerPercent(float powerPercent) { PowerPercent = powerPercent; }
	void				SetStaminaPercent(float staminaPercent) { StaminaPercent = staminaPercent; }
	void				SetPlayerAngle(float angle) { PlayerAngle = angle;}
	void				SetBarrelAngle(float angle) { BarrelAngle = angle;}
	void				SetSpecialWeapon(bool specialWeapon) { SpecialWeapon = specialWeapon; }
	void				SetRemainTime(int32 remainTime) { RemainTime = remainTime; }

	float				GetWindPercent() const { return WindPercent; }
	float				GetPowerPercent() const { return PowerPercent; }
	float				GetStaminaPercent() const { return StaminaPercent; }
	float				GetPlayerAngle() const { return PlayerAngle; }
	float				GetBarrelAngle() const { return BarrelAngle; }
	bool				GetSpecialWeapon() const { return SpecialWeapon; }
	int32				GetRemainTime() const { return RemainTime; }

private:
	float				WindPercent = 0; // [-100~100]
	float				PowerPercent = 0;
	float				StaminaPercent = 0;
	float				PlayerAngle = 0;
	float				BarrelAngle = 0;
	bool				SpecialWeapon = false;
	int32				RemainTime = 0;
};

