// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MinesweeperGridDataAsset.generated.h"

class ACell;
/**
 * 
 */
UCLASS()
class MINESWEEPER_API UMinesweeperGridDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 MineCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid")
	AActor* GridStartLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<FVector> GridCenterPosses;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<ACell*> Cells;

	TArray<TArray<ACell*>> NestedCells;

	void GenerateGrid();

	void Reset();
};
