// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = TEXT("TimeToHit");
	HealOrDamageSwitchParamName = TEXT("HealDamageSwitch");
	
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}



void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this) //we only check this so that we don't set the target to ourselves, AI can still get hurt by other AI and set the target to the other AI
		{
				SetTargetActor(InstigatorActor);
		}
		GetMesh()->SetScalarParameterValueOnMaterials(HealOrDamageSwitchParamName, 1.0f);
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
        	{
			// stop BT
			AAIController* AIC = Cast <AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//  ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// set life span
			SetLifeSpan(10.0f);
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        		
        	}
	}
	if (Delta > 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(HealOrDamageSwitchParamName, 0.0f);
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast <AAIController>(GetController());

	if (AIC)
	{
	 AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Purple,4.0f, true);
}