// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	//Continuous timer to spawn more bots
	//Actual amout of bots and if it is allowed to spawn determined by logic later in the chain...
	GetWorldTimerManager().SetTimer(ThimerHandle_SpawnBits, this, &ASGameModeBase::SpawnBotTimeElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimeElapsed()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
		UE_LOG(LogTemp, Warning, TEXT("Found %i alive bots"), NrOfAliveBots);
		float MaxBotCount = 10.0f;
		
		if (DifficultyCurve)
		{
			MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);	
		}
		if (NrOfAliveBots >= MaxBotCount)
		{
			UE_LOG(LogTemp, Log, TEXT("Max number of bots reached, not spawning more bots"));
			return;
		}
	}
	
	 UEnvQueryInstanceBlueprintWrapper* QueryInstance =	UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	 if (ensure(QueryInstance))
	 {
		 QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	 }
	 
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Query Failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 32, FColor::Blue, false, 60.0f);
	}

}