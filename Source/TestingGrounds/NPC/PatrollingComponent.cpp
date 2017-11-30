// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingComponent.h"

TArray<ATargetPoint*> UPatrollingComponent::GetPatrolPoints() const {
	return PatrolRoute;
}
