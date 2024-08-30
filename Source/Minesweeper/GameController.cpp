// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"

#include "Cell.h"
#include "MainCameraActor.h"
#include "MinesweeperGrid.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

// Sets default values
AGameController::AGameController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameController BeginPlay"));
		
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(MainCameraActor, 0);

	CreateGeneralUI();

	MinesweeperGrid->GridStartLocation = GridStartLocation;

	if(MinesweeperGrid->GridStartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridStartLocation: %s"), *MinesweeperGrid->GridStartLocation->GetActorLocation().ToString());

		MinesweeperGrid->GenerateGrid();

		SpawnCells();

		SetNestedCellsSize();

		FillNestedCells();

		AllocateMines();
	}
}

void AGameController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UE_LOG(LogTemp, Warning, TEXT("EndPlay: %d"), EndPlayReason);

	MinesweeperGrid->GridCenterPosses.Empty();
	MinesweeperGrid->Cells.Empty();
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameController::SpawnCells()
{
	MinesweeperGrid->Cells.Empty();
	
	int32 index = 0;
		
	for (FVector GridCenterPoss : MinesweeperGrid->GridCenterPosses)
	{
		ACell* spawnedCell = GetWorld()->SpawnActor<ACell>(Cell, GridCenterPoss, FRotator::ZeroRotator);
		spawnedCell->SetActorLabel(FString::Printf(TEXT("Cell_%d"), index++));
		spawnedCell->AttachToActor(MinesweeperGrid->GridStartLocation, FAttachmentTransformRules::KeepWorldTransform);
		spawnedCell->CellType = ECT_Empty;

		MinesweeperGrid->Cells.Add(spawnedCell);
	}

	TotalEmptyCells = MinesweeperGrid->Cells.Num();
}

void AGameController::SetNestedCellsSize()
{
	MinesweeperGrid->NestedCells.SetNum(MinesweeperGrid->Width);
		
	for (int32 i = 0; i < MinesweeperGrid->Width; i++)
		MinesweeperGrid->NestedCells[i].SetNum(MinesweeperGrid->Height);
}

void AGameController::FillNestedCells()
{
	int32 index = 0;

	for (int32 i = 0; i < MinesweeperGrid->NestedCells.Num(); i++)
	{
		int32 k = MinesweeperGrid->NestedCells[i].Num();
			
		for (int32 j = 0; j < k; j++)
		{
			MinesweeperGrid->NestedCells[i][j] = MinesweeperGrid->Cells[index++];
			MinesweeperGrid->NestedCells[i][j]->GridIndexX = i;
			MinesweeperGrid->NestedCells[i][j]->GridIndexY = j;

			UE_LOG(LogTemp, Warning, TEXT("NestedCells[%d][%d]: %s"), i, j, *MinesweeperGrid->NestedCells[i][j]->GetActorLabel());
		}
	}
}

void AGameController::AllocateMines()
{
	for (int32 i = 0; i < MinesweeperGrid->MineCount; i++)
	{
		int32 j = FMath::RandRange(0, MinesweeperGrid->NestedCells.Num() - 1);
		int32 k = FMath::RandRange(0, MinesweeperGrid->NestedCells[j].Num() - 1);

		MinesweeperGrid->NestedCells[j][k]->CellType = ECT_Mine;

		TotalEmptyCells--;
	}
}

void AGameController::CameraDistanceSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("CameraDistanceSliderValueChanged: %f"), Value);

	MainCameraActor->SetSpringArmLength(Value);
	CameraDistanceText->SetText(FText::FromString(FString::Printf(TEXT("Camera Distance: %d"), (int)Value)));
}

void AGameController::GridAsSquareSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("GridAsSquareSliderValueChanged: %f"), Value);

	GridAsSquareText->SetText(FText::FromString(FString::Printf(TEXT("Grid as Square: %d"), (int)Value)));
}

void AGameController::MineCountSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("MineCountSliderValueChanged: %f"), Value);

	MineCountText->SetText(FText::FromString(FString::Printf(TEXT("Mine Count: %d"), (int)Value)));
}

void AGameController::CreateGeneralUI()
{
	//Create viewport
	if (GeneralUI)
	{
		GeneralUIWidget = CreateWidget<UUserWidget>(GetWorld(), GeneralUI);
		
		if (GeneralUIWidget)
		{
			USlider* CameraDistanceSlider = (USlider*) GeneralUIWidget->GetWidgetFromName(TEXT("Slider_CameraDistance"));
			CameraDistanceText = (UTextBlock*) GeneralUIWidget->GetWidgetFromName(TEXT("Text_CameraDistance"));
			USlider* GridAsSquareSlider = (USlider*) GeneralUIWidget->GetWidgetFromName(TEXT("Slider_GridAsSquare"));
			GridAsSquareText = (UTextBlock*) GeneralUIWidget->GetWidgetFromName(TEXT("Text_GridAsSquare"));
			USlider* MineCountSlider = (USlider*) GeneralUIWidget->GetWidgetFromName(TEXT("Slider_MineCount"));
			MineCountText = (UTextBlock*) GeneralUIWidget->GetWidgetFromName(TEXT("Text_MineCount"));
			
			if(CameraDistanceSlider)
				CameraDistanceSlider->OnValueChanged.AddDynamic(this, &AGameController::CameraDistanceSliderValueChanged);
			
			if(GridAsSquareSlider)
				GridAsSquareSlider->OnValueChanged.AddDynamic(this, &AGameController::GridAsSquareSliderValueChanged);

			if(MineCountSlider)
				MineCountSlider->OnValueChanged.AddDynamic(this, &AGameController::MineCountSliderValueChanged);
			
			GeneralUIWidget->AddToViewport();
		}
	}
}

