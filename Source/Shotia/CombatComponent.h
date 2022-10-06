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
#include "GrenadeProjectile.h"
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

	void EquipPrimaryWeapon(AWeaponClass* Weapon);
	void EquipSecondaryWeapon(AWeaponClass* Weapon);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
	UFUNCTION(BlueprintCallable)
	void AddPickupAmmo(EWeaponType Type, int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	UFUNCTION(BlueprintCallable)
	void FinishReload();

	UFUNCTION(BlueprintCallable)
	void FinishThrowing();

	void ThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();
	
	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);
	
	void HandleReload();

	UFUNCTION(BlueprintCallable)
    int32 ReloadAmount();

	void UpdateGrenades();

	void UpdateAmmoValues();

	void UpdateShotgunAmmo();

	void SetFire(bool IsPressed);
	

	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	void AttachItemByHand(AActor* Item,bool bIsLeft);

	void AttachItemToBackpack(AActor* Item);


	//
	//
	//
	//
	//
	//
	//
	//


protected:
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitLoc);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitLoc);

	void SetHUDCrosshairs(float DeltaTime);

	FVector HitPoint;
	

	//
	//
	//
	//
	//
	//
	//
	//


private:
	/*
	//
	// Functions
	//
	*/

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	UFUNCTION()
	void OnRep_CarriedAmmo();

	UFUNCTION()
	void OnRep_Grenades();

	UFUNCTION()
	void OnRep_CombatState();
	
	UFUNCTION(Server, Reliable)
	void ServerSetAim(bool NewValue);
	
	void PlayEquipWeaponSound(AWeaponClass* Weapon);

	void DropEquippedWeapon();

	void UpdateCarriedAmmo(AWeaponClass* Weapon);

	void SetAim(bool IsAiming);
	
	void Fire();

	void InitializeMag();

	void InterpFOV(float DeltaTime);

	/*
	//
	// Properties
	//
	*/

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeaponClass* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeaponClass* SecondaryWeapon;

	UPROPERTY(Replicated)
	bool IsAiming;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 60;
	
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 5;
	
	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 30;
	
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 60;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 6;

	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo = 10;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenedeLauncher = 3;

	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxGrenades = 4;

	//Carries ammo for the current weapon
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CState = ECombatState::ECS_Unoccupied;

	/*
	//
	// Variables
	//
	*/

	ACharacterController* Player;
	AShoqianPlayerController* Controller;

	AShoqianHUD* HUD;

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
	
	void FireTimerStart();
	void FireTimerFinish();
	
	/*
	//
	// Inline functions
	//
	*/

public:
	FORCEINLINE AWeaponClass* GetWeapon() { return EquippedWeapon ? EquippedWeapon : nullptr; }
	FORCEINLINE uint32 GetGrenades() { return Grenades; }
	FORCEINLINE bool CanFire() { return EquippedWeapon == nullptr ? false : !EquippedWeapon->IsEmpty() && bCanFire && CState == ECombatState::ECS_Unoccupied; }
};
