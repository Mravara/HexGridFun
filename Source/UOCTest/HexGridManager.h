// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <queue>
#include <unordered_set>

#include "CoreMinimal.h"
#include "Hex.h"
#include "HexTile.h"
#include "GameFramework/Actor.h"
#include "HexGridManager.generated.h"

struct Hex;

// Point for Grid Layout
struct Point
{
	double X, Y;
	Point(double x_, double y_): X(x_), Y(y_) {}
};

// Fraction
struct FractionalHex
{
	const double Q, R, S;
	FractionalHex(double q_, double r_, double s_)
	: Q(q_), R(r_), S(s_) {}
};


// combines two hashes
inline size_t hexHashCombine(int h1, int h2)
{
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}

inline size_t hexToHash(const Hex& h)
{
    return hexHashCombine(hexHashCombine(h.Q, h.R), h.S);
}

namespace std
{
    template<> struct hash<Hex>
    {
        size_t operator()(const Hex& h) const noexcept
        {
            return hexToHash(h);
        }
    };
}

struct PathfindingInfo
{    
    int GetTotalCost() const { return ToStartCost + ToEndCost; }
    
    int GetToStartCost() { return ToStartCost; }
    void SetToStartCost(int Cost) { ToStartCost = Cost; }

    int GetToEndCost() { return ToEndCost; }
    void SetToEndCost(int Cost) { ToEndCost = Cost; }

    void SetConnection(Hex connection) { Connection = connection; }
    Hex GetConnection() { return Connection; }
    
private:
    Hex Connection;
    int ToStartCost = 0;
    int ToEndCost = 0;
    int TotalCost = 0;
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

    // Lerp hex
    FractionalHex LerpHex(Hex a, Hex b, double t);

    // Better precision lerp
    static float PreciseLerp(double a, double b, double t);

    // Get line in hexes
    std::vector<Hex> GetHexLine(const Hex& StartHex, const Hex& EndHex);

    // Get path in hexes
    std::vector<Hex> GetShortestPath(const Hex& Start, const Hex& End);

    // Returns associated blueprint to Hex 
	AHexTile* GetTileByHex(Hex& H);

	// Returns 2D point
	Point HexToWorldPoint(const Hex Tile) const;

	// Returns 3D point
	FVector HexToWorldLocation(Hex Tile) const;
	
	static int Length(const Hex Tile);
    static int ManhattanDistance(const Hex& A, const Hex& B);

    static int Distance(const Hex& A, const Hex& B);

	// returns a vector of Hexes in a desired Rangee
	std::vector<Hex> GetHexesInRange(Hex StartingHex, int Range) const;
	
	// Returns the number of hexes in a desired Range
	static int GetHexCountForRange(int Range);
	
	void SelectHexes(const std::vector<Hex>& Hexes);
	
	void UnselectHexes();

    // Calculate costs
    float GetFromStartCost(const Hex& Start, const Hex& Current);
    float GetToEndCost(const Hex& Current, const Hex& End);
    float GetTotalCost(const Hex& Start, const Hex& Current, const Hex& End);

    // Old cost calculation
    float GetHexCost(const Hex& Current, const Hex& Next, const Hex& Last, const Hex& Start, const Hex& End);

    // Return Material of type
    UMaterialInstance* GetMaterial(EHexTypes Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void GenerateGrid();

    // Calculate and return neighbors
    std::vector<Hex> GetNeighbors(const Hex& H);

    // Get direction between two hexes
    Hex GetHexDirection(const Hex& From, const Hex& To);
    FVector GetVectorDirection(const Hex& From, const Hex& To);

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
	int LeftCount = -30;
	
	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
    int RightCount = 30;
	
	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	int UpCount = -30;

	UPROPERTY(EditAnywhere, Category = "Hex Grid | Number of tiles")
	int DownCount = 30;

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
	TArray<Hex> DirectionVectors = TArray
	{
		Hex(1, 0, -1),
		Hex(1, -1, 0),
		Hex(0, -1, 1),
		Hex(-1, 0, 1),
		Hex(-1, 1, 0),
		Hex(0, 1, -1)
	};

    std::map<EHexTypes, float> HexTileCostMap = {
        {EHexTypes::Dirt, 1},
        {EHexTypes::Grass, 3},
        {EHexTypes::Water, 5},
    };
    
	std::map<Hex, AHexTile*> HexTileMap;

    std::map<EHexTypes, UMaterialInstance*> Materials;

    std::vector<Hex> SelectedHexes;
};

struct PriorityQueue
{
    typedef std::pair<float, Hex> HexPrioPair;
    
    std::priority_queue<HexPrioPair, std::vector<HexPrioPair>, std::greater<HexPrioPair>> elements;

    inline bool empty() const
    {
        return elements.empty();
    }

    inline void put(Hex hex, float priority)
    {
        elements.emplace(priority, hex);
    }

    Hex get()
    {
        Hex best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};