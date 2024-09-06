// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "CellWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();

	//find cell widget component
	CellWidgetComponent = FindComponentByClass<UWidgetComponent>();
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UCellWidget* ACell::GetCellWidget()
{	
	return Cast<UCellWidget>(CellWidgetComponent->GetUserWidgetObject());
}

