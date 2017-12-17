// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPoolComponent.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() {

	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPoolComponent>(FName("Nav Mesh Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {

	TActorIterator<ANavMeshBoundsVolume> VolumeIterator{ GetWorld() };
	while (VolumeIterator) {
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd) {

	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}


