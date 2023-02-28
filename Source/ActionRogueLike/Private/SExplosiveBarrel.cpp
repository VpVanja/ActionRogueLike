// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody); // This is the default value for this property but it's good to be explicit about it
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // This is the default value for this property but it's good to be explicit about it
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 500.0f;
	RadialForceComp->ForceStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	
}


void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHitBarrel);
}

void ASExplosiveBarrel::OnHitBarrel(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast <APawn>(OtherActor) == nullptr)
	{
		RadialForceComp->FireImpulse();

		

		UE_LOG(LogTemp, Log, TEXT("OnHit() explosive barrel"));
		UE_LOG(LogTemp, Warning, TEXT(" OTherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at location %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
		
		if(ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect->Template, Hit.ImpactPoint);
		}
		
	}
	
}



