#include "ShoqianPlayerState.h"

void AShoqianPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShoqianPlayerState, DeathCount);
}

void AShoqianPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	try
	{
	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		if(Character && Character->Controller)
		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;
		
		if (Player)
		{
			Player->SetHUDKills(Score);
		}
	}
	}
	catch (const std::exception&)
	{
		return;
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
		try
		{
		if (Character->Controller == nullptr)
			return;

		if (Character != nullptr)
			if (Player != nullptr)
				Player = Cast<AShoqianPlayerController>(Character->PlayerController);

		if (Player)
		{
			Player->SetHUDDeaths(DeathCount);
		}
		}
		catch (const std::exception&)
		{
			return;
		}
	}
}

void AShoqianPlayerState::IncreaseDeathBy(int Amount)
{
	Character = Character == nullptr ? Cast<ACharacterController>(GetPawn()) : Character;

	if (Character)
	{
		DeathCount += Amount;
		Player = Player == nullptr ? Cast<AShoqianPlayerController>(Character->Controller) : Player;

		if (Player)
		{
			Player->SetHUDDeaths(DeathCount);
		}

	}
}