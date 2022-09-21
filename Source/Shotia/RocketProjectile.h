// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shotia/ProjectileBullet.h"
#include "RocketProjectileMovement.h"
#include "RocketProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API ARocketProjectile : public AProjectileBullet
{
	GENERATED_BODY()
public:
	ARocketProjectile();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		URocketProjectileMovement* RocketMove;

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	virtual void Destroyed() override;

private:
	UPROPERTY(EditDefaultsOnly)
		float MinDamage = 10.f;
	UPROPERTY(EditDefaultsOnly)
		float MinDamageRad = 150.f;
	UPROPERTY(EditDefaultsOnly)
		float MaxDamageRad = 300.f;

};
