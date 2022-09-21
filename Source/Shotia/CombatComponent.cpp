#include "CombatComponent.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IsAiming = false;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacterController>(GetOwner());

	if(Player)
	DefaultFov = Player->GetFOV();

	if (DefaultFov == -1)
		DefaultFov = 90;

	CurrentFov = DefaultFov;

	if(Player && Player->HasAuthority())
	InitializeMag();
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetHUDCrosshairs(DeltaTime);

	FHitResult result;
	TraceUnderCrosshairs(result);

	HitPoint = result.ImpactPoint;

	InterpFOV(DeltaTime);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, IsAiming);
	DOREPLIFETIME(UCombatComponent, CState);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
}


void UCombatComponent::EquipWeapon(AWeaponClass* Weapon)
{
	if (Player != nullptr && Weapon != nullptr)
	{
		if (EquippedWeapon)
			EquippedWeapon->Drop();

		this->EquippedWeapon = Weapon;
		this->EquippedWeapon->SetWeaponState(UWeaponState::EWS_Equipped);

		if (Weapon->GetMaxAmmo() <= 0)
			Debug("Max ammo 0 olamaz gerizekali")

		const USkeletalMeshSocket* WeaponSocket = Player->GetMesh()->GetSocketByName(FName("RightHandSocket"));

		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(Weapon,Player->GetMesh());
		}

		this->EquippedWeapon->SetOwner(Player);
		this->EquippedWeapon->ShowPickWidget(false);
		
		if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
		{
			CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];	
		}

		Controller = Controller == nullptr ? Cast<AShoqianPlayerController>(Player->Controller) : Controller;

		if (Controller)
		{
			Controller->SetHUDAmmo(EquippedWeapon->Ammo);
			Controller->SetHUDMagAmmo(CarriedAmmo);
		}

		this->Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		this->Player->bUseControllerRotationYaw = true;
		this->Player->bUseControllerRotationYaw = true;

		if (EquippedWeapon->EquipSound)
			UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->EquipSound,EquippedWeapon->GetActorLocation());

		if (EquippedWeapon->IsEmpty())
			Reload();
	}
}

void UCombatComponent::InitializeMag()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssultRifle,StartingARAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher,StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol,StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Submachine,StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun,StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle,StartingSniperAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenedeLauncher,StartingGrenedeLauncher);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Player)
	{
		this->EquippedWeapon->SetWeaponState(UWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* WeaponSocket = Player->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		
		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(EquippedWeapon, Player->GetMesh());
		}

		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		Player->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitLoc)
{
	MulticastFire(TraceHitLoc);
}

void UCombatComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitLoc)
{
	if (EquippedWeapon && Player && CState == ECombatState::ECS_Unoccupied)
	{
		Player->PlayMontage(IsAiming);
		EquippedWeapon->Fire(TraceHitLoc);
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D VPSize;

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(VPSize);

	FVector2D CrosshairLocation(VPSize.X / 2, VPSize.Y / 2);
	FVector CrosshairWordPos;
	FVector CrosshairWordDir;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),CrosshairLocation,CrosshairWordPos, CrosshairWordDir);

	if (Player && bScreenToWorld)
	{
		FVector Start = CrosshairWordPos;
		FVector End = Start + CrosshairWordDir * 8000;

		float DistanceToChar = (Player->GetActorLocation() - Start).Size();
		Start += CrosshairWordDir * (DistanceToChar + 50.0f);

		GetWorld()->LineTraceSingleByChannel(TraceHitResult,Start,End,ECollisionChannel::ECC_Visibility);
	}
}

void UCombatComponent::Reload()
{
	if(CarriedAmmo > 0 && CState != ECombatState::ECS_Reloading)
		ServerReload();
}

void UCombatComponent::ServerReload_Implementation()
{
	if (Player == nullptr && EquippedWeapon) return;

	
	if(Player->HasAuthority())
	CState = ECombatState::ECS_Reloading;

	HandleReload();
}

void UCombatComponent::FinishReload()
{
	CState = ECombatState::ECS_Unoccupied;
	
	UpdateAmmoValues();

	if (bFirePressed)
		Fire();
}

void UCombatComponent::UpdateAmmoValues()
{
	if (EquippedWeapon && Player)
	{
		int32 amount = ReloadAmount();
		if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
		{
			CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= amount;
			CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		}

		Controller = Controller == nullptr ? Cast<AShoqianPlayerController>(Player->Controller) : Controller;

		if (Controller)
			Controller->SetHUDMagAmmo(CarriedAmmo);

		EquippedWeapon->AddAmmo(-amount);
	}
}

void UCombatComponent::HandleReload()
{
	Player->PlayReload();
}

