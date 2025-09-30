// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "PaperFlipbookComponent.h"
#include "TopdownCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanFollow && Player)
	{
		// Move towards the player
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
		float DistanceToPlayer = DirectionToPlayer.Length();

		if (DistanceToPlayer >= StopDistance)
		{
			DirectionToPlayer.Normalize();
			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}

		// Face the player
		CurrentLocation = GetActorLocation();
		float FlipbookXScale = EnemyFlipbook->GetComponentScale().X;

		if ((PlayerLocation.X - CurrentLocation.X) >= 0.0f) // Player is on the right side of the enemy
		{
			if (FlipbookXScale < 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		else // Player is on the left side of the enemy
		{
			if (FlipbookXScale > 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
	}
}

void AEnemy::Die()
{
	if (!IsAlive) return;

	IsAlive = false;
	CanFollow = false;

	EnemyFlipbook->SetFlipbook(DeadFlipbookAsset);
	EnemyFlipbook->SetTranslucentSortPriority(-5);

	EnemyDiedDelegate.Broadcast();
	
	float DestroyTime = 10.f;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout,
		1.f, false, DestroyTime);
	
}

void AEnemy::OnDestroyTimerTimeout()
{
	Destroy();
}

