// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolComponent.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector{ 0, -2000, 0 };
	MaxExtent = FVector{ 4000, 2000, 0 };

	NavigationBoundsOffset = FVector{ 2000, 0, 0 };
}

void ATile::SetPool(UActorPoolComponent* Pool) {

	this->Pool = Pool;
	//UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(this->Pool->GetName()));
	PositionNavMeshBoundsVolume(Pool);
}

void ATile::PositionNavMeshBoundsVolume(UActorPoolComponent * Pool) {
	NavMeshBoundsVolume = Pool->Checkout();
	if (!NavMeshBoundsVolume) {
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in Pool"), *(this->GetName()));
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale) {

	int32 NumberToSpawn{ FMath::RandRange(MinSpawn, MaxSpawn) };
	for (size_t i = 0; i < NumberToSpawn; i++) {

		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool Found{ FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale) };
		if (Found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale) {

	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius) {
	
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
	
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {

	FBox Bounds{ MinExtent, MaxExtent };
	const int32 MAX_ATTEMPTS{ 100 };
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint{ FMath::RandPointInBox(Bounds) };
		// CandidatePoint here is at Local Space
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition) {
	AActor* Spawned{ GetWorld()->SpawnActor<AActor>(ToSpawn) };
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0.f, SpawnPosition.Rotation, 0.f));
		Spawned->SetActorScale3D(FVector{ SpawnPosition.Scale });
	}
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition) {

	FRotator Rotation{ 0.f, SpawnPosition.Rotation, 0.f };

	APawn* Spawned{ GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation) };
	if (Spawned) {
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay() {
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (Pool && NavMeshBoundsVolume) {
		Pool->Return(NavMeshBoundsVolume);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {

	FHitResult HitResult;
	/*
	 * Remember Location comes in Local Space,
	 * it has to be transformed to Global Space for SweepSingleByChannel()
	 */
	FVector GlobalLocation{ ActorToWorld().TransformPosition(Location) };
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	return !HasHit;
}