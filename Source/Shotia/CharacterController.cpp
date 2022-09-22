#include "CharacterController.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShotiaGameMode.h"
#include "ShoqianPlayerState.h"


ACharacterController::ACharacterController()
{
	bReplicates = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Overhead = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	Overhead->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);

	DissolveT = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimeline"));

	AimSpeed = 1;
	
	SprintSpeed = 1;
	
	TurnDirection = ETurningInPlace::ETIP_NotTurning;

	Health = MaxHealth;
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AShoqianPlayerController>(Controller);

	UpdateHUD();
	
	if(HasAuthority())
		OnTakeAnyDamage.AddDynamic(this,&ACharacterController::ReceiveDamage);

	if (Combat == nullptr)
	{
		Combat = NewObject<UCombatComponent>(this,TEXT("Combat"));
	}

	if (Combat)
	{
		Combat->SetIsReplicated(true);
		Combat->Player = this;
	}

	PollInit();
}

void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetSpeed = DefaultSpeed * SprintSpeed * AimSpeed;
	
	SetSpeed(FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, TargetSpeed, DeltaTime, 6.0f));

	RotateInPlace(DeltaTime);

	HideCharacterIfClose();

	CalculateAO_Pitch();
	
	if (PlayerController)
		if (Combat && Combat->EquippedWeapon)
			PlayerController->SetHUDAmmoVisibility(true);
		else
			PlayerController->SetHUDAmmoVisibility(false);

	if (Combat == nullptr)
		Debug("Combat component is NULL!! Restart  the fucking game");
}

void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);

	PlayerInputComponent->BindAction("Interract", IE_Pressed,this,&ACharacterController::InterractKeyPressed);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this,&ACharacterController::CrouchKeyPressed);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACharacterController::ReloadPressed);

	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&ACharacterController::AimButtonPressed);

	PlayerInputComponent->BindAction("Aim",IE_Released,this,&ACharacterController::AimButtonReleased);

	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&ACharacterController::FireButtonPressed);
	
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&ACharacterController::FireButtonReleased);

	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&ACharacterController::SprintButtonPressed);
	
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&ACharacterController::SprintButtonReleased);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ACharacterController::MoveForward);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ACharacterController::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ACharacterController::Turn);

	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&ACharacterController::LookUp);
}

void ACharacterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
		Combat->Player = this;
}

void ACharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACharacterController, OverlappedWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ACharacterController, bDisableGameplay);
	DOREPLIFETIME(ACharacterController, AimSpeed);
	DOREPLIFETIME(ACharacterController, SprintSpeed);
	DOREPLIFETIME(ACharacterController, Health)
}

void ACharacterController::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	
	SimProxyTurn();
	TimeSinceLastMovementReplication = 0;
}

void ACharacterController::Destroyed()
{
	Super::Destroyed();

	if (ParticleComp)
	 ParticleComp->DestroyComponent();

	if (Combat && Combat->EquippedWeapon)
		Combat->EquippedWeapon->Drop();
}

void ACharacterController::OnRep_OverlappingWeapon(AWeaponClass* LastWeapon)
{
	if (OverlappedWeapon)
		OverlappedWeapon->ShowPickWidget(true);

	if (LastWeapon)
		LastWeapon->ShowPickWidget(false);
}

void ACharacterController::OnRep_HealthChanged()
{
	UpdateHUD();
	PlayHitReact();

}

void ACharacterController::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage,0,MaxHealth);

	UpdateHUD();
	PlayHitReact();

	if (Health == 0)
	{

		AShotiaGameMode* gm = Cast<AShotiaGameMode>(GetWorld()->GetAuthGameMode());
		
		if (gm)
		{
			AShoqianPlayerController* PC = Cast<AShoqianPlayerController>(Controller);
			AShoqianPlayerController* EPC = Cast<AShoqianPlayerController>(InstigatedBy);

			if (PC)
			{
			gm->EliminatePlayer(this, PC, EPC);
			}
		}
	}
}

void ACharacterController::Eliminate()
{
	OnDie();
	
	Combat->EquipWeapon(nullptr);

	MulticastEliminate();
	
	if (Combat && Combat->EquippedWeapon)
		Combat->EquippedWeapon->Drop();
	
	GetWorldTimerManager().SetTimer(ElimTimer,this,&ACharacterController::ElimTimerFinished, ElimDelay);
}

void ACharacterController::MulticastEliminate_Implementation()
{
	if (PlayerController)
		PlayerController->SetHUDAmmo(0);

	Overhead->SetVisibility(false);

	IsEliminated = true;
	PlayElimMontage();

	if (IDissolveMaterial)
	{
		IDissolveMaterialDynamic = UMaterialInstanceDynamic::Create(IDissolveMaterial, this);

		GetMesh()->SetMaterial(0, IDissolveMaterialDynamic);
	    IDissolveMaterialDynamic->SetScalarParameterValue(FName("Disolve"),0);

		StartDissolving();
	}

	bDisableGameplay = true;
	GetCharacterMovement()->DisableMovement();
	if (Combat)
		Combat->SetFire(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	if (BotEffect)
	{
		FVector BotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200);
		ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BotEffect, BotSpawnPoint,GetActorRotation());

		if (ElimBotSound)
			UGameplayStatics::SpawnSoundAtLocation(this,ElimBotSound,GetActorLocation());
	}
	
}

