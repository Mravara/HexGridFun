// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"

// Sets default values
AHexTile::AHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AHexTile::SetCoordinates(const int X, const int Y, const int Z)
{
	Coordinates = FIntVector(X, Y, Z);
}

// Called when the game starts or when spawned
void AHexTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

