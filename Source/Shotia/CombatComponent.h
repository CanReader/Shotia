// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShoqianPlayerController.h"
#include "ShoqianHUD.h"
#include "WeaponTypes.h"
#include "CombatState.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOTIA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend class ACharacterController;

	UCombatComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(AWeaponClass* Weapon);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	UFUNCTION(BlueprintCallable)
	void FinishReload();

	void HandleReload();

	UFUNCTION(BlueprintCallable)
    int32 ReloadAmount();

	void UpdateAmmoValues();

protected:
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitLoc);

	void SetHUDCrosshairs(float DeltaTime);

	FVector HitPoint;

private:
	void InitializeMag();

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_CarriedAmmo();

	void SetAim(bool IsAiming);

	void SetFire(bool IsPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool NewValue);
	
	ACharacterController* Player;
	AShoqianPlayerController* Controller;

	AShoqianHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponClass* EquippedWeapon;

	UPROPERTY(Replicated)
	bool IsAiming;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 60;

	//Carries ammo for the current weapon
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CState = ECombatState::ECS_Unoccupied;

	bool bFirePressed;

	bool bCanFire = true;

	float CrosshairVelocityFactor;
	float CrosshairAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootFactor;
	float CrosshairCrouchFactor;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;
	float DefaultFov = 90.f;
	float CurrentFov;

	FTimerHandle FireTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = 0.15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool Automatic = true;

	void FireTimerStart();
	void FireTimerFinish();

	UFUNCTION()
	void OnRep_CombatState();

	void InterpFOV(float DeltaTime);

	bool CanFire();
	
public:
	FORCEINLINE AWeaponClass* GetWeapon() { return EquippedWeapon ? EquippedWeapon : nullptr; }
};
