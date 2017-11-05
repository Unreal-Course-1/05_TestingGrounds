// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrollingComponent.generated.h"

class ATargetPoint;

/**
* A "route card" to help AI choose their next waypoint
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Patrolling")
	TArray<ATargetPoint*> PatrolRoute;

public:

	TArray<ATargetPoint*> GetPatrolPoints() const;
};
