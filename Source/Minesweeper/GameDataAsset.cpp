// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataAsset.h"

void UGameDataAsset::Reset()
{
	bIsGameOver = false;
	bIsPlayerWinner = false;
	TotalEmptyCells = 0;
}
