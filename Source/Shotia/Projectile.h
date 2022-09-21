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
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
		float Damage = 20;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* TrailSystem;

	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* TrailComponent;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 0.3f;

	FTimerHandle DestroyTimer;

	UFUNCTION()
	void SpawnTrailSystem();
	
	UFUNCTION()
	void ExplodeDamage(float MinDamage, float InnerRadius, float OuterRadius, float DamageFalloff = 1.f);

	void DestroyTimerStart();
	UFUNCTION()
	void DestroyTimerFinished();

	UPROPERTY(EditAnywhere)
	USoundCue* ImpactSound;

private:
	//Properties
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere)
		UParticleSystem* Tracer;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticles;


		UParticleSystemComponent* TracerComponent;

	//Functions
};
