// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOTIA_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	friend class ACharacterController;

	void SetInitialSpeeds(float BaseSpeed, float BaseCrouchSpeed);
	void SetInitialJump(float BaseJump);

	void Heal(float HealAmount,float HealingTime);
	void Accelerate(float BuffSpeed, float BuffCrouchSpeed, float Duration);
	void JumpBoost(float BoostPower, float Duration);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		ACharacterController* Character;

private:

	/*
	// Health buff
	*/

	void RaiseHealth(float DeltaTime);

	bool bIsHealing = false;
	float AmountToHealth;
	float HealRate;

	/*
	// Speed buff
	*/

	FTimerHandle SpeedBuffTimer;	
	void ResetSpeeds();

	float InitialSpeed;
	float InitialCrouchSpeed;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAccelerate(float BuffSpeed,float BuffSpeedCrouched);
	
	/*
	// Jump buff
	*/

	FTimerHandle JumpBoostTimer;
	void ResetJump();

	float InitialJumpVelocity;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastJump(float Boost);
};
