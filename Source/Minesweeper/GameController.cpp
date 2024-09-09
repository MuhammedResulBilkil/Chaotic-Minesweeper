// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"

#include "Cell.h"
#include "GameDataAsset.h"
#include "MainCameraActor.h"
#include "MinesweeperGridDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
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

	GameDataAsset->Reset();
	MinesweeperGridDataAsset->Reset();
		
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(MainCameraActor, 0);

	CreateGeneralUI();

	MinesweeperGridDataAsset->GridStartLocation = GridStartLocation;

	if(MinesweeperGridDataAsset->GridStartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridStartLocation: %s"), *MinesweeperGridDataAsset->GridStartLocation->GetActorLocation().ToString());

		MinesweeperGridDataAsset->GenerateGrid();

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

	GameDataAsset->Reset();
	MinesweeperGridDataAsset->Reset();
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameController::SpawnCells()
{	
	int32 index = 0;
		
	for (FVector GridCenterPoss : MinesweeperGridDataAsset->GridCenterPosses)
	{
		ACell* spawnedCell = GetWorld()->SpawnActor<ACell>(Cell, GridCenterPoss, FRotator::ZeroRotator);
		spawnedCell->SetActorLabel(FString::Printf(TEXT("Cell_%d"), index++));
		spawnedCell->AttachToActor(MinesweeperGridDataAsset->GridStartLocation, FAttachmentTransformRules::KeepWorldTransform);
		spawnedCell->CellType = ECT_Empty;
		spawnedCell->MineClicked.AddDynamic(this, &AGameController::OnMineClicked);
		spawnedCell->EmptyClicked.AddDynamic(this, &AGameController::OnEmptyClicked);

		MinesweeperGridDataAsset->Cells.Add(spawnedCell);
	}

	GameDataAsset->TotalEmptyCells = MinesweeperGridDataAsset->Cells.Num();
}

void AGameController::SetNestedCellsSize()
{	
	MinesweeperGridDataAsset->NestedCells.SetNum(MinesweeperGridDataAsset->Width);
		
	for (int32 i = 0; i < MinesweeperGridDataAsset->Width; i++)
		MinesweeperGridDataAsset->NestedCells[i].SetNum(MinesweeperGridDataAsset->Height);
}

void AGameController::FillNestedCells()
{
	int32 index = 0;

	for (int32 i = 0; i < MinesweeperGridDataAsset->NestedCells.Num(); i++)
	{
		int32 k = MinesweeperGridDataAsset->NestedCells[i].Num();
			
		for (int32 j = 0; j < k; j++)
		{
			MinesweeperGridDataAsset->NestedCells[i][j] = MinesweeperGridDataAsset->Cells[index++];
			MinesweeperGridDataAsset->NestedCells[i][j]->GridIndexX = i;
			MinesweeperGridDataAsset->NestedCells[i][j]->GridIndexY = j;

			UE_LOG(LogTemp, Warning, TEXT("NestedCells[%d][%d]: %s"), i, j, *MinesweeperGridDataAsset->NestedCells[i][j]->GetActorLabel());
		}
	}
}

void AGameController::AllocateMines()
{
	for (int32 i = 0; i < MinesweeperGridDataAsset->MineCount; i++)
	{
		int32 j = FMath::RandRange(0, MinesweeperGridDataAsset->NestedCells.Num() - 1);
		int32 k = FMath::RandRange(0, MinesweeperGridDataAsset->NestedCells[j].Num() - 1);

		MinesweeperGridDataAsset->NestedCells[j][k]->CellType = ECT_Mine;

		GameDataAsset->TotalEmptyCells--;
	}
}

void AGameController::OnCameraDistanceSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("CameraDistanceSliderValueChanged: %f"), Value);

	MainCameraActor->SetSpringArmLength(Value);
	CameraDistanceText->SetText(FText::FromString(FString::Printf(TEXT("Camera Distance: %d"), (int)Value)));
}

void AGameController::OnGridAsSquareSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("GridAsSquareSliderValueChanged: %f"), Value);

	GridAsSquareText->SetText(FText::FromString(FString::Printf(TEXT("Grid as Square: %d"), (int)Value)));
}

void AGameController::OnMineCountSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("MineCountSliderValueChanged: %f"), Value);

	MineCountText->SetText(FText::FromString(FString::Printf(TEXT("Mine Count: %d"), (int)Value)));
}

void AGameController::OnMineClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMineClicked - PlayerLost!"));
		
	GameDataAsset->bIsGameOver = true;
	GameDataAsset->bIsPlayerWinner = false;

	ShowGameStatusText("<Style.Bold>You Lost!</>");
}

void AGameController::OnEmptyClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEmptyClicked"));

	
}

void AGameController::ShowGameStatusText(const char* Value)
{
	GameStatusText->SetText(FText::FromString(Value));
	GameStatusText->SetVisibility(ESlateVisibility::Visible);
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
			GameStatusText = (URichTextBlock*) GeneralUIWidget->GetWidgetFromName(TEXT("RichText_GameStatus"));
			
			if(CameraDistanceSlider)
				CameraDistanceSlider->OnValueChanged.AddDynamic(this, &AGameController::OnCameraDistanceSliderValueChanged);
			
			if(GridAsSquareSlider)
				GridAsSquareSlider->OnValueChanged.AddDynamic(this, &AGameController::OnGridAsSquareSliderValueChanged);

			if(MineCountSlider)
				MineCountSlider->OnValueChanged.AddDynamic(this, &AGameController::OnMineCountSliderValueChanged);
			
			GeneralUIWidget->AddToViewport();
		}
	}
}

