// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponTypes.h"
#include "Sound/SoundCue.h"
#include "WeaponClass.generated.h"

UENUM(BlueprintType)
enum class UWeaponState : uint8
{
	EWS_None UMETA(DisplayName = "Default"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Max UMETA(DisplayName = "DefaultMax"),
};

UCLASS()
class SHOTIA_API AWeaponClass : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponClass();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnRep_Owner() override;

	UFUNCTION()
	void ShowPickWidget(bool bShowWidget);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//Overlap events

    UFUNCTION()
    virtual void OnSphereEnterOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor* OtherActor,
								 UPrimitiveComponent* OtherComponent,
								 int32 OtherBodyIndex,
								 bool bFromSweep,
								 const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereLeaveOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);

	//Properties
	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairBottom;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
	int32 MagAmmo;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmo = 30;

	UPROPERTY(EditAnywhere)
	USoundCue* EquipSound;

	//Functions
	void SetWeaponState(UWeaponState NewState);
	
	void SetHUDAmmo();

	void Drop();

	void AddAmmo(int32 AmmoToAdd);

	virtual void Fire(const FVector& HitPos);

	//Inline functions
	FORCEINLINE EWeaponType GetWeaponType()const { return WeaponType; }
	FORCEINLINE USphereComponent* GetCollider() { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh ? WeaponMesh : nullptr; };
	FORCEINLINE float GetZoomedFOV() { return ZoomedFOV; }
	FORCEINLINE bool IsEmpty() { return Ammo == 0; }
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	//FORCEINLINE int32 AmmoCap() const { return MagAmmo; }

private:
	//Variables
	UPROPERTY()
	 class AShoqianPlayerController* Player;
	UPROPERTY()
	 class ACharacterController* Character;

	//Properties
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

    UPROPERTY(VisibleAnywhere, Category = "Weapon anatomy")
	USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, Category = "Weapon anatomy")
    USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState)
	UWeaponState State;

    UPROPERTY(VisibleAnywhere)
	UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere)
	UAnimationAsset* FireAnim;

	//Functions

	UFUNCTION()
	void SpendRound();

	UFUNCTION()
	void OnRep_WeaponState();

	UFUNCTION()
	void OnRep_Ammo();	
};
