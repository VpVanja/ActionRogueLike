// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class ASTeleportProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	
	
public:
	// Sets default values for this character's properties
	ASCharacter();

	

protected:
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	 UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	 USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;



	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAbility;


	UPROPERTY()
	FVector ProjectileDestination;

	UPROPERTY()
	bool DidTimerFire = true;

	UPROPERTY()
	AActor* SpawnedProjectile;

	UPROPERTY()
	AActor* SpawnedTeleporter;
	
	

	UFUNCTION()
	void CameraTrace();
	UFUNCTION()
	FTransform GetProjectileTransform();
	UFUNCTION()
	void DashAbility();
	
	
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	UFUNCTION()
	void PrimaryAttack();
	UFUNCTION()
	void PrimaryAttack_TimeElapsed();
	UFUNCTION()
	void PrimaryInteract();
	UFUNCTION()
	void BlackHoleAbility();
	UFUNCTION()
	void BlackHoleAbility_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
		

};



