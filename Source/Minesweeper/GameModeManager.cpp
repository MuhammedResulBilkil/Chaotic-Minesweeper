// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeManager.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void AGameModeManager::BeginPlay()
{
	Super::BeginPlay();

	//Create viewport
	if (GeneralUI)
	{
		GeneralUIWidget = CreateWidget<UUserWidget>(GetWorld(), GeneralUI);
		TArray<AActor*> CameraActorArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActorArray);

		for (AActor* Actor : CameraActorArray)
		{
			UE_LOG(LogTemp, Warning, TEXT("CameraActor: %s"), *Actor->GetName());
			
			if(Actor->ActorHasTag("Camera_Main"))
			{
				CameraActor = Cast<ACameraActor>(Actor);
				break;
			}
		}
		
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CameraActor, 0);
		
		if (GeneralUIWidget)
		{
			USlider* GridAsSquareSlider = (USlider*) GeneralUIWidget->GetWidgetFromName(TEXT("Slider_GridAsSquare"));
			GridAsSquareText = (UTextBlock*) GeneralUIWidget->GetWidgetFromName(TEXT("Text_GridAsSquare"));
			
			if(GridAsSquareSlider)
				GridAsSquareSlider->OnValueChanged.AddDynamic(this, &AGameModeManager::GridAsSquareSliderValueChanged);
			
			GeneralUIWidget->AddToViewport();
		}
	}
	
}

void AGameModeManager::GridAsSquareSliderValueChanged(float Value)
{
	//log
	UE_LOG(LogTemp, Warning, TEXT("GridAsSquareSliderValueChanged: %f"), Value);

	GridAsSquareText->SetText(FText::FromString(FString::Printf(TEXT("Grid as Square: %d"), (int)Value)));
}
