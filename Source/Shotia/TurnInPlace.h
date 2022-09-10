#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8 
{
 ETIP_TurnLeft UMETA(DisplayName = "Turning Left"),
 ETIP_TurnRight UMETA(DisplayName = "Turning Right"),
 ETIP_NotTurning UMETA(DisplayName = "Not Turning"),

 ETIP_TurnMax UMETA(DisplayName = "Defult Max")
};