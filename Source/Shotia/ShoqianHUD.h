// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterOverlay.h"
#include "WarmupWidget.h"
#include "ShoqianHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CrosshairCenter;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CrosshairLeft;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CrosshairRight;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CrosshairTop;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CrosshairBottom;

	float Spread;
};

UCLASS()
class SHOTIA_API AShoqianHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	void AddOverlay();
	
	void AddWarmupWidget();

	UPROPERTY()
	UCharacterOverlay* Overlay;

	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> WarmupWidgetClass;

	UPROPERTY()
	UWarmupWidget* WarmWidget;

private:
	bool CrosshairDrawable;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

	UPROPERTY(EditAnywhere, Category = "Overlays")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Structs", meta = (AllowPrivateAccess = "true"))
	FHUDPackage Pack;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { Pack = Package; }
	FORCEINLINE void SetCrosshairDrawable(bool NewValue) { this->CrosshairDrawable = NewValue; }

};
