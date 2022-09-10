// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponClass.h"
#include "CharacterController.h"
#include "ShoqianPlayerController.h"

// Sets default values
AWeaponClass::AWeaponClass()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(RootComponent);

    SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));	
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	PickupWidget->SetVisibility(false);

	State = UWeaponState::EWS_None;
}

// Called when the game starts or when spawned
void AWeaponClass::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn,ECollisionResponse::ECR_Overlap);

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this,&AWeaponClass::OnSphereEnterOverlap);
	
	AreaSphere->OnComponentEndOverlap.AddDynamic(this,&AWeaponClass::OnSphereLeaveOverlap);
	}
	
}


void AWeaponClass::OnSphereEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
  ACharacterController* CharC = Cast<ACharacterController>(OtherActor);

  if(CharC)
	  CharC->SetOverlappedWeapon(this);
}

void AWeaponClass::OnSphereLeaveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ACharacterController* CharC = Cast<ACharacterController>(OtherActor);

	if (CharC)
		CharC->SetOverlappedWeapon(nullptr);
}



void AWeaponClass::Fire(const FVector& HitPos)
{
	if(FireAnim)
	{
		WeaponMesh->PlayAnimation(FireAnim,false);
	}
	SpendRound();
}

void AWeaponClass::ShowPickWidget(bool bShowWidget)
{
	if (PickupWidget)
		PickupWidget->SetVisibility(bShowWidget);
}

void AWeaponClass::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponClass, State);
	DOREPLIFETIME(AWeaponClass, Ammo);
}

void AWeaponClass::SetWeaponState(UWeaponState NewState)
{
	this->State = NewState;
	this->State = NewState;

	switch (State)
	{
	case UWeaponState::EWS_Equipped:
		ShowPickWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case UWeaponState::EWS_Dropped:
		if (HasAuthority())
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case UWeaponState::EWS_Max:
		break;
	default:
		break;
	}
}

void AWeaponClass::SetHUDAmmo()
{
	Character = Character ? Character : Cast<ACharacterController>(GetOwner());

	if (Character)
		Player = Player ? Player : Cast<AShoqianPlayerController>(Character->Controller);

	if (Player)
	{
		Player->SetHUDAmmo(Ammo);
	}
}

void AWeaponClass::Drop()
{
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld,true);
	
	SetWeaponState(UWeaponState::EWS_Dropped);
	WeaponMesh->DetachFromComponent(Rules);
	SetOwner(nullptr);

	Character = nullptr;
	Player = nullptr;
}

void AWeaponClass::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo - AmmoToAdd,0,MaxAmmo);
	SetHUDAmmo();
}

void AWeaponClass::SpendRound()
{
	Ammo = FMath::Clamp(Ammo-1,0,MaxAmmo-1);

	SetHUDAmmo();
}

void AWeaponClass::OnRep_WeaponState()
{
	switch (State)
	{
	case UWeaponState::EWS_Equipped:
		ShowPickWidget(false);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
	case UWeaponState::EWS_Dropped:
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case UWeaponState::EWS_Max:
		break;
	default:
		break;
	}
}

void AWeaponClass::OnRep_Ammo()
{
	SetHUDAmmo();
}

// Called every frame
void AWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponClass::OnRep_Owner()
{
	Super::OnRep_Owner();

	if (Owner == nullptr)
	{
		Character = nullptr;
		Player = nullptr;
	}
	else
	{
		Character = Character ? Character : Cast<ACharacterController>(GetOwner());

		if (Character)
		{
			Player = Player ? Player : Cast<AShoqianPlayerController>(Character->Controller);
		}
	}
}

