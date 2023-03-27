// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexEnum.h"
#include "GameFramework/Actor.h"
#include "HexTile.generated.h"

UCLASS()
class UOCTEST_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexTile();

	UPROPERTY(EditAnywhere)
	EHexTypes TileType;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void Init(UMaterialInstance* Material);

    // Marks the hex tile as selected
	void Select(UMaterialInstance* MaterialInstance) const;

	// Unselects the tile
	void Unselect() const;

	void SetCoordinates(int Q, int R, int S);

		// operators
	FORCEINLINE bool operator == (const AHexTile* Other) const
	{
		return Q() == Other->Q() && R() == Other->R() && S() == Other->S();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ShuffleMaterials();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Mesh")
	UStaticMeshComponent* MeshComponent;

	// Coordinates
	FORCEINLINE int Q() const { return Coordinates.X; }
	FORCEINLINE int R() const { return Coordinates.Y; }
	FORCEINLINE int S() const { return Coordinates.Z; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FIntVector Coordinates; // bottom left, top, bottom right

	UPROPERTY()
	UMaterialInstance* DefaultMaterial;

};
