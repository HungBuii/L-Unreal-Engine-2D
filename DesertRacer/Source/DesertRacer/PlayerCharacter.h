// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class DESERTRACER_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* CarSprite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;
};
