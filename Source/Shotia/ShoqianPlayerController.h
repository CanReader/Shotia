// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShoqianHUD.h"
#include "ShoqianPlayerController.generated.h"

UCLASS()
class SHOTIA_API AShoqianPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDKills(int Kills);
	void SetHUDDeaths(int Deaths);
	void SetHUDAmmo(int32 WeaponAmmo);
	void SetHUDMagAmmo(int32 WeaponAmmo);
	void SetHUDMatchCountDown(float CountDownTime);

	void SetHUDWarmupCountdown(float CountDownTime);

	void SetHUDAmmoVisibility(bool Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnMatchStateSet(FName NewMatchState);

	void OnMatchHasStarted();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void PollInit();

	virtual float GetServerTime();
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float StartingTime);

	/*
	Sync time between client and server
	*/

	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // Difference between client and server time

	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void SetHUDTime();

private:
	AShoqianHUD* HUD;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	uint32 CountDownInt = 0;

	UPROPERTY()
	UCharacterOverlay* Overlay;

	bool bInitializeCharacterOverlay;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDeath;
	

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();



};
