// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PaperFlipbookComponent.h"

#include "LevelExit.generated.h"

UCLASS()
class CRUSTYPIRATE_API ALevelExit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* PlayerEnterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeInSeconds = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive = true;

	FTimerHandle WaitTimer;


	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	void OnWaitTimerTimeout();

};
