#include "Shotia/GrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AGrenadeProjectile::AGrenadeProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MoveComp->SetIsReplicated(true);
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bShouldBounce = true;
}

void AGrenadeProjectile::BeginPlay()
{
	AActor::BeginPlay();

	SpawnTrailSystem();
	DestroyTimerStart();

	MoveComp->OnProjectileBounce.AddDynamic(this,&AGrenadeProjectile::OnBounce);
}

void AGrenadeProjectile::Destroyed()
{
	ExplodeDamage(FMath::RandRange(0, 10),InnerDamageRad,OuterDamageRad,1.5);

	Super::Destroyed();
}

void AGrenadeProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if(BounceSound)
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound,GetActorLocation());
}
