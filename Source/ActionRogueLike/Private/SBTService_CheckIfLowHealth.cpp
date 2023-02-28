// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckIfLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckIfLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//check if health is low

	
	
	 AAIController* AIC = OwnerComp.GetAIOwner();
	 if (AIC)
	 {
	 	if (ASAICharacter*  AIChar = Cast<ASAICharacter>(AIC->GetPawn()))
	 	{
			bool bIsHealthLow = AIChar->GetAttributeComp()->IsHealthLow();
	 		AIC->GetBlackboardComponent()->SetValueAsBool("IsHealthLow", bIsHealthLow);
		}
	
	 }
	
	
	
}
