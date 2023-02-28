// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public AProjectileBase
{
	GENERATED_BODY()


public:
	//constructor
	ASTeleportProjectile();

protected:

	virtual void PostInitializeComponents() override;

	FTimerHandle StopProjectileTImerHandle;
	FTimerHandle TeleportTimerHandle;
	FTimerHandle TeleportOnHitTimerHandle;
	UFUNCTION()
	void StopProjectile();

	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* TeleportEffect;
	
	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	//virtual void OnActorHIt(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
	void TeleportInstigator();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};


