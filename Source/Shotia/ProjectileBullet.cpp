#include "Shotia/ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());

	if (OwnerChar)
	{
		AController* PC = OwnerChar->Controller;
		if(PC)
			UGameplayStatics::ApplyDamage(OtherActor,Damage,PC,this,UDamageType::StaticClass());
	}


	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
