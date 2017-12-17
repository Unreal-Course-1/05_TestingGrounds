// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPoolComponent.h"


// Sets default values for this component's properties
UActorPoolComponent::UActorPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Gives an Actor from the pool
AActor* UActorPoolComponent::Checkout() {
	
	if (Pool.Num() <= 0) {
		return nullptr;
	} else {
		return Pool.Pop();
	}
}

// Returns an Actor to the pool
void UActorPoolComponent::Return(AActor* ActorToReturn) {

	Add(ActorToReturn);
	return;
}

// For initially populating the pool
void UActorPoolComponent::Add(AActor* ActorToAdd) {

	Pool.Push(ActorToAdd);
	return;
}

