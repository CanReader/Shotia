#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterController.h"

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

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MoveComp->bRotationFollowsVelocity = true;
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

