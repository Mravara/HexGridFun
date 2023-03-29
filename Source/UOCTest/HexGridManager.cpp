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

    // Populate materials map
    Materials = decltype(Materials)
    {
        { EHexTypes::Invalid, InvalidMaterial },
        { EHexTypes::Blocked, BlockedMaterial },
        { EHexTypes::Dirt, DirtMaterial },
        { EHexTypes::Grass, GrassMaterial },
        { EHexTypes::Water, WaterMaterial },
    };

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

    // Generate grid and add the HexTileMap
	for (int q = LeftCount; q <= RightCount; q++)
	{
		const int QOffset = floor(q/2.f);
		for (int r = UpCount - QOffset; r <= DownCount - QOffset; r++)
		{
		    // Create hex
			Hex hex = Hex(q, r, -q-r, EHexTypes::Grass);
		    
		    // Add neighbors
	        for (int i = 0; i < DirectionVectors.Num(); ++i)
	        {
	            FIntVector Direction = DirectionVectors[i];
		        Hex Neighbor = Hex(hex.Q + Direction.X, hex.R + Direction.Y, hex.S + Direction.Z);
		        if (HexTileMap.count(Neighbor))
		        {
		            hex.Neighbors[i] = Neighbor;
		        }
		    }

		    // Get world location and rotation
			Point SpawnLocation = HexToWorldPoint(hex);
			float YawRotation = IsFlatTopLayout ? 30.f : 0.f;

		    // Instantiate blueprint on location
		    AHexTile* Tile = GetWorld()->SpawnActor<AHexTile>(HexTile, FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), FRotator(0.f, YawRotation, 0.f));
			Tile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d_%d"), q, r, -q-r));
		    Tile->Init(Materials[EHexTypes::Grass]);

		    // Save to map for future use
			HexTileMap[hex] = Tile;
		}
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

Hex AHexGridManager::WorldToHex(const FVector& Location) const
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

FractionalHex AHexGridManager::LocationToFractionalHex(const FVector& Location) const
{
	Point pt = Point(Location.Y / OuterTileSize, Location.X / OuterTileSize);
	double q = (2.0 / 3.0) * pt.X;
    double r = (-1.0 / 3.0) * pt.X + sqrt(3.0) / 3.0 * pt.Y;
	return FractionalHex(q, r, -q - r);
}

FractionalHex AHexGridManager::LerpHex(Hex a, Hex b, double t) {
    return FractionalHex(PreciseLerp(a.Q, b.Q, t),
                         PreciseLerp(a.R, b.R, t),
                         PreciseLerp(a.S, b.S, t));
}

float AHexGridManager::PreciseLerp(double a, double b, double t)
{
    return a * (1-t) + b * t;
    /* better for floating point precision than
       a + (b - a) * t, which is what I usually write */
}

std::vector<Hex> AHexGridManager::GetHexLine(const Hex& StartHex, const Hex& EndHex)
{
    const int HexDistance = Distance(StartHex, EndHex);
    
    std::vector<Hex> Results = {};
    double Step = 1.0 / FMath::Max(HexDistance, 1);

    for (int i = 0; i <= HexDistance; i++)
    {
        Results.push_back(HexRound(LerpHex(StartHex, EndHex, Step * i)));
    }

    return Results;
}

std::vector<Hex> AHexGridManager::GetShortestPath(Hex Start, Hex End)
{
    std::vector<Hex> Path;
    
}

Hex AHexGridManager::HexRound(const FractionalHex h) const
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
		int(FMath::Abs(Tile.Q) +
		FMath::Abs(Tile.R) +
		FMath::Abs(Tile.S)) / 2);
}

int AHexGridManager::Distance(const Hex A, const Hex B)
{
	return Length(Subtract(A, B));
}

std::vector<Hex> AHexGridManager::GetHexesInRange(const Hex StartingHex, const int Range) const
{
	// declare vector
	std::vector<Hex> Result;
	Result.reserve(GetHexCountForRange(Range));

	for (int q = -Range; q <= Range; q++)
	{
		const int r1 = std::max(-Range, -q - Range);
		const int r2 = std::min(Range, -q + Range);
		for (int r = r1; r <= r2; r++)
		{
			Result.push_back(Add(StartingHex, Hex(q, r)));
		}
	}

	return Result;
}

int AHexGridManager::GetHexCountForRange(const int Range)
{
	return 3 * Range * (Range + 1);
}

void AHexGridManager::SelectHexes(const std::vector<Hex>& Hexes)
{
    for (Hex Hex : Hexes)
    {
        const AHexTile* Tile = GetTileByHex(Hex);
        if (Tile)
        {
            Tile->Select(SelectedMaterial);
            SelectedHexes.push_back(Hex);
        }
    }
}

void AHexGridManager::UnselectHexes()
{
    for (Hex Hex : SelectedHexes)
    {
        const AHexTile* Tile = GetTileByHex(Hex);
        if (Tile)
        {
            Tile->Unselect();
        }
    }

    SelectedHexes.clear();
}

int AHexGridManager::GetHexCost(const Hex& Tile)
{
    if (HexTileCostMap.count(Tile.HexType))
    {
        return HexTileCostMap[Tile.HexType];
    }

    return 1000;
}
