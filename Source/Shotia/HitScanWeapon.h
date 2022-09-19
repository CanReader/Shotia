#pragma once

#include "CoreMinimal.h"
#include "Shotia/WeaponClass.h"
#include "HitScanWeapon.generated.h"

UCLASS()
class SHOTIA_API AHitScanWeapon : public AWeaponClass
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitPos) override;

private:
	UPROPERTY(EditAnywhere)
		float Damage;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
		UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlashParticles;

	UPROPERTY(EditAnywhere)
		USoundCue* FireSound;

	UPROPERTY(EditAnywhere)
		USoundCue* HitSound;

};
