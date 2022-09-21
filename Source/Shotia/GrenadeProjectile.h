#pragma once

#include "CoreMinimal.h"
#include "Shotia/Projectile.h"
#include "GrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API AGrenadeProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AGrenadeProjectile();
	void BeginPlay() override;
	void Destroyed() override;
	
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UPROPERTY(EditAnywhere)
	float InnerDamageRad = 200;
	
	UPROPERTY(EditAnywhere)
	float OuterDamageRad = 600;

private:
	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;

protected:

};
