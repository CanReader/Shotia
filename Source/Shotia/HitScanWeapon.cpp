#include "Shotia/HitScanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
		WeaponTraceHit(Start,HitPos,hit);
		
		if(MuzzleFlashParticles)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashParticles, SocketTransform);
		
		if (FireSound)
				UGameplayStatics::PlaySoundAtLocation(this,FireSound,SocketTransform.GetLocation());
		
			ACharacterController* player = Cast<ACharacterController>(hit.GetActor());
			if (player && InstigatorController)
				UGameplayStatics::ApplyDamage(player, Damage, InstigatorController, this, UDamageType::StaticClass());
			
			if (ImpactParticles)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			
			if (HitSound)
				UGameplayStatics::SpawnSoundAtLocation(this,HitSound,hit.ImpactPoint);

	}
}

FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0,SphereRadius);
	FVector EndLoc = SphereCenter + RandVec;
	FVector ToEnd = EndLoc - TraceStart;

	return EndLoc;
}

void AHitScanWeapon::WeaponTraceHit(const FVector& Start, const FVector& HitTarget, FHitResult& HitResult)
{
	UWorld* world = GetWorld();

	if (world)
	{
		FVector End = bUseScatter ? TraceEndWithScatter(Start,HitTarget) : Start + (HitTarget - Start) * 1.25;

		world->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn);
	
		FVector BeamEnd = End;
	
		if (HitResult.bBlockingHit)
		{
			BeamEnd = HitResult.ImpactPoint;
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(world, BeamParticles, Start,FRotator::ZeroRotator);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

