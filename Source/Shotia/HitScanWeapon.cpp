#include "Shotia/HitScanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "CharacterController.h"

void AHitScanWeapon::Fire(const FVector& HitPos)
{
	Super::Fire(HitPos);

	const USkeletalMeshSocket* Muzzle = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (Owner == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	if (Muzzle)
	{
		FTransform SocketTransform = Muzzle->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitPos - Start) * 1.25;

		FHitResult hit;

		FVector BeamEnd = End;

		UWorld* world = GetWorld();
		if (world)
		{
			if(MuzzleFlashParticles)
			UGameplayStatics::SpawnEmitterAtLocation(world, MuzzleFlashParticles,SocketTransform);

			if (FireSound)
				UGameplayStatics::PlaySoundAtLocation(this,FireSound,SocketTransform.GetLocation());

			world->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Pawn);

			if (hit.bBlockingHit)
			{
				BeamEnd = hit.ImpactPoint;
				ACharacterController* player = Cast<ACharacterController>(hit.GetActor());
				if (player && InstigatorController)
				{
					UGameplayStatics::ApplyDamage(player, Damage, InstigatorController, this, UDamageType::StaticClass());
				}

				if (ImpactParticles)
					UGameplayStatics::SpawnEmitterAtLocation(world, ImpactParticles, hit.ImpactPoint, hit.ImpactNormal.Rotation());

				if (HitSound)
					UGameplayStatics::SpawnSoundAtLocation(this,HitSound,hit.ImpactPoint);
			}
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(world, BeamParticles, SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"),BeamEnd);
			}
		}
	}
}
