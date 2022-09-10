// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponClass.h"
#include "ProjectileBullet.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOTIA_API AProjectileWeapon : public AWeaponClass
{	
	GENERATED_BODY()

public:
	//Events

	//Properties

	//Functions
	virtual void Fire(const FVector& HitPos) override;

private:
	//Properties

	//Functions
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBullet> ProjectileClass;
	
};
