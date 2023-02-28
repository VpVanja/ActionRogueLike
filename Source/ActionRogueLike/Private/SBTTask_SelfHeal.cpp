// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_SelfHeal.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"

EBTNodeResult::Type USBTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (ASAICharacter*  AIChar = Cast<ASAICharacter>(AIC->GetPawn()))
	{
		USAttributeComponent* AttComp = AIChar->GetAttributeComp();
		if (AttComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		float Health = AttComp->GetHealth();
		float MaxHealth = AttComp->GetMaxHealth();
		AttComp->ApplyHealthChange(AIChar, MaxHealth - Health);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	
}