int32 UCombatComponent::ReloadAmount()
{
	try
	{
		
		if (EquippedWeapon != nullptr)
		{
			int32 RoomInMag = EquippedWeapon->GetMaxAmmo() - EquippedWeapon->GetAmmo();
			
			if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
			{
				int32 AmmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
				int32 least = FMath::Min(RoomInMag,AmmountCarried);
				return FMath::Clamp(RoomInMag,0,least);
			}
		}
	}
	catch (const std::exception&)
	{
		Debug("I hate Unreal Engine");
		return 0;
	}

	return 0;
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CState)
	{
	case ECombatState::ECS_Unoccupied:
		if (bFirePressed)
			Fire();
		break;
	case ECombatState::ECS_Reloading:
		HandleReload();
		break;
	case ECombatState::ECS_Max:
		break;
	default:
		break;
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Player == nullptr || Player->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<AShoqianPlayerController>(Player->Controller) : Controller;

	HUD = HUD == nullptr ? Cast<AShoqianHUD>(Controller->GetHUD()) : HUD;

	if (HUD)
	{
		FHUDPackage Package;
		if (EquippedWeapon)
		{
			Package.CrosshairCenter = EquippedWeapon->CrosshairCenter;
			Package.CrosshairTop = EquippedWeapon->CrosshairTop;
			Package.CrosshairLeft = EquippedWeapon->CrosshairLeft;
			Package.CrosshairRight = EquippedWeapon->CrosshairRight;
			Package.CrosshairBottom = EquippedWeapon->CrosshairBottom;
			HUD->SetCrosshairDrawable(true);
		}
		else
		{
			Package.CrosshairCenter = nullptr;
			Package.CrosshairTop = nullptr;
			Package.CrosshairLeft = nullptr;
			Package.CrosshairRight = nullptr;
			Package.CrosshairBottom = nullptr;
			HUD->SetCrosshairDrawable(false);
		}
		
		//Calculate crosshair spread

		//[0, 600] -> [0,1]
		FVector2D WalkSpeedRange(0.f,Player->GetCharacterMovement()->MaxWalkSpeed);
		FVector2D Target(0,1);
		FVector Velocity = Player->GetVelocity();
		Velocity.Z = 0;
	
		CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange,Target,Velocity.Size());

		if (Player->GetMovementComponent()->IsFalling())
		{
			CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor,2.25,DeltaTime,0.25);
		}
		else
		{
			CrosshairAirFactor = FMath::FInterpTo(CrosshairAirFactor,0,DeltaTime,2.25);
		
		}
		if (IsAiming)
			CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor,0.58f,DeltaTime,30.f);
		else
			CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor,0,DeltaTime,30.f);

		if (Player->bIsCrouched)
			CrosshairCrouchFactor = 0.2;
		else
			CrosshairCrouchFactor = 0;

		CrosshairShootFactor = FMath::FInterpTo(CrosshairShootFactor,0,DeltaTime,6.75);

		Package.Spread = 
			0.5f +	
			CrosshairVelocityFactor +
			CrosshairAirFactor +
			CrosshairShootFactor -
			(CrosshairAimFactor + CrosshairCrouchFactor);

		Package.Spread = FMath::Clamp(Package.Spread,0.03,3);

		HUD->SetHUDPackage(Package);
	}
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	Controller = Controller == nullptr ? Cast<AShoqianPlayerController>(Player->Controller) : Controller;

	if (Controller)
		Controller->SetHUDMagAmmo(CarriedAmmo);
}

void UCombatComponent::SetAim(bool Isaiming)
{
	if (Player == nullptr || EquippedWeapon == nullptr) return;
	IsAiming = Isaiming;
	ServerSetAim(Isaiming);

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		Player->ShowSniperScopeWidget(Isaiming);
}

void UCombatComponent::SetFire(bool IsPressed)
{
	bFirePressed = IsPressed;

	if (IsPressed)
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (!CanFire())return;

	ServerFire(HitPoint);
	if (EquippedWeapon)
	{
		CrosshairShootFactor = 10.75f;
	}
	FireTimerStart();
}

void UCombatComponent::FireTimerStart()
{
	if (EquippedWeapon == nullptr || Player == nullptr) return;

	Player->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinish,
		EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinish()
{
	if (EquippedWeapon == nullptr)
		return;

	if (bFirePressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}

	if (EquippedWeapon->IsEmpty())
		Reload();
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;

	if (IsAiming)
	{
		CurrentFov = FMath::FInterpTo(CurrentFov,EquippedWeapon->GetZoomedFOV(),DeltaTime,EquippedWeapon->ZoomInterpSpeed);
	}
	else 
	{
		CurrentFov = FMath::FInterpTo(CurrentFov,DefaultFov,DeltaTime,EquippedWeapon->ZoomInterpSpeed);
	}
	if (Player && Player->GetFOV() != -1)
	{
		Player->SetFOV(CurrentFov);
	}
}

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;
	return !EquippedWeapon->IsEmpty() && bCanFire && CState == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::ServerSetAim_Implementation(bool NewValue)
{
	IsAiming = NewValue;
}
