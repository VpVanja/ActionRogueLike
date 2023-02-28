// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SHealingPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealingPotion : public ASPowerUpBase
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:
	ASHealingPotion();

protected:

	FTimerHandle DisolveTimerHndle;

	FTimerHandle ResetHealTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealAmount = 50.f;
	
};
