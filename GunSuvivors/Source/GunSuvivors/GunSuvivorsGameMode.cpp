// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSuvivorsGameMode.h"

#include "Kismet/GameplayStatics.h"

AGunSuvivorsGameMode::AGunSuvivorsGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetScore(0);
}

void AGunSuvivorsGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetScore(0);
}

void AGunSuvivorsGameMode::SetScore(int NewScore)
{
	if (NewScore >= 0)
	{
		Score = NewScore;
		ScoreChangedDelegate.Broadcast(Score);
	}
}

void AGunSuvivorsGameMode::AddScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Score = %d"), Score));

}

void AGunSuvivorsGameMode::RestartGame()
{
	GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AGunSuvivorsGameMode::OnRestartGameTimerTimeout,
		1.f, false, TimeBeforeRestart);
}

void AGunSuvivorsGameMode::OnRestartGameTimerTimeout()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}
