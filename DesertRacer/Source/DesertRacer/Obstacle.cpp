// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

#include "MyGameModeBase.h"
#include "PaperSpriteComponent.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	ObstacleSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ObstacleSprite"));
	ObstacleSprite->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OverlapBegin);
	
	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("Overlapped with player"));
		UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetActorNameOrLabel());
		UE_LOG(LogTemp, Warning, TEXT("OverlappedComponent: %s"), *OverlappedComponent->GetName());
		UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetName());
		UE_LOG(LogTemp, Warning, TEXT("OtherBodyIndex: %i"), OtherBodyIndex);
		UE_LOG(LogTemp, Warning, TEXT("FromSweep: %d"), FromSweep);
		UE_LOG(LogTemp, Warning, TEXT("SweepResult: %s"), *SweepResult.ToString());
		
		if (Player->CanMove)
		{
			Player->CanMove = false;

			UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
			
			MyGameMode->ResetLevel(IsFinishLine);
		}
	}
}

