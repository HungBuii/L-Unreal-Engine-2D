// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PaperZDAnimInstance.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */

struct FInputActionValue;

UCLASS()
class CRUSTYPIRATE_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* AttackCollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	const UPaperZDAnimSequence* AttackAnimSequence;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerHUD* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int HitPoint = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStunDuration = 0.3f;

	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	FTimerHandle StunTimer;
	
	
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpEnded(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	void UpdateDirection(float MoveDirection);

	void OnAttackOverrideAnimEnd(bool Completed);

	UFUNCTION()
	void AttackBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool Enabled);

	void TakeDamage(int DamageAmount, float StunDuration);
	void UpdateHP(int NewHP);

	void Stun(float DurationInSeconds);
	void OnStunTimerTimeout();
};
