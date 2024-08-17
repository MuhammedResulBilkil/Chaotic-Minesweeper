// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeManager.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MINESWEEPER_API AGameModeManager : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GeneralUI;

private:

	UPROPERTY()
	ACameraActor* CameraActor;
	
	UPROPERTY()
	UUserWidget* GeneralUIWidget;

	UPROPERTY()
	UTextBlock* GridAsSquareText;

	UFUNCTION()
	void GridAsSquareSliderValueChanged(float Value);
};
