// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Projectile.generated.h"

UCLASS()
class SHOTIA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Events
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION()
		virtual void Destroyed() override;

	//Properties

	//Functions

protected:
	UPROPERTY(EditAnywhere)
		float Damage = 20;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MoveComp;

private:
	//Properties
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere)
		UParticleSystem* Tracer;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
		USoundCue* ImpactSound;

		UParticleSystemComponent* TracerComponent;

	//Functions
};
