// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellType.h"
#include "CellWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

class UGameDataAsset;
class UMinesweeperGridDataAsset;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMineClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmptyClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckIsGameEndSignature);

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 NeighbourMineCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 GridXLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell")
	int32 GridYLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	UMinesweeperGridDataAsset* MinesweeperGridDataAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	UGameDataAsset* GameDataAsset;

	UPROPERTY(BlueprintAssignable)
	FOnMineClickedSignature MineClickedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEmptyClickedSignature EmptyClickedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnCheckIsGameEndSignature CheckIsGameEndDelegate;

	void ShowMark();
	void ShowCell();
	void Reveal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	TObjectPtr<UWidgetComponent> CellWidgetComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> CellStaticMeshComponent;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CellMaterialInstanceDynamic;

	UPROPERTY()
	TObjectPtr<UGeometryCollectionComponent> CellGeometryCollectionComponent;
	
	UPROPERTY()
	TObjectPtr<UImage> FlagImage;
	UPROPERTY()
	TObjectPtr<UImage> MineImage;
	UPROPERTY()
	TObjectPtr<UTextBlock> NeighbourMineCountText;
	
	bool bIsRevealedOnce;
	bool bIsTotalEmptyCellsDecreased;
	bool bIsMarkOn;
	
	void ShowNeighbourMineCount();
	void FloodFill();
	
	UCellWidget* GetCellWidget();
};
