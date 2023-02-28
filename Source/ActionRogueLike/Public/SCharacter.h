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
class USAttributeComponent;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* CastVfx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HealOrDamageSwitchParamName;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAbility;
	FTimerHandle TimerHandle_DashAbility;


	UPROPERTY()
	FVector ProjectileDestination;

	UPROPERTY()
	bool DidTimerFire = true;

	UPROPERTY()
	AActor* SpawnedProjectile;

	UPROPERTY()
	AActor* SpawnedTeleporter;
	
	

	UFUNCTION()
	FVector CameraTrace();
	UFUNCTION()
	FTransform GetProjectileTransformFromTraceEnd(FVector Destination);
	UFUNCTION()
	void DashAbility();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	
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
	UFUNCTION()
	void DashAbility_TimeElapsed();

	UFUNCTION()
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

};



