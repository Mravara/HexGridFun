// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <unordered_set>

#include "CoreMinimal.h"
#include "Hex.h"
#include "HexTile.h"
#include "GameFramework/Actor.h"
#include "HexGridManager.generated.h"

struct Hex;

// Map storage
namespace std {
	template <> struct hash<Hex> {
		size_t operator()(const Hex& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.Q);
			size_t hr = int_hash(h.R);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

// Point for Grid Layout
struct Point {
	double X, Y;
	Point(double x_, double y_): X(x_), Y(y_) {}
};

// Fraction
struct FractionalHex {
	const double Q, R, S;
	FractionalHex(double q_, double r_, double s_)
	: Q(q_), R(r_), S(s_) {}
};

UCLASS()
class UOCTEST_API AHexGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGridManager();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	Hex WorldToHex(FVector& Location);
	
	FractionalHex LocationToFractionalHex(FVector& Location);

	AHexTile* GetTileByHex(Hex& H);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void GenerateGrid();

	// Rounding from fractal coordinates
	Hex HexRound(FractionalHex h);
	
	// Arithmetics
	static Hex Add(const Hex A, const Hex B);
	static Hex Subtract(const Hex A, const Hex B);
	static Hex Multiply(const Hex Tile, int Multiplier);
	static Hex Divide(const Hex Tile, int Divisor);

	static int Length(const Hex Tile);
	static int Distance(const Hex A, const Hex B);

	Point HexToWorld(const Hex Tile) const;

	// Fields
	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	TSubclassOf<AHexTile> GrassTile;

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	TSubclassOf<AHexTile> DirtTile;

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	TSubclassOf<AHexTile> WaterTile;

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	FIntVector2 GridSize = FIntVector2(10, 10);

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	float OuterTileSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	float InnerTileSize = 86.60254037844386467637f; // sqrt(3) * OuterTileSize / 2

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	FVector TileOffset;

	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	int LeftCount = -5;
	
	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
    int RightCount = 5;
    	
	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	int UpCount = -5;

	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	int DownCount = 5;

	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	bool IsFlatTopLayout = true;

	UPROPERTY()
	float TileWidth;

	UPROPERTY()
	float TileHeight;

	UPROPERTY()
	float HorizontalTileSpacing;

	UPROPERTY()
	float VerticalTileSpacing;

		
	// All directions
	TArray<FIntVector> DirectionVectors = TArray
	{
		FIntVector(1, 0, -1),
		FIntVector(1, -1, 0),
		FIntVector(0, -1, 1),
		FIntVector(-1, 0, 1),
		FIntVector(-1, 1, 0),
		FIntVector(0, 1, -1)
	};
	
	std::unordered_set<Hex> Map;
	
	std::map<Hex, AHexTile*> HexTileMap;
	
};
