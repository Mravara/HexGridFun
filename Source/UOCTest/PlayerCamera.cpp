// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCamera.h"

#include "EnhancedInputComponent.h"
#include "Hex.h"
#include "LineTypes.h"
#include "UOCTestGameMode.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCamera::APlayerCamera() : APawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement Component"));
	AddOwnedComponent(FloatingPawnMovement);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> ClickInputAction(TEXT("/Game/TopDown/Input/Actions/IA_SetDestination_Click"));
    if (const UInputAction* ActionObject = ClickInputAction.Object)
	{
		ClickAction = ActionObject;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RightClickInputAction(TEXT("/Game/TopDown/Input/Actions/IA_RightClick"));
	if (const UInputAction* ActionObject = RightClickInputAction.Object)
	{
		RightClickAction = ActionObject;
	}

    static ConstructorHelpers::FObjectFinder<UInputAction> RightClickModifiedInputAction(TEXT("/Game/TopDown/Input/Actions/IA_RightClickModified"));
    if (const UInputAction* ActionObject = RightClickModifiedInputAction.Object)
    {
        RightClickActionModified = ActionObject;
    }
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &APlayerCamera::OnMouseClicked);

	    // Right Click
	    EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &APlayerCamera::OnRightMouseClicked);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayerCamera::OnRightMouseHold);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &APlayerCamera::OnRightMouseReleased);

	    // Right Click Modified (Shift)
	    EnhancedInputComponent->BindAction(RightClickActionModified, ETriggerEvent::Started, this, &APlayerCamera::OnRightMouseModifiedClicked);
	    EnhancedInputComponent->BindAction(RightClickActionModified, ETriggerEvent::Triggered, this, &APlayerCamera::OnRightMouseModifiedHold);
	    EnhancedInputComponent->BindAction(RightClickActionModified, ETriggerEvent::Completed, this, &APlayerCamera::OnRightMouseModifiedReleased);
	}
}

void APlayerCamera::OnMouseClicked()
{
	AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FVector ClickLocation = GetMouseWorldLocation();
	Hex Tile = GameMode->GridManager->WorldToHex(ClickLocation);
	AHexTile* HexTile = GameMode->GridManager->GetTileByHex(Tile);
	if (HexTile)
	{
		HexTile->ShuffleMaterials();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("TILE_%d_%d_%d"), Tile.Q, Tile.R, Tile.S));
	}
}

FVector APlayerCamera::GetMouseWorldLocation() const
{
	FVector2D MousePosition;
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
	{
		FVector StartLocation = CameraComponent->GetComponentLocation();
		FVector WorldOrigin;
		FVector WorldDirection;
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldOrigin, WorldDirection))
		{
			FHitResult Hit;
			bool Success = GetWorld()->LineTraceSingleByChannel(
				Hit,
				WorldOrigin,
				StartLocation + WorldDirection * 20000.0f,
				TraceChannel);

			if (Success)
			{
				return Hit.ImpactPoint;
			}
		}
	}

	return FVector();
}

void APlayerCamera::OnRightMouseClicked()
{
	AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FVector ClickLocation = GetMouseWorldLocation();
	StartHex = GameMode->GridManager->WorldToHex(ClickLocation);
}

void APlayerCamera::OnRightMouseReleased()
{
	AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    GameMode->GridManager->UnselectHexes();

	
	// for (auto &Value : Hexes)
	// {
	// 	AHexTile* HexTile = GameMode->GridManager->GetTileByHex(Value);
	// 	if (HexTile)
	// 	{
	// 		HexTile->ShuffleMaterials();
	// 	}
	// }
}

void APlayerCamera::OnRightMouseHold()
{
    const AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    const FVector ClickLocation = GetMouseWorldLocation();
	EndHex = GameMode->GridManager->WorldToHex(ClickLocation);

    // Unselect hexes
    GameMode->GridManager->UnselectHexes();

    // Select Line
    std::vector<Hex> Hexes = GameMode->GridManager->GetHexesInRange(StartHex, GameMode->GridManager->Distance(StartHex, EndHex));
    GameMode->GridManager->SelectHexes(Hexes);
    
	DrawLine();
}

void APlayerCamera::OnRightMouseModifiedClicked()
{
    const AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    const FVector ClickLocation = GetMouseWorldLocation();
    StartHex = GameMode->GridManager->WorldToHex(ClickLocation);
}

void APlayerCamera::OnRightMouseModifiedHold()
{
    const AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    const FVector ClickLocation = GetMouseWorldLocation();
    EndHex = GameMode->GridManager->WorldToHex(ClickLocation);

    // Unselect hexes
    GameMode->GridManager->UnselectHexes();

    // Select Line
    std::vector<Hex> Hexes = GameMode->GridManager->GetHexLine(StartHex, EndHex);
    GameMode->GridManager->SelectHexes(Hexes);

    // Draw line
    DrawLine(FColor::Red, true);
}

void APlayerCamera::OnRightMouseModifiedReleased()
{
    const AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    const FVector ClickLocation = GetMouseWorldLocation();
    StartHex = GameMode->GridManager->WorldToHex(ClickLocation);

    // Unselect hexes
    GameMode->GridManager->UnselectHexes();
}

void APlayerCamera::DrawLine(const FColor Color, bool DrawDots) const
{
    const AUOCTestGameMode* GameMode = Cast<AUOCTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    const FVector StartLocation = GameMode->GridManager->HexToWorldLocation(StartHex) + FVector::UpVector * 20.f;
    const FVector EndLocation = GameMode->GridManager->HexToWorldLocation(EndHex) + FVector::UpVector * 20.f;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, 0.f, 0, 10.f);

    if (DrawDots)
    {
        float Distance = (EndLocation - StartLocation).Size();
        const int HexDistance = GameMode->GridManager->Distance(StartHex, EndHex);
        float Step = Distance / HexDistance;
        FVector Direction = (EndLocation - StartLocation).GetSafeNormal();

        for (int i = 0; i <= HexDistance; i++)
        {
            FVector CenterLocation = StartLocation + Direction * Step * i;
            FTransform Transform(FRotator(90.f, 0.f, 0.f), CenterLocation);
            DrawDebugCircle(GetWorld(), Transform.ToMatrixWithScale(), 15.f, 32, Color, false, 0.f, 0);
        }
    }
	// int Distance = GameMode->GridManager->Distance(StartHex, EndHex);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Distance: %d"), Distance)); // int
}

