// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shotia/ProjectileBullet.h"
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

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

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
		float MinDamageRad = 200.f;
	UPROPERTY(EditDefaultsOnly)
		float MaxDamageRad = 200.f;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* RocketMesh;



};
