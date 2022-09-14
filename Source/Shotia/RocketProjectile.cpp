#include "Shotia/RocketProjectile.h"
#include "Kismet/GameplayStatics.h"

ARocketProjectile::ARocketProjectile()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* pPawn = GetInstigator();

	if (pPawn)
	{
		auto Player = pPawn->GetController();
		
		if (Player)
			UGameplayStatics::ApplyRadialDamageWithFalloff(
			this,
				Damage,
				MinDamage,
				GetActorLocation(),
				MinDamageRad,
				MaxDamageRad,
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				Player
				);
			
	}

	Super::OnHit(HitComp,OtherActor,OtherComp,NormalImpulse,Hit);
}

void ARocketProjectile::Destroyed()
{
	Super::Destroyed();
}