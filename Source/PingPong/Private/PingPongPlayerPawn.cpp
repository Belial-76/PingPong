// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APingPongPlayerPawn::APingPongPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SetReplicates(true);
}

void APingPongPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APingPongPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

