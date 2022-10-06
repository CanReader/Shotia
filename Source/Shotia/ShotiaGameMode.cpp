#include "Shotia/ShotiaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ShoqianPlayerState.h"
#include "ShotiaGameState.h"

namespace MatchState
{
	const FName CoolDown = FName("Cooldown");
}

AShotiaGameMode::AShotiaGameMode()
{
	//Wait response for passing to InProgress state from WaitingToStart
	this->bDelayedStart = true;
}

void AShotiaGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AShotiaGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		AShoqianPlayerController* pc = Cast<AShoqianPlayerController>(*it);

		if (pc)
		{
			pc->OnMatchStateSet(MatchState);
		}
	}

}

void AShotiaGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Set match states by transition with time
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		//GetTimeSeconds returns the total seconds since the world has been created and same for all clients
		CountDownTime = WarmUpTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		if (CountDownTime <= 0.f)
			StartMatch();
	}
	else if (GetMatchState() == MatchState::InProgress)
	{
		CountDownTime = WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		if (CountDownTime <= 0.f)
			SetMatchState(MatchState::CoolDown);
	}
	else if (GetMatchState() == MatchState::CoolDown)
	{
		CountDownTime = CooldownTime + WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		if (CountDownTime <= 0.f)
			RestartGame();
	}
}

void AShotiaGameMode::EliminatePlayer(ACharacterController* EliminatedPlayer, AShoqianPlayerController* VictimController, AShoqianPlayerController* Attacker)
{
	if (MatchState != MatchState::InProgress)
		return;

	AShoqianPlayerState* AttackerState = Attacker ? Cast<AShoqianPlayerState>(Attacker->PlayerState) : nullptr;
	AShoqianPlayerState* VictimState = VictimController ? Cast<AShoqianPlayerState>(VictimController->PlayerState) : nullptr;

	AShotiaGameState* State = GetGameState<AShotiaGameState>();

	if (AttackerState && AttackerState != VictimState && State)
	{
		AttackerState->IncreaseKillBy(1);
		State->UpdateTopPlayers(AttackerState);
	}

	if (VictimState)
		VictimState->IncreaseDeathBy(1);

	if (EliminatedPlayer)
	{
		if (EliminatedPlayer->GetWeapon() && EliminatedPlayer->GetWeapon()->DefaultWeapon)
			EliminatedPlayer->GetWeapon()->Destroy();

		EliminatedPlayer->Eliminate();
	}
}

void AShotiaGameMode::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();
	}
	
	if (EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this,APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Selection]);
	}
}
