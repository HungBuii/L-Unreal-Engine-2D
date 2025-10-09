// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PlayerDetectorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTextRenderComponent* HPText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APlayerCharacter* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistanceToTarget = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoint = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DetectorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	
	bool ShouldMoveToTarget();
	void UpdateDirection(float MoveDirection);

	void UpdateHP(int NewHP);

	void TakeDamage(int DamageAmount, float StunDuration);
};
