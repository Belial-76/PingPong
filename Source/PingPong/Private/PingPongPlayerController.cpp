// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include "PingPongPlatform.h"
#include "Widget_Expectation.h"

APingPongPlayerController::APingPongPlayerController()
{
	SetReplicates(true);
}

void APingPongPlayerController::SetStartTransform(FTransform NewStartTransform)
{
	StartTransform = NewStartTransform;
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &APingPongPlayerController::MoveRight);
}

void APingPongPlayerController::Widget()
{
	ExpectationWidget = CreateWidget<UWidget_Expectation>(this, ExpectationWidgetClass);
	ExpectationWidget->AddToViewport();
}

void APingPongPlayerController::MoveRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APingPongPlayerController::MoveRight"));
	}

	Server_PlatformMoveRight(AxisValue);
}

void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if (Platform)
	{
		if (AxisValue != 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation"));
		}

		Platform->Server_MoveRight(AxisValue);
	}
}

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::SpawnPlatform_Implementation(TSubclassOf<APingPongPlatform> SpawnPlatformClass)
{
	Platform = (APingPongPlatform*)GetWorld()->SpawnActor<APingPongPlatform>(SpawnPlatformClass);
	if (Platform)
	{
		Platform->SetActorLocation(StartTransform.GetLocation());
		Platform->SetActorRotation(StartTransform.GetRotation());
	}
}

bool APingPongPlayerController::SpawnPlatform_Validate(TSubclassOf<APingPongPlatform> SpawnPlatformClass)
{
	return PlatformClass != NULL;
}

void APingPongPlayerController::Initialize_Implementation()
{
	if (Platform)
	{
		Platform->Destroy();
	}

	SpawnPlatform(PlatformClass);
}

bool APingPongPlayerController::Initialize_Validate()
{
	return true;
}
