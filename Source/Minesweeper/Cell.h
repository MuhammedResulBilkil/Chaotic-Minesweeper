// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellType.h"
#include "CellWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

class UImage;

UCLASS()
class MINESWEEPER_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cell")
	TEnumAsByte<ECellType> CellType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 GridIndexX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 GridIndexY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CellWidget")
	TObjectPtr<UWidgetComponent> CellWidgetComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UCellWidget* GetCellWidget();

	void ShowMark();

private:
	
	bool bIsMarkOn;

	UPROPERTY()
	TObjectPtr<UImage> FlagImage;
	UPROPERTY()
	TObjectPtr<UImage> MineImage;
};
