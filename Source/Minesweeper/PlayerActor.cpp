// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"

#include "Cell.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("PlayerActor BeginPlay"));
}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MouseLeftButtonReleasedAction, ETriggerEvent::Triggered, this, &APlayerActor::OnMouseLeftButtonReleased);
}

void APlayerActor::OnMouseLeftButtonReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Left Button Released!"));

	FVector WorldLocation;
	FVector WorldDirection;
	
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	//UE_LOG(LogTemp, Warning, TEXT("WorldLocation: %s"), *WorldLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("WorldDirection: %s"), *WorldDirection.ToString());

	FHitResult HitResult;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000, ECC_Visibility);

	//DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + WorldDirection * 10000, FColor::Red, false, 5, 0, 1);

	if (HitResult.bBlockingHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HitResult: %s"), *HitResult.ImpactPoint.ToString());
		UE_LOG(LogTemp, Warning, TEXT("HitResult Actor Name: %s"), *HitResult.GetActor()->GetActorLabel());
		
		ACell* Cell = Cast<ACell>(HitResult.GetActor());
		
		if(Cell)
		{
			UEnum* EnumPtr = StaticEnum<ECellType>();
			FString CellTypeName = EnumPtr->GetNameByValue(Cell->CellType.GetValue()).ToString();
			
			UE_LOG(LogTemp, Warning, TEXT("CellType: %s"), *CellTypeName);
		}
	}
}

