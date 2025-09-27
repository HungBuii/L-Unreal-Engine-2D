// Fill out your copyright notice in the Description page of Project Settings.


#include "TopdownCharacter.h"

#include "Bullet.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATopdownCharacter::ATopdownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(RootComponent);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);
	
}

// Called when the game starts or when spawned
void ATopdownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
														(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

bool ATopdownCharacter::IsInMapBoundsHorizontal(float XPos)
{
	bool Result = true;
	Result = (XPos > HorizontalLimits.X) && (XPos < HorizontalLimits.Y);

	return Result;
}

bool ATopdownCharacter::IsInMapBoundsVertical(float ZPos)
{
	bool Result = true;
	Result = (ZPos > VerticalLimits.X) && (ZPos < VerticalLimits.Y);

	return Result;
}

// Called every frame
void ATopdownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move player
	if (CanMove)
	{
		if (MovementDirection.Length() > 0.f)
		{
			if (MovementDirection.Length() > 1.0f)
			{
				MovementDirection.Normalize();
			}

			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.f, 0.f);
			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(DistanceToMove.X, 0.f, 0.f);
			}
	
			NewLocation += FVector(0.f, 0.f, DistanceToMove.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				NewLocation -= FVector(0.f, 0.f, DistanceToMove.Y);
			}
			
			SetActorLocation(NewLocation);
		}
	}

	// Rotate the gun
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector CurrentLocation = GetActorLocation();
		FVector Start = FVector(CurrentLocation.X, 0.f, CurrentLocation.Z);
		FVector Target = FVector(MouseWorldLocation.X, 0.f, MouseWorldLocation.Z);
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		GunParent->SetRelativeRotation(GunParentRotator);
	}
	
}

// Called to bind functionality to input
void ATopdownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopdownCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopdownCharacter::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopdownCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::Shoot);
	}
}

void ATopdownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();

	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, MoveActionValue.ToString());

	if (CanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbook->SetFlipbook(RunFlipbook);

		FVector FlipbookScale = CharacterFlipbook->GetComponentScale();
		if (MovementDirection.X < 0.f)
		{
			if (FlipbookScale.X > 0.f)
			{
				CharacterFlipbook->SetWorldScale3D(FVector(-1.f, 1.f, 1.f));
			}
		}
		else if (MovementDirection.X > 0.f)
		{
			if (FlipbookScale.X < 0.f)
			{
				CharacterFlipbook->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
			}
		}
	}
}

void ATopdownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("MoveCompleted"));

	MovementDirection = FVector2D::ZeroVector;
	CharacterFlipbook->SetFlipbook(IdleFlipbook);
}

void ATopdownCharacter::Shoot(const FInputActionValue& Value)
{
	if (CanShoot)
	{
		CanShoot = false;

		// Bullet Spawn code...
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shoot"));

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPosition->GetComponentLocation(),
			FRotator::ZeroRotator);
		check(Bullet);

		// Get mouse world location
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		check(PlayerController);
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Calculate bullet direction
		FVector CurrentLocation = GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X,
			MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection.Normalize();

		// Launch the bullet
		float BulletSpeed = 300.f;
		Bullet->Launch(BulletDirection, BulletSpeed);
		
		GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATopdownCharacter::OnShootCooldownTimerTimeout,
			1.f, false, ShootCooldownDurationInSeconds);

		
	}
}

void ATopdownCharacter::OnShootCooldownTimerTimeout()
{
	CanShoot = true;
	
}

