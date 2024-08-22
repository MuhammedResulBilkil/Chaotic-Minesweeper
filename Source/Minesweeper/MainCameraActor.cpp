// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCameraActor::AMainCameraActor(): CameraComponent(nullptr), SpringArmComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AMainCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCameraActor::SetSpringArmLength(float Length)
{
	SpringArmComponent->TargetArmLength = Length;
}

