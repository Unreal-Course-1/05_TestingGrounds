// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
/* For getting the Points from the Actor (the extended class):
 * #include "PatrollingGuard.h"	// TODO Remove coupling
 */
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Engine/TargetPoint.h"
#include "PatrollingComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// TODO try using references(&) instead of pointers(*)

	// Get Patrol Points
	ATP_ThirdPersonCharacter* Patrol{ Cast<ATP_ThirdPersonCharacter>(OwnerComp.GetAIOwner()->GetPawn()) };

	/* For getting the Points from the Actor (the extended class):
	 *
	 * TArray<ATargetPoint*> PatrolPoints{ Patrol->PatrolRouteCPP };	// TODO Protect against empty patrol routes
	 */
	UPatrollingComponent* PatrollingComponent{ Patrol->FindComponentByClass<UPatrollingComponent>() };

	if (!ensure(PatrollingComponent)) {
		return EBTNodeResult::Failed;
	}
	TArray<ATargetPoint*> PatrolPoints{ PatrollingComponent->GetPatrolPoints() };
	if (PatrolPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("%s missing patrol points"), *Patrol->GetName());
	}
	// Set Next Waypoint
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	/* This works, but it's not taking the name of the Blackboard key from the variable that we
	 * linked in the Blueprint. So, it's better to take it from the variable than hard wiring it
	 * here.
	 *
	 * int32 Index = BlackboardComp->GetValueAsInt(FName{ "Next Waypoint Index" });
	 */
	int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle index
	int32 NewIndex{ ++Index % PatrolPoints.Num() };
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);

	return EBTNodeResult::Succeeded;
}


