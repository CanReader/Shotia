// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "OverheadWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponClass.h"
#include "Net/UnrealNetwork.h"
#include "ShoqianPlayerController.h"
#include "TurnInPlace.h"
#include "Components/TimelineComponent.h"
#include "Sound/SoundCue.h"
#include "CombatState.h"

#include "CharacterController.generated.h"

#define Debug(msg) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,msg);

UCLASS()
class SHOTIA_API ACharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	//Overrides
	ACharacterController();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void Destroyed() override;

private:
	//Functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	virtual void Jump() override;
	void InterractKeyPressed();
	void CrouchKeyPressed();
	void ReloadPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void SprintButtonPressed();
	void SprintButtonReleased();

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeaponClass* LastWeapon);
	
	UFUNCTION()
	void OnRep_HealthChanged();

	UFUNCTION(Server, Reliable)
	void ServerInterractKeyPressed();

	UFUNCTION(Server, Reliable)
	void ServerSetSpeed(float NewSpeed);

	UFUNCTION()
	void UpdateDissovleMaterial(float DissolveValue);

	void StartDissolving();

	void SetSpeed(float NewSpeed);

	void HideCharacterIfClose();
	
	void AimOffset(float DeltaTime);

	void SimProxyTurn();

	void TurnPlace(float DeltaTime);

	void CalculateAO_Pitch();

	float CalculateSpeed();

	void ElimTimerFinished();

	virtual void OnDie();

	virtual void OnRespawn();

	//Properties

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* Overhead;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		AWeaponClass* OverlappedWeapon;

	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats", ReplicatedUsing = OnRep_HealthChanged)
		float Health;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* FireWeaponAnim;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactAnim;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* ElimAnim;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UParticleSystem* BotEffect;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	USoundCue* ElimBotSound;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTimelineComponent* DissolveT;

	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UMaterialInstanceDynamic* IDissolveMaterialDynamic;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* IDissolveMaterial;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UCurveFloat* DissolveCurve;

	//Variables

	class AShoqianPlayerState* PState;

	FOnTimelineFloat DissolveTTrack;

	const float DefaultSpeed = 300;

	const float TurnThreshold = .75f;

	FRotator ProxyLastFrameRotation;
	FRotator ProxyRotation;
	float ProxyYaw;

	FRotator StartingAimRotation;
	float AO_Yaw;
	float AO_InterpYaw;
	float AO_Pitch;

	float TimeSinceLastMovementReplication;

	bool bRotateRootBone;

	ETurningInPlace TurnDirection;
	
	bool IsEliminated;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

public:
	//Properties
	UPROPERTY(Replicated, Category = "Character Movement", EditAnywhere, BlueprintReadWrite)
	float AimSpeed;

	UPROPERTY(Replicated, Category = "Character Movement", EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UCombatComponent* Combat;

	//Functions
	void SetOverlappedWeapon(AWeaponClass* Weapon);

	UFUNCTION(BlueprintCallable)
	bool GetWeaponEquipped();

	void PlayElimMontage();
	void PlayMontage(bool IsAiming);
	void PlayHitReact();
	void PlayReload();

	void OnCurveUpdate(float NewValue);

	FVector GetHitTarget();

	void PollInit();

	UFUNCTION()
		void UpdateHUD();

	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage,const UDamageType* DamageType,AController* InstigatedBy, AActor* DamageCauser);

		void Eliminate();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastEliminate();

	//Variables

	AShoqianPlayerController* PlayerController;

	//Inline functions
	bool GetIsAiming();
	AWeaponClass* GetWeapon();
	ECombatState GetCombatState()const;
	FORCEINLINE float GetAOYaw() { return AO_Yaw; }
	FORCEINLINE float GetAOPitch() { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurnDirection() { return TurnDirection; }
	FORCEINLINE float GetFOV() { return FollowCamera ? FollowCamera->FieldOfView : -1; }
	FORCEINLINE bool GetIsEliminated() { return IsEliminated; }
	FORCEINLINE void SetFOV(float NewValue) { FollowCamera->SetFieldOfView(NewValue); }
	FORCEINLINE bool GetProxyRotateRootBone() { return bRotateRootBone; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
};
