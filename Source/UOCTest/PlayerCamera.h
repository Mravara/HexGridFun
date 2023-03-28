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
	void DrawLine(const FColor Color = FColor::Blue, bool DrawDots = false) const;

    // Used for converting screen to world space coordinates
	FVector GetMouseWorldLocation() const;
    
    FVector GetMouseWorldLocation(FVector2D& MousePosition) const;

    // Used for camera movement
    void MoveCamera(const FInputActionValue& Value);

    // Used for camera rotation
    void RotateCamera(const FInputActionValue& Value);

    // Used for camera zoom in and out
    void ZoomCamera(const FInputActionValue& Value);

    // Called every tick while updating camera zoom
    void UpdateCameraZoom(const float DeltaTime);

    // Called when drag camera is started
    void StartDragCamera(const FInputActionValue& Value);

    // Called each tick while drag camera is active
    void UpdateCameraDrag(const FInputActionValue& Value);
    
    // Called when camera drag ends
    void EndDragCamera(const FInputActionValue& Value);

    // Started drag from this hex
    Hex StartHex;

    // Ended drag on this hex
    Hex EndHex;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
	const UInputAction* ClickAction;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
	const UInputAction* RightClickAction;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
    const UInputAction* RightClickActionModified;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
    const UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
    const UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, Category="Camera | Actions")
    const UInputAction* ZoomAction;

    UPROPERTY(EditAnywhere, Category="Camera | Actions")
    const UInputAction* DragAction;
    
    // Zoom vars
    UPROPERTY()
    float StartingZoom;

    UPROPERTY()
    float TargetZoom;

    UPROPERTY()
    float CurrentZoomDuration;

    UPROPERTY()
    bool IsUpdatingCameraZoom;

    UPROPERTY()
    FVector2D StartMouseScreenPosition;

    UPROPERTY()
    FVector2D OldScreenMousePosition;

    UPROPERTY()
    FVector OldWorldMousePosition;

    UPROPERTY()
    FVector StartingCameraDragPosition;

    UPROPERTY()
    bool IsDraggingCamera;

    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    float ZoomDuration = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    float DefaultZoom = 2000.f;

    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    float MinZoom = 1000.f;

    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    float MaxZoom = 3000.f;
	
    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    float ZoomStep = 500.f;

    UPROPERTY(EditAnywhere, Category = "Camera | Zoom")
    UCurveFloat* ZoomCurve;

    UPROPERTY(EditAnywhere, Category = "Camera | Movement")
    float MoveSpeed = 1.f;

    UPROPERTY(EditAnywhere, Category = "Camera | Movement")
    float RotationSpeed = 1.f;

    UPROPERTY(EditAnywhere, Category = "Camera | Drag")
    float DragSpeed = 100.f;
    
};
