// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shotia/Pickup.h"
#include "JumpPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API AJumpPickup : public APickup
{
	GENERATED_BODY()
public:
	AJumpPickup();

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
		float BoostPower = 2500.f;
	UPROPERTY(EditAnywhere)
		float JumpBoostTime = 10.f;
};
