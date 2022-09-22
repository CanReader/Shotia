#pragma once

#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssultRifle UMETA(DisplayName = "Assult Weapon"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Weapon"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_Submachine UMETA(DisplayName = "SMG"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper"),
	EWT_GrenedeLauncher UMETA(DisplayName = "Granede Launcher"),

	EWT_Max UMETA(DisplayName = "Max")
};