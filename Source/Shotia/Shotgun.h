#pragma once

#include "CoreMinimal.h"
#include "Shotia/HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitPos) override;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	uint32 PelletNumber = 10;
	
};
