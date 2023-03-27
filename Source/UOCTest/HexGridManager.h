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

	// World coordinate to Hex
	Hex WorldToHex(const FVector& Location) const;

	// World location to fractional Hex -> used to find Hex
	FractionalHex LocationToFractionalHex(const FVector& Location) const;
    FractionalHex LerpHex(Hex a, Hex b, double t);
    static float PreciseLerp(double a, double b, double t);
    std::vector<Hex> GetHexLine(const Hex& StartHex, const Hex& EndHex);

    // Returns associated blueprint to Hex 
	AHexTile* GetTileByHex(Hex& H);

	// Returns 2D point
	Point HexToWorldPoint(const Hex Tile) const;

	// Returns 3D point
	FVector HexToWorldLocation(Hex Tile) const;
	
	static int Length(const Hex Tile);

	static int Distance(const Hex A, const Hex B);

	// returns a vector of Hexes in a desired Rangee
	std::vector<Hex> GetHexesInRange(Hex StartingHex, int Range) const;
	
	// Returns the number of hexes in a desired Range
	static int GetHexCountForRange(int Range);
	
	void SelectHexes(const std::vector<Hex>& Hexes);
	
	void UnselectHexes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void GenerateGrid();

	// Rounding from fractal coordinates
	Hex HexRound(FractionalHex h) const;
	
	// Arithmetics
	static Hex Add(const Hex A, const Hex B);
	static Hex Subtract(const Hex A, const Hex B);
	static Hex Multiply(const Hex Tile, int Multiplier);
	static Hex Divide(const Hex Tile, int Divisor);

	// Fields
	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	TSubclassOf<AHexTile> HexTile;

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

    // Tile materials
    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* InvalidMaterial;

    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* BlockedMaterial;

    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* DirtMaterial;
    
    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* GrassMaterial;

    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* WaterMaterial;

    UPROPERTY(EditAnywhere, Category = "Hex Grid | Materials")
    UMaterialInstance* SelectedMaterial;
    
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

    std::map<EHexTypes, UMaterialInstance*> Materials;

    std::vector<Hex> SelectedHexes;
};
