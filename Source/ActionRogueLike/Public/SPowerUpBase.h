// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPowerUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	// Sets default values for this actor's properties
	ASPowerUpBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* PickupVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* PickupSound;
	UPROPERTY()
	FTimerHandle TimerHandle_RespawnPowerUp;
	
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void ShowPowerUp();
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void SetPowerUpState(bool bNewIsActive);
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void HideAndCoolDownPowerUp();


public:	


};
