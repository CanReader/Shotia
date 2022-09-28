#include "Shotia/Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponTypes.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	OverlapSphere->SetSphereRadius(250.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	Mesh->SetupAttachment(OverlapSphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this,&APickup::OnSphereOverlapEnter);

	if(HasAuthority())
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this,&APickup::OnSphereOverlapLeave);


	Mesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	Mesh->MarkRenderStateDirty();
	Mesh->SetRenderCustomDepth(true);
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::Destroyed()
{
	Super::Destroyed();

	if (PickupSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation());

		
}

void APickup::OnSphereOverlapEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug("Anan enter");
}

void APickup::OnSphereOverlapLeave(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex)
{
	Debug("Anan leave");
}

