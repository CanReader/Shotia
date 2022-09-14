#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssultRifle UMETA(DisplayName = "Assult Weapon"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Weapon"),

	EWT_Max UMETA(DisplayName = "Max")
};