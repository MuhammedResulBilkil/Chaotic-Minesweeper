// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

class ACell;
class UMinesweeperGrid;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	UMinesweeperGrid* MinesweeperGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	AActor* GridStartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cell")
	TSubclassOf<ACell> Cell;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 TotalEmptyCells;

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
	
	UFUNCTION()
	void CameraDistanceSliderValueChanged(float Value);
	UFUNCTION()
	void GridAsSquareSliderValueChanged(float Value);
	UFUNCTION()
	void MineCountSliderValueChanged(float Value);
};
