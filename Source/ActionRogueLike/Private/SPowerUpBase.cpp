// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASPowerUpBase::ASPowerUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName(TEXT("PowerUp"));

	RespawnTime = 5.f;
	
}


void ASPowerUpBase::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ASPowerUpBase::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);//Setting collistion on all the components
	RootComponent->SetVisibility(bNewIsActive, true);//Hide root and all children
}

void ASPowerUpBase::HideAndCoolDownPowerUp()
{
	SetPowerUpState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnPowerUp, this, &ASPowerUpBase::ShowPowerUp, RespawnTime, false);
}


void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	//logic in child classes
}
