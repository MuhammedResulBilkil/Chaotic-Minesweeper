// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerActor.generated.h"

class AGameController;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MINESWEEPER_API APlayerActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* MouseLeftButtonReleasedAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* MouseRightButtonReleasedAction;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void LineTraceFromMousePosition(FHitResult& HitResult);

private:

	UFUNCTION()
	void OnMouseLeftButtonReleased();
	UFUNCTION()
	void OnMouseRightButtonReleased();
};
