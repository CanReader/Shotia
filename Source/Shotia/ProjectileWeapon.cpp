// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotia/ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitPos)
{
	Super::Fire(HitPos);

	APawn* InstPawn = Cast<APawn>(GetOwner());

	const USkeletalMeshSocket* MuzzleFlash = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if (MuzzleFlash)
	{
		auto SocketTransform = MuzzleFlash->GetSocketTransform(GetWeaponMesh());

		auto ToTarget = HitPos - SocketTransform.GetLocation();
		FRotator SocketRotator = ToTarget.Rotation();

		if (ProjectileClass && InstPawn)
		{
			UWorld* world = GetWorld();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstPawn;

			if (world)
			{
				try
				{
				world->SpawnActor<AProjectile>(ProjectileClass,SocketTransform.GetLocation(), SocketRotator, SpawnParams);
				}
				catch (const std::exception&)
				{
					GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,"Failed to cast! Bad programming");
					return;
				}
			}
		}
	}
}

