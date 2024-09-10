// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameDataAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartActionSignature);

UCLASS()
class MINESWEEPER_API UGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game")
	bool bIsGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game")
	bool bIsPlayerWinner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 TotalEmptyCells;

	UPROPERTY()
	FOnRestartActionSignature RestartActionDelegate;

	void Reset();
};
