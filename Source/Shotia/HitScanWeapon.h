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

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector HitTarget);

	void WeaponTraceHit(const FVector& Start, const FVector& HitTarget, FHitResult& HitResult);
	
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

	UPROPERTY(EditAnywhere,Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere,Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere,Category = "Weapon Scatter")
	bool bUseScatter = false;

private:
};
