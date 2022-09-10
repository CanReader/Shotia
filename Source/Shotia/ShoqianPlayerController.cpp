#include "Shotia/ShoqianPlayerController.h"
#include "CharacterController.h"
#include "ShotiaGameMode.h"
#include "Kismet/GameplayStatics.h"

void AShoqianPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;
	
	if (HUD != nullptr)
	{
		const float HealthPercent = Health / MaxHealth;
	
		if (HUD->Overlay != nullptr && HUD->Overlay->HealthBar != nullptr)
		{
			HUD->Overlay->HealthBar->SetPercent(HealthPercent);
			
			HUD->Overlay->HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth))));
		}
		else
		{
			bInitializeCharacterOverlay = true;
			this->HUDHealth = Health;
			this->HUDMaxHealth = MaxHealth;
		}
	}
}

void AShoqianPlayerController::SetHUDKills(int Kills)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != nullptr && HUD->Overlay != nullptr && HUD->Overlay->Kills != nullptr)
	{
		FString str("Kills: ");
		str.AppendInt(Kills);
		HUD->Overlay->Kills->SetText(FText::FromString(str));
	}
}

void AShoqianPlayerController::SetHUDDeaths(int Deaths)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != nullptr && HUD->Overlay != nullptr && HUD->Overlay->Deaths != nullptr)
	{
		FString str("Deaths: ");
		str.AppendInt(Deaths);
		HUD->Overlay->Deaths->SetText(FText::FromString(str));
	}
}

void AShoqianPlayerController::SetHUDAmmo(int32 WeaponAmmo)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != NULL && HUD->Overlay != nullptr && HUD->Overlay->AmmoAmount != nullptr)
	{
		HUD->Overlay->AmmoAmount->SetText(FText::FromString(FString::FromInt(WeaponAmmo)));
	}
}

void AShoqianPlayerController::SetHUDMagAmmo(int32 WeaponAmmo)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != NULL && HUD->Overlay != nullptr && HUD->Overlay->MagazineAmount != nullptr)
		HUD->Overlay->MagazineAmount->SetText(FText::FromString(FString::FromInt(WeaponAmmo)));
}

void AShoqianPlayerController::SetHUDMatchCountDown(float CountDownTime)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != nullptr && HUD->Overlay != nullptr && HUD->Overlay->MatchCountText != nullptr)
	{
		int32 Min = FMath::FloorToInt(CountDownTime/60);
		int32 Secs = CountDownTime - Min * 60;

		FString str = FString::Printf(TEXT("%02d:%02d"),Min,Secs);
		HUD->Overlay->MatchCountText->SetText(FText::FromString(str));
	}
}

void AShoqianPlayerController::SetHUDWarmupCountdown(float CountDownTime)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != nullptr && HUD->WarmWidget != nullptr && HUD->WarmWidget->WarmupTime != nullptr)
	{
		int32 Min = FMath::FloorToInt(CountDownTime / 60);
		int32 Secs = CountDownTime - Min * 60;

		FString str = FString::Printf(TEXT("%02d:%02d"), Min, Secs);
		HUD->WarmWidget->WarmupTime->SetText(FText::FromString(str));
	}
}

void AShoqianPlayerController::SetHUDAmmoVisibility(bool Value)
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD != NULL && HUD->Overlay != nullptr && HUD->Overlay->AmmoAmount != nullptr)
		if(Value)
		HUD->Overlay->AmmoBox->SetVisibility(ESlateVisibility::Visible);
		else
		HUD->Overlay->AmmoBox->SetVisibility(ESlateVisibility::Hidden);
}

void AShoqianPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShoqianPlayerController, MatchState);

}

void AShoqianPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACharacterController* Shoqian = Cast<ACharacterController>(InPawn);

	if (Shoqian != nullptr)
	{
		SetHUDHealth(Shoqian->GetMaxHealth(), Shoqian->GetMaxHealth());
	}
}

void AShoqianPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ServerCheckMatchState();

	HUD = Cast<AShoqianHUD>(GetHUD());

	ServerCheckMatchState();
}

void AShoqianPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();

	TimeSyncRunningTime += DeltaTime;

	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}

	PollInit();
}

void AShoqianPlayerController::PollInit()
{
	if (Overlay == nullptr)
	{
		if (HUD && HUD->Overlay)
		{
			this->Overlay = HUD->Overlay;
			
			if (Overlay)
			{
				SetHUDHealth(HUDHealth,HUDMaxHealth);
				SetHUDKills(HUDScore);
				SetHUDDeaths(HUDDeath);
			}
		}
	}
}

float AShoqianPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AShoqianPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());

	}
}

void AShoqianPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);

	if (HUD && MatchState == "WaitingToStart")
		HUD->AddWarmupWidget();
}

void AShoqianPlayerController::ServerCheckMatchState_Implementation()
{
	AShotiaGameMode* gm = Cast<AShotiaGameMode>(UGameplayStatics::GetGameMode(this));

	if (gm)
	{
		WarmupTime = gm->WarmUpTime;
		MatchTime = gm->MatchTime;
		LevelStartingTime = gm->LevelStartingTime;
		MatchState = gm->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, LevelStartingTime);
	}
}

void AShoqianPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();

	ClientReportServerTime(TimeOfClientRequest,ServerTimeOfReceipt);
}

void AShoqianPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;

	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5 * RoundTripTime);

	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AShoqianPlayerController::SetHUDTime()
{
	float TimeLeft = 0;
	if (MatchState == "WaitingToStart")
		TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == "InProgress")
		TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;

	uint32 SecsLeft = FMath::CeilToInt(TimeLeft);

	if (CountDownInt != SecsLeft)
	{
		if (MatchState == "WaitingToStart")
		{
			SetHUDWarmupCountdown(SecsLeft);
		}
		else if (MatchState == "InProgress")
		{
			SetHUDMatchCountDown(SecsLeft);
		}
	}
	
	CountDownInt = SecsLeft;
}

void AShoqianPlayerController::OnRep_MatchState()
{
	if (MatchState == "InProgress")
	{
		OnMatchHasStarted();
	}
}

void AShoqianPlayerController::OnMatchStateSet(FName NewMatchState)
{
	MatchState = NewMatchState;

	if (MatchState == "InProgress")
	{
		OnMatchHasStarted();
	}

}

void AShoqianPlayerController::OnMatchHasStarted()
{
	HUD = HUD == nullptr ? Cast<AShoqianHUD>(GetHUD()) : HUD;

	if (HUD)
	{
		HUD->AddOverlay();

		if (HUD->WarmWidget)
			HUD->WarmWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
