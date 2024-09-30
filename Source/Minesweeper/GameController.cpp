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

void AGameController::StartGeneratingMinesweeperGrid()
{
	MinesweeperGridDataAsset->GridStartLocation = GridStartLocation;

	if(MinesweeperGridDataAsset->GridStartLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridStartLocation: %s"), *MinesweeperGridDataAsset->GridStartLocation->GetActorLocation().ToString());

		MinesweeperGridDataAsset->GenerateGrid();

		SpawnCells();

		SetNestedCellsSize();

		FillNestedCells();

		AllocateMines();

		CountNeighbourMines();

		CenterMainCameraActor();
	}
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameController BeginPlay"));

	GameDataAsset->RestartActionDelegate.AddDynamic(this, &AGameController::OnRestartAction);

	DefaultWidth = MinesweeperGridDataAsset->Width;
	DefaultHeight = MinesweeperGridDataAsset->Height;
	DefaultMineCount = MinesweeperGridDataAsset->MineCount;

	GameDataAsset->Reset();
	MinesweeperGridDataAsset->Reset();
		
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(MainCameraActor, 0);

	CreateGeneralUI();

	StartGeneratingMinesweeperGrid();
}

void AGameController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UE_LOG(LogTemp, Warning, TEXT("GameController EndPlay: %d"), EndPlayReason);

	GameDataAsset->RestartActionDelegate.RemoveDynamic(this, &AGameController::OnRestartAction);

	MinesweeperGridDataAsset->Width = DefaultWidth;
	MinesweeperGridDataAsset->Height = DefaultHeight;
	MinesweeperGridDataAsset->MineCount = DefaultMineCount;

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
#if WITH_EDITOR
  spawnedCell->SetActorLabel(FString::Printf(TEXT("Cell_%d"), index++));
#endif

		spawnedCell->AttachToActor(MinesweeperGridDataAsset->GridStartLocation, FAttachmentTransformRules::KeepWorldTransform);
		spawnedCell->CellType = ECT_Empty;
		spawnedCell->MineClickedDelegate.AddDynamic(this, &AGameController::OnMineClicked);
		spawnedCell->EmptyClickedDelegate.AddDynamic(this, &AGameController::OnEmptyClicked);
		spawnedCell->CheckIsGameEndDelegate.AddDynamic(this, &AGameController::OnCheckIsGameEnd);

		MinesweeperGridDataAsset->Cells.Add(spawnedCell);
	}

	GameDataAsset->TotalEmptyCells = MinesweeperGridDataAsset->Cells.Num();
}

void AGameController::CountNeighbourMines()
{
	for (ACell* CellActor : MinesweeperGridDataAsset->Cells)
	{
		CellActor->GridXLength = MinesweeperGridDataAsset->NestedCells.Num();
		CellActor->GridYLength = MinesweeperGridDataAsset->NestedCells[0].Num();
		
		if(CellActor->CellType == ECT_Mine)
		{
			CellActor->NeighbourMineCount = -1;
			continue;
		}

		int32 TotalNeighbourMines = 0;

		for (int xOff = -1; xOff <=1; xOff++)
		{
			for (int yOff = -1; yOff <=1; yOff++)
			{
				int i = CellActor->GridIndexX + xOff;
				int j = CellActor->GridIndexY + yOff;

				if (i > -1 && i < MinesweeperGridDataAsset->NestedCells.Num() && j > -1 && j < MinesweeperGridDataAsset->NestedCells[0].Num())
				{
					ACell* NeighbourCell = MinesweeperGridDataAsset->NestedCells[i][j];
                
					if(NeighbourCell->CellType == ECT_Mine)
						TotalNeighbourMines++;
				}
			}
		}

		CellActor->NeighbourMineCount = TotalNeighbourMines;
	}
}

void AGameController::SetNestedCellsSize()
{	
	MinesweeperGridDataAsset->NestedCells.SetNum(MinesweeperGridDataAsset->Width);
		
	for (int32 i = 0; i < MinesweeperGridDataAsset->Width; i++)
		MinesweeperGridDataAsset->NestedCells[i].SetNum(MinesweeperGridDataAsset->Height);
}

