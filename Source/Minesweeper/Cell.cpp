// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "CellWidget.h"
#include "Components/Image.h"
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
	
	CellStaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	CellMaterialInstanceDynamic = CellStaticMeshComponent->
	CreateAndSetMaterialInstanceDynamicFromMaterial(0, CellStaticMeshComponent->GetMaterial(0));
	
	CellWidgetComponent = FindComponentByClass<UWidgetComponent>();
	CellWidgetComponent->SetCastShadow(false);

	FlagImage = (UImage*) GetCellWidget()->GetWidgetFromName("Image_Flag");
	MineImage = (UImage*) GetCellWidget()->GetWidgetFromName("Image_Mine");
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACell::Reveal()
{
	if(CellType == ECT_Mine)
	{
		CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Red);
		MineImage->SetVisibility(ESlateVisibility::Visible);

		MineClicked.Broadcast();
	}
	else if (CellType == ECT_Empty)
	{
		CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Green);

		EmptyClicked.Broadcast();
	}
}

UCellWidget* ACell::GetCellWidget()
{	
	return Cast<UCellWidget>(CellWidgetComponent->GetUserWidgetObject());
}

void ACell::ShowMark()
{
	bIsMarkOn = !bIsMarkOn;
	FlagImage->SetVisibility(bIsMarkOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

