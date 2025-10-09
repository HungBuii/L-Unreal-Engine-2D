// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "PaperZDAnimInstance.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);

	HPText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HPText"));
	HPText->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);

	UpdateHP(HitPoint);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && FollowTarget)
	{
		float MoveDirection = (FollowTarget->GetActorLocation().X - GetActorLocation().X) > 0.f ? 1.f : -1.f;
		UpdateDirection(MoveDirection);

		if (ShouldMoveToTarget())
		{
			if (CanMove)
			{
				FVector WorldDirection = FVector(1.f, 0.f, 0.f);
				AddMovementInput(WorldDirection, MoveDirection);
			}
		}
		
	}
}

void AEnemy::UpdateDirection(float MoveDirection)
{
	FRotator CurrentRotation = GetActorRotation();

	if (MoveDirection < 0.f)
	{
		if (CurrentRotation.Yaw != 180.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll));
		}
	}
	else if (MoveDirection > 0.f)
	{
		if (CurrentRotation.Yaw != 0.f)
		{
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
		}
	}
}

bool AEnemy::ShouldMoveToTarget()
{
	bool Result = false;
	if (FollowTarget)
	{
		float DistToTarget = abs(FollowTarget->GetActorLocation().X - GetActorLocation().X);
		Result = DistToTarget > StopDistanceToTarget;
	}
	
	return Result;
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		FollowTarget = Player;
	}
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		FollowTarget = NULL;
	}
}

void AEnemy::UpdateHP(int NewHP)
{
	HitPoint = NewHP;

	FString Str = FString::Printf(TEXT("HP: %d"), HitPoint);
	HPText->SetText(FText::FromString(Str));
}

void AEnemy::TakeDamage(int DamageAmount, float StunDuration)
{
	if (!IsAlive) return;

	UpdateHP(HitPoint - DamageAmount);

	if (HitPoint <= 0)
	{
		// Enemy is dead
		UpdateHP(0);
		HPText->SetHiddenInGame(true);

		IsAlive = false;
		CanMove = false;

		// Play the die animation
		GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CrabbyStateMachine"));
		
	}
	else
	{
		// Enenmy is alive

		// Play the take hit animation
		GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CrabbyStateMachine"));
		
	}
}
