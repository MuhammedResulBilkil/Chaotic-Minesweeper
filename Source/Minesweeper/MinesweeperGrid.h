// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MinesweeperGrid.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API UMinesweeperGrid : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid")
	int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	AActor* GridStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<FVector> GridCenterPosses;
};
