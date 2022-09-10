// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterController.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = Character)
		ACharacterController* Player;
private:

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool IsinAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool IsRunning;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool IsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		ETurningInPlace TurnDirection;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool IsEquipped;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool IsAimimg;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		AWeaponClass* Weapon;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool IsEliminated;

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bUseFabrik;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Bones, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = Networking, meta = (AllowPrivateAccess = "true"))
		bool LocallyControlled;

	UPROPERTY(BlueprintReadOnly, Category = Networking, meta = (AllowPrivateAccess = "true"))
		bool bRotateRootBone;

	FRotator CharacterRotation;
	FRotator LastFrameCharacterRotation;
	FRotator DeltaRotation;
};
