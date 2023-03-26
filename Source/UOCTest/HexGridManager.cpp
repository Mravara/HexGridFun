// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"

#include "Hex.h"
#include "LineTypes.h"
#include "UOCTestGameMode.h"
#include "Kismet/GameplayStatics.h"

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

	AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->GridManager = this;

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
			Hex hex = Hex(q, r, -q-r);
			Point SpawnLocation = HexToWorldPoint(hex);
			AHexTile* Tile = GetWorld()->SpawnActor<AHexTile>(GrassTile, FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), FRotator());
			Tile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), q, r, -q-r));

			HexTileMap[hex] = Tile;
		}
	}

	for (Hex hex : Map)
	{
		
	}
}

AHexTile* AHexGridManager::GetTileByHex(Hex& H)
{
	if (HexTileMap.count(H))
	{
		return HexTileMap[H];
	}

	return nullptr;
}

Point AHexGridManager::HexToWorldPoint(const Hex Tile) const
{
	const float Right = Tile.Q * HorizontalTileSpacing; // 150
	const float Up = VerticalTileSpacing * Tile.Q + Tile.R * VerticalTileSpacing * 2.f; // 86.6

	// His way
	// Right = OuterTileSize * (3./2 * Tile.Q);
	// Up = -OuterTileSize * (sqrt(3)/2 * Tile.Q + sqrt(3) * Tile.R);

	return Point(Up, Right);
}

FVector AHexGridManager::HexToWorldLocation(const Hex Tile) const
{
	const float Right = Tile.Q * HorizontalTileSpacing; // 150
	const float Up = VerticalTileSpacing * Tile.Q + Tile.R * VerticalTileSpacing * 2.f; // 86.6

	// His way
	// Right = OuterTileSize * (3./2 * Tile.Q);
	// Up = -OuterTileSize * (sqrt(3)/2 * Tile.Q + sqrt(3) * Tile.R);

	return FVector(Up, Right, 0.f);
}

Hex AHexGridManager::WorldToHex(FVector& Location)
{
	// const float Right = Tile.Q * HorizontalTileSpacing; // 150
	// const float Up = -VerticalTileSpacing * Tile.Q + Tile.R * -VerticalTileSpacing * 2.f; // 86.6
	//
	// // His way
	// // Right = OuterTileSize * (3./2 * Tile.Q);
	// // Up = -OuterTileSize * (sqrt(3)/2 * Tile.Q + sqrt(3) * Tile.R);
	//
	// return Point(Up, Right);
	return HexRound(LocationToFractionalHex(Location));
}

FractionalHex AHexGridManager::LocationToFractionalHex(FVector& Location)
{
	Point pt = Point(Location.Y / OuterTileSize, Location.X / OuterTileSize);
	double q = (2.0 / 3.0) * pt.X;
    double r = (-1.0 / 3.0) * pt.X + sqrt(3.0) / 3.0 * pt.Y;
	return FractionalHex(q, r, -q - r);
}

Hex AHexGridManager::HexRound(FractionalHex h)
{
	int q = int(round(h.Q));
	int r = int(round(h.R));
	int s = int(round(h.S));
	double q_diff = abs(q - h.Q);
	double r_diff = abs(r - h.R);
	double s_diff = abs(s - h.S);
	if (q_diff > r_diff && q_diff > s_diff)
	{
		q = -r - s;
	}
	else if (r_diff > s_diff)
	{
		r = -q - s;
	}
	else
	{
		s = -q - r;
	}
	
	return Hex(q, r, s);
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



