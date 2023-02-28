// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "SCharacter.h"

ASTeleportProjectile::ASTeleportProjectile()
{	
		MovementComp->InitialSpeed = 6000.0f;
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnActorHit);
}

void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(StopProjectileTImerHandle,this, &ASTeleportProjectile::StopProjectile, 0.2f, false);
}



void ASTeleportProjectile::StopProjectile()
{
	GetWorldTimerManager().ClearTimer(StopProjectileTImerHandle);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, GetActorLocation(), GetActorRotation());
	
	MovementComp->StopMovementImmediately();
	
	EffectComp->DeactivateSystem();
	
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ASTeleportProjectile::TeleportInstigator, 0.2f, false);
	
}


void ASTeleportProjectile::Explode_Implementation()
{
	//Super::Explode_Implementation(); //we are overriding the the Explode cemopletely so we dont need to call the super
	
	GetWorldTimerManager().ClearTimer(StopProjectileTImerHandle);
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVfx, GetActorLocation(), GetActorRotation());
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(TeleportOnHitTimerHandle, this, &ASTeleportProjectile::TeleportInstigator, 0.2f, false);
	
}


inline void ASTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
        	Destroy();
	}
	
}

