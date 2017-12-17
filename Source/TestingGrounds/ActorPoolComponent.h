// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<AActor*> Pool;

public:	
	// Sets default values for this component's properties
	UActorPoolComponent();

	// Gives an Actor from the pool
	AActor* Checkout();

	// Returns an Actor to the pool
	void Return(AActor* ActorToReturn);

	// For initially populating the pool
	void Add(AActor* ActorToAdd);
	
};
