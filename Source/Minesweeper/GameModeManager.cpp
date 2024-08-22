// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeManager.h"

void AGameModeManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}


