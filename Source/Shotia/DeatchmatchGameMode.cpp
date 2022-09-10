// Fill out your copyright notice in the Description page of Project Settings.


#include "DeatchmatchGameMode.h"

ADeatchmatchGameMode::ADeatchmatchGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}
