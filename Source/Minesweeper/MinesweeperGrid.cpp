// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperGrid.h"

void UMinesweeperGrid::GenerateGrid()
{
	GridCenterPosses.Empty();
	
	for (int32 y = 0; y < Width; y++)
	{
		for (int32 z = 0; z < Height; z++)
		{
			FVector CellLocation = GridStartLocation->GetActorLocation() + FVector(0.0f, y * CellSize, z * CellSize);
			GridCenterPosses.Add(CellLocation);
		}
	}
}
