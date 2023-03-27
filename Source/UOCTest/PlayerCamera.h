// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hex.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCamera.generated.h"

UCLASS()
class UOCTEST_API APlayerCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// When Left Mouse Button is pressed
	void OnMouseClicked();

	// When Right Mouse Button is pressed
	void OnRightMouseClicked();

	// When Right Mouse Button is released
	void OnRightMouseReleased();

	// When Right Mouse Button is held
	void OnRightMouseHold();

    // When Shift + Right Mouse Button is pressed
    void OnRightMouseModifiedClicked();

    // When Shift + Right Mouse Button is held
    void OnRightMouseModifiedHold();

    // When Shift + Right Mouse Button is released
    void OnRightMouseModifiedReleased();

    // Draw debug line
	void DrawLine(const FColor Color = FColor::White, bool DrawDots = false) const;

    // Used for converting screen to world space coordinates
	FVector GetMouseWorldLocation() const;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category="Player Camera")
	const UInputAction* ClickAction;

	UPROPERTY(EditAnywhere, Category="Player Camera")
	const UInputAction* RightClickAction;

    UPROPERTY(EditAnywhere, Category="Player Camera")
    const UInputAction* RightClickActionModified;

    // Started drag from this hex
	Hex StartHex;

    // Ended drag on this hex
	Hex EndHex;
};
