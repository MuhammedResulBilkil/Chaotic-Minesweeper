// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "CellWidget.h"
#include "MinesweeperGridDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
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
	NeighbourMineCountText = (UTextBlock*) GetCellWidget()->GetWidgetFromName("Text_NeighbourMineCount");
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACell::ShowNeighbourMineCount()
{
	if(NeighbourMineCount > 0)
		NeighbourMineCountText->SetText(FText::FromString(FString::FromInt(NeighbourMineCount)));
}

void ACell::Reveal()
{
	if(CellType == ECT_Mine)
	{
		CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Red);
		MineImage->SetVisibility(ESlateVisibility::Visible);

		MineClickedDelegate.Broadcast();

		return;
	}

	FlagImage->SetVisibility(ESlateVisibility::Hidden);

	if (CellType == ECT_Empty && NeighbourMineCount != 0)
	{
		CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Green);

		ShowNeighbourMineCount();

		//EmptyClickedDelegate.Broadcast();
	}

	CellType = ECT_Revealed;
	
	if(NeighbourMineCount == 0)
	{
		CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Green);

		FloodFill();
	}
}

void ACell::FloodFill()
{
	for (int xOff = -1; xOff <= 1; xOff++)
	{
		for (int yOff = -1; yOff <= 1; yOff++)
		{
			int i = GridIndexX + xOff;
			int j = GridIndexY + yOff;
                
			if (i > -1 && i < GridXLength && j > -1 && j < GridYLength)
			{
				ACell* neighbour = MinesweeperGridDataAsset->NestedCells[i][j];
                    
				if (neighbour->CellType != ECT_Mine && neighbour->CellType != ECT_Revealed)
				{
					neighbour->CellMaterialInstanceDynamic->SetVectorParameterValue("BaseColor", FLinearColor::Green);

					neighbour->Reveal();
				}
			}
		}
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