void ACharacterController::UpdateDissovleMaterial(float DissolveValue)
{
	IDissolveMaterialDynamic->SetScalarParameterValue(FName("Disolve"), DissolveValue);
}

void ACharacterController::StartDissolving()
{
	DissolveTTrack.BindDynamic(this,&ACharacterController::UpdateDissovleMaterial);

	if (DissolveCurve)
	{
		DissolveT->AddInterpFloat(DissolveCurve,DissolveTTrack);
		DissolveT->Play();
	}
}

void ACharacterController::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	ServerSetSpeed(NewSpeed);
}

void ACharacterController::HideCharacterIfClose()
{
	if (!IsLocallyControlled()) return;

	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < 100.0f)
	{
		GetMesh()->SetVisibility(false);
		if (Combat && Combat->EquippedWeapon)
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (Combat && Combat->EquippedWeapon)
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
	}
}

////////////////////////////////////////////////

void ACharacterController::MoveForward(float Value)
{
	if (bDisableGameplay) return;
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f,Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, Value);
	}
}

void ACharacterController::MoveRight(float Value)
{
	if (bDisableGameplay) return;

	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f,Controller->GetControlRotation().Yaw,0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction,Value);
	}
}

void ACharacterController::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACharacterController::LookUp(float Value)
{

	AddControllerPitchInput(Value);
}

void ACharacterController::Jump()
{

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void ACharacterController::InterractKeyPressed()
{
	if (bDisableGameplay) return;
	if (Combat)
	{
			Combat->EquipWeapon(OverlappedWeapon);
			ServerInterractKeyPressed();
	}
}

void ACharacterController::CrouchKeyPressed()
{
	if (bDisableGameplay) return;
	if (!bIsCrouched)
		Crouch();
	else
		UnCrouch();
}

void ACharacterController::ReloadPressed()
{
	if (bDisableGameplay) return;

	if (Combat)
		Combat->Reload();
}

void ACharacterController::AimButtonPressed()
{
	if (bDisableGameplay) return;

	if (Combat && GetWeaponEquipped())
	{
		Combat->SetAim(true);
		
		AimSpeed = 0.4;
	}

	SprintButtonReleased();
}

void ACharacterController::AimButtonReleased()
{
	if (bDisableGameplay) return;

	if (Combat && GetWeaponEquipped())
		Combat->SetAim(false);

	AimSpeed = 1;
}

void ACharacterController::FireButtonPressed()
{
	if (bDisableGameplay) return;

	if (Combat )
	{
		Combat->SetFire(true);
	}
}

void ACharacterController::FireButtonReleased()
{
	if (bDisableGameplay) return;

	if (Combat)
		Combat->SetFire(false);
}

void ACharacterController::SprintButtonPressed()
{
	SprintSpeed = 1.78;
}

void ACharacterController::SprintButtonReleased()
{
	SprintSpeed = 1;
}

void ACharacterController::ServerInterractKeyPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappedWeapon);
	}
}

void ACharacterController::ServerSetSpeed_Implementation(float NewSpeed)
{
	if(GetCharacterMovement())
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ACharacterController::AimOffset(float DeltaTime)
{
	float Speed = CalculateSpeed();
	bool bIsInAir = GetMovementComponent()->IsFalling();

	if (Speed == 0.f && !bIsInAir)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0, GetBaseAimRotation().Yaw, 0);
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(StartingAimRotation, CurrentAimRotation);
		AO_Yaw = DeltaRotation.Yaw;


		if (TurnDirection == ETurningInPlace::ETIP_NotTurning)
			AO_InterpYaw = AO_Yaw;
		
		bUseControllerRotationYaw = true;
		TurnPlace(DeltaTime);
	}

	if (Speed > 0.f || bIsInAir)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0, GetBaseAimRotation().Yaw, 0);
		AO_Yaw = 0;
		bUseControllerRotationYaw = true;
		TurnDirection = ETurningInPlace::ETIP_NotTurning;

	}

	CalculateAO_Pitch();

	if (GetIsAiming())
	{
		AO_Yaw = AO_Yaw / 1.6;
		AO_Pitch = AO_Pitch / 1.6;
	}
}

void ACharacterController::SimProxyTurn()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	bRotateRootBone = false;

	ProxyLastFrameRotation = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyLastFrameRotation).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurnDirection = ETurningInPlace::ETIP_TurnRight;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurnDirection = ETurningInPlace::ETIP_TurnLeft;
		}
		else
		{
			TurnDirection = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	else
	{
			TurnDirection = ETurningInPlace::ETIP_NotTurning;
	}
}

