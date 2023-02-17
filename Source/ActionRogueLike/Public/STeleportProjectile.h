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
	UFUNCTION()
	void StopProjectile();

	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* TeleportEffect;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Teleport();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};


