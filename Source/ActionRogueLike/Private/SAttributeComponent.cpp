// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"




// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;

	HealthMax = 100.0f;
}




bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth; // if we have 0 health and we try to apply -10 health, ActualDelta will be 0 while delta will be -10
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta); 
	return ActualDelta != 0.0f;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::IsMaxHealth() const
{
	return Health == HealthMax;
}

bool USAttributeComponent::IsHealthLow() const
{
	return Health <= HealthMax * 0.4f;
}

float USAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return GetAttributes(Actor)->IsAlive();
	}
	return false;
}
