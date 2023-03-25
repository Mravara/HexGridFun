// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"

#include "Hex.h"
#include "LineTypes.h"

// Sets default values
AHexGridManager::AHexGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();

	// init vars
	TileWidth = OuterTileSize * 2.f;

	TileHeight = InnerTileSize * 2.f;

	HorizontalTileSpacing = TileWidth * (3.f / 4.f);

	VerticalTileSpacing = TileHeight / 2.f;

	// generate grid
	GenerateGrid();
}

void AHexGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE::Geometry::FLine3d line = UE::Geometry::FLine3d();
	
}

void AHexGridManager::GenerateGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("HorizontalTileSpacing %f"), HorizontalTileSpacing);
	UE_LOG(LogTemp, Warning, TEXT("VerticalTileSpacing %f"), VerticalTileSpacing);
	UE_LOG(LogTemp, Warning, TEXT("TileWidth %f"), TileWidth);
	UE_LOG(LogTemp, Warning, TEXT("TileHeight %f"), TileHeight);
	UE_LOG(LogTemp, Warning, TEXT("OuterTileSize %f"), OuterTileSize);
	UE_LOG(LogTemp, Warning, TEXT("InnerTileSize %f"), InnerTileSize);
	
	for (int q = LeftCount; q <= RightCount; q++)
	{
		const int QOffset = floor(q/2.f);
		for (int r = UpCount - QOffset; r <= DownCount - QOffset; r++)
		{
			// Map.insert(Hex(q, r, -q-r));
			Point SpawnLocation = HexToWorld(Hex(q, r, -q-r));
			AHexTile* Tile = GetWorld()->SpawnActor<AHexTile>(GrassTile, FVector(SpawnLocation.x, SpawnLocation.y, 0.f), FRotator());
			Tile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), q, r, -q-r));
			UE_LOG(LogTemp, Warning, TEXT("spawning hex: %d %d %d"), q, r, -q-r);
			UE_LOG(LogTemp, Warning, TEXT("on location: %f %f"), SpawnLocation.x, SpawnLocation.y);
			UE_LOG(LogTemp, Warning, TEXT("----"));
		}
	}

	for (Hex hex : Map)
	{
		
	}
}

Point AHexGridManager::HexToWorld(const Hex Tile) const
{
	const float Right = Tile.Q * HorizontalTileSpacing; // 150
	const float Up = -VerticalTileSpacing * Tile.Q + Tile.R * -VerticalTileSpacing * 2.f; // 86.6

	// His way
	// Right = OuterTileSize * (3./2 * Tile.Q);
	// Up = -OuterTileSize * (sqrt(3)/2 * Tile.Q + sqrt(3) * Tile.R);

	return Point(Up, Right);
}

Hex AHexGridManager::Add(const Hex A, const Hex B)
{
	return Hex(A.Q + B.Q, A.R + B.R, A.S + B.S);
}

Hex AHexGridManager::Subtract(const Hex A, const Hex B)
{
	return Hex(A.Q - B.Q, A.R - B.R, A.S - B.S);
}

Hex AHexGridManager::Multiply(const Hex Tile, const int Multiplier)
{
	return Hex(Tile.Q * Multiplier, Tile.R * Multiplier, Tile.S * Multiplier);
}

Hex AHexGridManager::Divide(const Hex Tile, const int Divisor)
{
	return Hex(Tile.Q / Divisor, Tile.R / Divisor, Tile.S / Divisor);
}

AHexTile* AHexGridManager::GetHexTile(const FIntVector Coordinates)
{
	if (HexTileMap.Contains(Coordinates))
	{
		return HexTileMap[Coordinates];
	}

	return nullptr;
}

int AHexGridManager::Length(const Hex Tile)
{
	return (
		FMath::Abs(Tile.Q) +
		FMath::Abs(Tile.R) +
		FMath::Abs(Tile.S));
}

int AHexGridManager::Distance(const Hex A, const Hex B)
{
	return Length(Subtract(A, B));
}



