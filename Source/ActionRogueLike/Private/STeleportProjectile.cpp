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

	TeleportEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("TeleportEffect"));
	
}




void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(StopProjectileTImerHandle,this, &ASTeleportProjectile::StopProjectile, 0.2f, false);
}




void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTeleportProjectile::StopProjectile()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, GetActorLocation(), GetActorRotation());
	MovementComp->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ASTeleportProjectile::Teleport, 0.2f, false);
	
}


void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnHit);
}



void ASTeleportProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(StopProjectileTImerHandle);
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, GetActorLocation(), GetActorRotation());
	MovementComp->StopMovementImmediately();
	GetInstigator()->SetActorLocation(GetActorLocation());
	
	
	Destroy();
}

inline void ASTeleportProjectile::Teleport()
{
	GetInstigator()->SetActorLocation(GetActorLocation());
	Destroy();
}