void AGameController::OnCheckIsGameEnd()
{
	GameDataAsset->TotalEmptyCells--;
	
	if(GameDataAsset->TotalEmptyCells == 0)
		PlayerWins();
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

#if WITH_EDITOR
  UE_LOG(LogTemp, Warning, TEXT("NestedCells[%d][%d]: %s"), i, j, *MinesweeperGridDataAsset->NestedCells[i][j]->GetActorLabel());
#endif
		}
	}
}

void AGameController::AllocateMines()
{
	int32 MineCount = MinesweeperGridDataAsset->MineCount;
	
	if(MineCount > MinesweeperGridDataAsset->Cells.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("MineCount is greater than total cells!"));

		MineCount = MinesweeperGridDataAsset->Cells.Num();
	}
	
	for (int32 i = 0; i < MineCount; i++)
	{
		int32 j = FMath::RandRange(0, MinesweeperGridDataAsset->NestedCells.Num() - 1);
		int32 k = FMath::RandRange(0, MinesweeperGridDataAsset->NestedCells[j].Num() - 1);

		if(MinesweeperGridDataAsset->NestedCells[j][k]->CellType == ECT_Mine)
		{
			i--;
			continue;
		}
		
		MinesweeperGridDataAsset->NestedCells[j][k]->CellType = ECT_Mine;
		GameDataAsset->TotalEmptyCells--;
	}
}

void AGameController::OnCameraDistanceSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("CameraDistanceSliderValueChanged: %f"), Value);

	CameraDistanceText->SetText(FText::FromString(FString::Printf(TEXT("Camera Distance: %d"), (int)Value)));
	
	MainCameraActor->SetSpringArmLength(Value);
}

void AGameController::OnGridAsSquareSliderValueChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("GridAsSquareSliderValueChanged: %f"), Value);

	GridAsSquareText->SetText(FText::FromString(FString::Printf(TEXT("Grid as Square: %d"), (int)Value)));

	MinesweeperGridDataAsset->Width = (int32) Value;
	MinesweeperGridDataAsset->Height = (int32) Value;
}

void AGameController::OnMineCountSliderValueChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("MineCountSliderValueChanged: %f"), Value);

	MineCountText->SetText(FText::FromString(FString::Printf(TEXT("Mine Count: %d"), (int)Value)));

	MinesweeperGridDataAsset->MineCount = (int32) Value;
}

void AGameController::PlayerWins()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCheckIsGameEnd - PlayerWon!"));
		
	GameDataAsset->bIsGameOver = true;
	GameDataAsset->bIsPlayerWinner = true;

	ShowGameStatusText("<Style.Bold>You Won!</>");

	for (ACell* CellActor : MinesweeperGridDataAsset->Cells)
		CellActor->ShowCell();
}

void AGameController::PlayerLost()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMineClicked - PlayerLost!"));
		
	GameDataAsset->bIsGameOver = true;
	GameDataAsset->bIsPlayerWinner = false;

	ShowGameStatusText("<Style.Bold>You Lost!</>");

	for (ACell* CellActor : MinesweeperGridDataAsset->Cells)
		CellActor->ShowCell();
}

void AGameController::OnMineClicked()
{
	PlayerLost();
}

void AGameController::OnEmptyClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnEmptyClicked"));

	
}

void AGameController::OnRestartAction()
{
	for (ACell* CellActor : MinesweeperGridDataAsset->Cells)
	{
		CellActor->MineClickedDelegate.RemoveDynamic(this, &AGameController::OnMineClicked);
		CellActor->EmptyClickedDelegate.RemoveDynamic(this, &AGameController::OnEmptyClicked);
		CellActor->CheckIsGameEndDelegate.RemoveDynamic(this, &AGameController::OnCheckIsGameEnd);
		
		CellActor->Destroy();
	}

	MinesweeperGridDataAsset->Reset();
	GameDataAsset->Reset();

	GameStatusText->SetVisibility(ESlateVisibility::Hidden);

	StartGeneratingMinesweeperGrid();
}

void AGameController::CenterMainCameraActor()
{
	FVector GridCenter = FVector::ZeroVector;
	
	for (ACell* CellActor : MinesweeperGridDataAsset->Cells)
		GridCenter += CellActor->GetActorLocation();

	GridCenter /= MinesweeperGridDataAsset->Cells.Num();

	GridCenter.X = MainCameraActor->GetActorLocation().X;

	MainCameraActor->SetActorLocation(GridCenter);
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

