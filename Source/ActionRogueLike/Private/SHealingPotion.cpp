// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealingPotion.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASHealingPotion::ASHealingPotion()
{
	MeshComp->SetupAttachment(SphereComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}


void ASHealingPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	
	if (AttributeComp && !AttributeComp->IsMaxHealth()) // check if we are alive and not at max health if not apply heal
	{
		if (ensure(PickupVfx))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupVfx, GetActorLocation());
		}
		if (ensure(PickupSound))
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		
		if (AttributeComp->ApplyHealthChange(this, HealAmount))
		{
			HideAndCoolDownPowerUp();
		}
		
	}
}
