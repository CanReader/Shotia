#include "Shotia/PickupSpawner.h"

APickupSpawner::APickupSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickupSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnTimerStarted((AActor*)nullptr);
}

void APickupSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupSpawner::SpawnPickup()
{
	int32 PickupClassCount = PickupClasses.Num();

	if (PickupClassCount > 0)
	{
		if (Type != EPickupType::EPT_None)
		{
			switch (Type)
			{
			case EPickupType::EPT_Ammo:
				if (AmmoClasses.Num() > 0)
				{
					if (AmmoType != EWeaponType::EWT_Max)
					{
						SpawnedPickup = GetWorld()->SpawnActor<APickup>(AmmoClasses[(int32)Type], GetActorTransform());
					}
					else
					{
						int32 Selection = FMath::RandRange(0, PickupClasses.Num() - 1);
						SpawnedPickup = GetWorld()->SpawnActor<APickup>(AmmoClasses[Selection],GetActorTransform());
					}
				}
				break;
			case EPickupType::EPT_Speed:
				SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[0], GetActorTransform());
				break;
			case EPickupType::EPT_Jump:
				SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[1], GetActorTransform());
				break;
			case EPickupType::EPT_Armor:
				SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[2], GetActorTransform());
				break;
			case EPickupType::EPT_Health:
				SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[3], GetActorTransform());
				break;
			default:
				break;
			}
		}
		else
		{
			int32 Selection = FMath::RandRange(0,PickupClassCount-1);
			SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection],GetActorTransform());
		}

	}
		if (SpawnedPickup)
			SpawnedPickup->OnDestroyed.AddDynamic(this,&APickupSpawner::SpawnTimerStarted);
}
void APickupSpawner::SpawnTimerStarted(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);

	GetWorldTimerManager().SetTimer(SpawnPickupTimer,this,&APickupSpawner::SpawnTimerFinished,SpawnTime);
}

void APickupSpawner::SpawnTimerFinished()
{
	if(HasAuthority())
	SpawnPickup();
}
