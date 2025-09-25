// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopdownCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class GUNSUVIVORS_API ATopdownCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopdownCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPaperFlipbookComponent* CharacterFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPaperFlipbook* IdleFlipbook;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPaperFlipbook* RunFlipbook;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float XPos);
	bool IsInMapBoundsVertical(float ZPos);
	
};
