#include "OverheadWidget.h"

TEnumAsByte<ENetRole> UOverheadWidget::GetRole(APawn* p)
{
	return p->GetLocalRole();
}

FString UOverheadWidget::GetSteamName(APawn* OwnPawn)
{
	return FString("Name");
}

void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel,InWorld);
}

void UOverheadWidget::SetRoleText(APawn* OwnPawn)
{
	ENetRole LocalRole = OwnPawn->GetLocalRole();
	FString Role;

	switch (LocalRole)
	{
	case ROLE_None:
		Role = FString("No role!");
		break;
	case ROLE_SimulatedProxy:
		Role = FString("Simulate Proxy");
		break;
	case ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ROLE_Authority:
		Role = FString("Authority");
		break;
	case ROLE_MAX:
		break;
	default:
		break;
	}

	FString LocalRoleString = FString::Printf(TEXT("Local Role = %s"), *Role);
	
	if (RoleBlock)
		RoleBlock->SetText(FText::FromString(LocalRoleString));
}

void UOverheadWidget::SetNameText(FString Name)
{
	PlayerName->SetText(FText::FromString("SteamName"));
}
