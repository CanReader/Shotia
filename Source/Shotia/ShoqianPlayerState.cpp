#include "ShoqianPlayerState.h"

void AShoqianPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShoqianPlayerState, DeathCount);
}

void AShoqianPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;
		
		if (Player)
		{
			Player->SetHUDKills(Score);
		}

	}
}

void AShoqianPlayerState::IncreaseKillBy(int Amount)
{
	SetScore(Score + Amount);

	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;

		if (Player)
		{
			Player->SetHUDKills(Score);
		}

	}
}

void AShoqianPlayerState::OnRep_DeathCount()
{
	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		if (Character->Controller == nullptr)
			return;

		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;

		if (Player)
		{
			Player->SetHUDDeaths(DeathCount);
		}

	}
}

void AShoqianPlayerState::IncreaseDeathBy(int Amount)
{
	DeathCount += Amount;

	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;

		if (Player)
		{
			Player->SetHUDDeaths(DeathCount);
		}

	}
}