// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotia/ShoqianHUD.h"
#include "CharacterController.h"
#include "GameFramework/PlayerController.h"

void AShoqianHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AShoqianHUD::AddOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass)
	{
		Overlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);

		Overlay->AddToViewport();
	}
}

void AShoqianHUD::AddWarmupWidget()
{
	APlayerController* pc = GetOwningPlayerController();

	if (pc && WarmupWidgetClass && WarmWidget == nullptr)
	{
		WarmWidget = CreateWidget<UWarmupWidget>(pc,WarmupWidgetClass);
		WarmWidget->AddToViewport();
	}
}

void AShoqianHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	if (CrosshairDrawable)	
	{
	  GEngine->GameViewport->GetViewportSize(ViewportSize);
	  const FVector2D ViewportCenter(ViewportSize.X / 2, ViewportSize.Y / 2);
	  
	  float SpreadScaled = CrosshairSpreadMax * Pack.Spread;

	  if (Pack.CrosshairCenter != nullptr)
	  	DrawCrosshair(Pack.CrosshairCenter, ViewportCenter,FVector2D(0,0));
	  if (Pack.CrosshairTop != nullptr)
	  	DrawCrosshair(Pack.CrosshairTop, ViewportCenter, FVector2D(0,-SpreadScaled));
	  if (Pack.CrosshairLeft != nullptr)
	  	DrawCrosshair(Pack.CrosshairLeft, ViewportCenter, FVector2D(-SpreadScaled,0));
	  if (Pack.CrosshairRight != nullptr)
	  	DrawCrosshair(Pack.CrosshairRight, ViewportCenter, FVector2D(SpreadScaled, 0));
	  if (Pack.CrosshairBottom != nullptr)
	  	DrawCrosshair(Pack.CrosshairBottom, ViewportCenter,FVector2D(0,SpreadScaled));
	}

}

void AShoqianHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D DrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X, 
		                      ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);
	
	DrawTexture(Texture,
		DrawPoint.X,
		DrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White);
}
