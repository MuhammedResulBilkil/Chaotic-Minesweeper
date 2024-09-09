// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class URichTextBlock;
class UGameDataAsset;
class ACell;
class UMinesweeperGridDataAsset;
class UTextBlock;
class USpringArmComponent;
class UCameraComponent;
class AMainCameraActor;

UCLASS()
class MINESWEEPER_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();
	void SetNestedCellsSize();
	void SpawnCells();
	void FillNestedCells();
	void AllocateMines();

	void CreateGeneralUI();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GeneralUI;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Camera")
	AMainCameraActor* MainCameraActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Grid")
	UMinesweeperGridDataAsset* MinesweeperGridDataAsset;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Grid")
	AActor* GridStartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cell")
	TSubclassOf<ACell> Cell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	UGameDataAsset* GameDataAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	TObjectPtr<UUserWidget> GeneralUIWidget;

	UPROPERTY()
	TObjectPtr<UTextBlock> CameraDistanceText;
	UPROPERTY()
	TObjectPtr<UTextBlock> GridAsSquareText;
	UPROPERTY()
	TObjectPtr<UTextBlock> MineCountText;
	UPROPERTY()
	TObjectPtr<URichTextBlock> GameStatusText;
	
	UFUNCTION()
	void OnCameraDistanceSliderValueChanged(float Value);
	UFUNCTION()
	void OnGridAsSquareSliderValueChanged(float Value);
	UFUNCTION()
	void OnMineCountSliderValueChanged(float Value);
	
	UFUNCTION()
	void OnMineClicked();
	UFUNCTION()
	void OnEmptyClicked();

	void ShowGameStatusText(const char* Value);
};
