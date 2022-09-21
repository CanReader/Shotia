#include "Shotia/RocketProjectile.h"
#include "Kismet/GameplayStatics.h"


ARocketProjectile::ARocketProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveComp = nullptr;

	RocketMove = CreateDefaultSubobject<URocketProjectileMovement>(TEXT("RocketMovement"));

	RocketMove->bRotationFollowsVelocity = true;
	RocketMove->SetIsReplicated(true);
	RocketMove->InitialSpeed = 1500.f;
	RocketMove->MaxSpeed = 1500.f;
	RocketMove->ProjectileGravityScale = 0;

}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	SpawnTrailSystem();
}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner()) return;

	ExplodeDamage(MinDamage,MinDamageRad,MaxDamageRad);

	Super::OnHit(HitComp,OtherActor,OtherComp,NormalImpulse,Hit);
}

void ARocketProjectile::Destroyed()
{
	Super::Destroyed();
}