void ACharacterController::TurnPlace(float DeltaTime)
{
	if (AO_Yaw > 65)
	{
		TurnDirection = ETurningInPlace::ETIP_TurnRight;
	}
	else if (AO_Yaw < -65)
	{
		TurnDirection = ETurningInPlace::ETIP_TurnLeft;
	
	}
	else
	{
		TurnDirection = ETurningInPlace::ETIP_NotTurning;
	}

	if (TurnDirection != ETurningInPlace::ETIP_NotTurning)
	{
		AO_InterpYaw = FMath::FInterpTo(AO_InterpYaw,0.f,DeltaTime,4.f);
		AO_Yaw = AO_InterpYaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurnDirection = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		}
	}
}

void ACharacterController::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90, 0);

		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

float ACharacterController::CalculateSpeed()
{
	
	FVector Velocity = this->GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

void ACharacterController::ElimTimerFinished()
{
	AShotiaGameMode* gm = Cast<AShotiaGameMode>(GetWorld()->GetAuthGameMode());

	if (gm)
	{
		gm->RequestRespawn(this,this->Controller);
		IsEliminated = false;
		
	OnRespawn();

	PlayerController->SetHUDHealth(MaxHealth,MaxHealth);
	UpdateHUD();

	Overhead->SetVisibility(true);

	}
}

void ACharacterController::OnDie()
{
	UpdateHUD();
}

void ACharacterController::OnRespawn()
{
	UpdateHUD();
	Health = MaxHealth;
}


void ACharacterController::SetOverlappedWeapon(AWeaponClass* Weapon)
{
	if(OverlappedWeapon)
		OverlappedWeapon->ShowPickWidget(false);		

	OverlappedWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if(OverlappedWeapon)
		OverlappedWeapon->ShowPickWidget(true);
	}
}

bool ACharacterController::GetWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

void ACharacterController::PlayElimMontage()
{
	//if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* Instance = GetMesh()->GetAnimInstance();

	if (Instance && ElimAnim)
	{
		Instance->Montage_Play(ElimAnim);
		Instance->Montage_JumpToSection("Origin");
	}
}

void ACharacterController::PlayMontage(bool IsAiming)
{
	if (Combat && Combat->EquippedWeapon)
	{
		UAnimInstance* instance = GetMesh()->GetAnimInstance();

		if (instance)
		{
			instance->Montage_Play(FireWeaponAnim);
			instance->Montage_JumpToSection(IsAiming ? FName("Rifle_Hip") : FName("Rifle_Aim"));
		}
	}
}

void ACharacterController::PlayHitReact()
{
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();

	if (Instance && HitReactAnim)
	{
		Instance->Montage_Play(HitReactAnim);
		Instance->Montage_JumpToSection("FromLeft");
	}
}

void ACharacterController::PlayReload()
{
	if (Combat && Combat->EquippedWeapon && ReloadAnim)
	{
		UAnimInstance* instance = GetMesh()->GetAnimInstance();

		if (instance)
		{
			FName Section;

			switch (Combat->EquippedWeapon->GetWeaponType())
			{
			case EWeaponType::EWT_AssultRifle:
				Section = FName("Rifle");
				break;
			case EWeaponType::EWT_RocketLauncher:
				Section = FName("RocketLauncher");
				break;
			case EWeaponType::EWT_Pistol:
				Section = FName("Pistol");
				break;
			case EWeaponType::EWT_Submachine:
				Section = FName("Pistol");
				break;
			case EWeaponType::EWT_Shotgun:
				Section = FName("Shotgun");
				break;
			case EWeaponType::EWT_SniperRifle:
				Section = FName("Sniper");
				break;
			case EWeaponType::EWT_GrenedeLauncher:
				Section = FName("Rifle");
				break;
			default:
				return;
				break;
			}
			instance->Montage_Play(ReloadAnim);
			instance->Montage_JumpToSection(Section);

		}
	}
}

bool ACharacterController::GetIsAiming()
{
	return (Combat && Combat->IsAiming);
}

AWeaponClass* ACharacterController::GetWeapon()
{
	return (Combat ? Combat->GetWeapon() : nullptr);
}

ECombatState ACharacterController::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_Max;
	return Combat->CState;
}

UCombatComponent* ACharacterController::GetCombat()
{
	return Combat;
}

FVector ACharacterController::GetHitTarget()
{
	return Combat->HitPoint;
}

void ACharacterController::PollInit()
{
	if (PlayerController)
	{
		PlayerController->SetHUDKills(0);
		PlayerController->SetHUDDeaths(0);
		PlayerController->SetHUDAmmo(0);
		PlayerController->SetHUDMagAmmo(0);
	}
}

void ACharacterController::RotateInPlace(float DeltaTime)
{
	if (bDisableGameplay) return;

	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
		AimOffset(DeltaTime);
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
			OnRep_ReplicatedMovement();
	}
}

void ACharacterController::UpdateHUD()
{
	PlayerController = PlayerController == nullptr ? Cast<AShoqianPlayerController>(Controller) : PlayerController;

	if (PlayerController)
		PlayerController->SetHUDHealth(Health, MaxHealth);
}
