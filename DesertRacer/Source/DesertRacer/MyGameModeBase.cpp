// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void AMyGameModeBase::ResetLevel(bool IsWin)
{
	float ResetTime = LoseResetTime;
	if (IsWin)
	{
		ResetTime = WinResetTime;
	}

	GetWorldTimerManager().SetTimer(ResetGameTimer, this, &AMyGameModeBase::OnResetGameTimerTimeout,
		1.0f, false, ResetTime);
}

void AMyGameModeBase::OnResetGameTimerTimeout()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}
