#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterController.h"
#include "NiagaraFunctionLibrary.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer,CollisionBox,FName(),GetActorLocation(),GetActorRotation(),EAttachLocation::KeepWorldPosition);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this,&AProjectile::OnHit);
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorLocation());

	if (ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
}

void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
}

void AProjectile::ExplodeDamage(float MinDamage, float InnerRadius, float OuterRadius, float DamageFalloff)
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
				InnerRadius,
				OuterRadius,
				DamageFalloff,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				Player
			);

	}
}

void AProjectile::DestroyTimerStart()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AProjectile::DestroyTimerFinished,
		DestroyTime);
}

void AProjectile::DestroyTimerFinished()
{
	Destroy();
}

