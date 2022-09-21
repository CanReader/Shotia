#include "Shotia/Shotgun.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "CharacterController.h"

void AShotgun::Fire(const FVector& HitPos)
{
	AWeaponClass::Fire(HitPos);
	const USkeletalMeshSocket* Muzzle = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (Owner == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	if (Muzzle)
	{
		FTransform SocketTransform = Muzzle->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = FVector::ZeroVector;

		TMap<ACharacterController*, uint32> HitMap;
		for (uint32 i = 0; i < PelletNumber; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start,HitPos,FireHit);

			//If hit to a player
			ACharacterController* player = Cast<ACharacterController>(FireHit.GetActor());
			if (player && InstigatorController)
			{
				if (HitMap.Contains(player))
				{
					HitMap[player]++;
				}
				else
				{
					HitMap.Emplace(player,1);
				}
			}

			if (ImpactParticles)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());

			if (HitSound)
				UGameplayStatics::SpawnSoundAtLocation(this, HitSound, FireHit.ImpactPoint, FRotator::ZeroRotator, 0.5f,FMath::FRandRange(-0.5f,0.5f));
		}

		for (auto HitPair : HitMap)
		{
			if (InstigatorController)
			{
				if (HitPair.Key && InstigatorController)
					UGameplayStatics::ApplyDamage(HitPair.Key, Damage * HitPair.Value, InstigatorController, this, UDamageType::StaticClass());
			}
		}
	}
}