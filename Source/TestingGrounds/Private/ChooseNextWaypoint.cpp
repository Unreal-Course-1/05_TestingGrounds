// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	/* This works, but it's not taking the name of the Blackboard key from the variable that we
	 * linked in the Blueprint. So, it's better to take it from the variable than hard wiring it
	 * here.
	 *
	 * int32 Index = BlackboardComp->GetValueAsInt(FName{ "Next Waypoint Index" });
	 */
	int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);

	return EBTNodeResult::Succeeded;
}


