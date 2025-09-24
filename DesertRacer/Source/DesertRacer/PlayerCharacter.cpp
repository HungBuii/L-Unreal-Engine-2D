// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	CarSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CarSprite"));
	CarSprite->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
														(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();
	// GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::White, MoveActionValue.ToString());

	if (CanMove)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		if (abs(MoveActionValue.X) > 0.f)
		{
			float RotationAmount = RotationSpeed * MoveActionValue.X * DeltaTime;
			AddActorWorldRotation(FRotator(-RotationAmount, 0, 0));
		} // Apply to situations where a car is stationary and can still be turned.
		
		if (abs(MoveActionValue.Y) > 0.f)
		{
			// if (abs(MoveActionValue.X) > 0.f)
			// {
			// 	float RotationAmount = -RotationSpeed * MoveActionValue.X * DeltaTime;
			// 	AddActorWorldRotation(FRotator(RotationAmount, 0, 0));
			// } // Apply to situations where the car is stationary but cannot be turned.
			
			float FinalMovementSpeed = MovementSpeed;
			if (MoveActionValue.Y < 0.f)
			{
				FinalMovementSpeed *= 0.5f;
			}
			
			FVector CurrentLocation = GetActorLocation();
			FVector DistanceToMove = GetActorUpVector() * FinalMovementSpeed * MoveActionValue.Y * DeltaTime;
			// GetActorUpVector(): direction of Z vector in world space (positive)
			FVector NewLocation = CurrentLocation + DistanceToMove;
			
			SetActorLocation(NewLocation);
		}
	}
	
}

