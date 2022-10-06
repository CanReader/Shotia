// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "AmmoPickup.h"
#include "SpeedPickup.h"
#include "JumpPickup.h"
#include "ArmorPickup.h"
#include "HealthPickup.h"
#include "PickupSpawner.generated.h"


UCLASS()
class SHOTIA_API APickupSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawner();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	EPickupType Type = EPickupType::EPT_None;

	UPROPERTY(EditAnywhere)
	EWeaponType AmmoType = EWeaponType::EWT_Max;

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickup>> PickupClasses;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickup>> AmmoClasses;


	void SpawnPickup();

private:

	APickup* SpawnedPickup;

	FTimerHandle SpawnPickupTimer;

	UFUNCTION()
	void SpawnTimerStarted(AActor* DestroyedActor);
	UFUNCTION()
	void SpawnTimerFinished();

	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;
};
