// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterController.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pickup.generated.h"

UCLASS()
class SHOTIA_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* PickupEffectComp;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* PickupEffect;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* OverlapSphere;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USoundCue* PickupSound;
};
