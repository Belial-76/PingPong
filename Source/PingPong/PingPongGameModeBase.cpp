// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameModeBase.h"

#include "PingPongPlayerController.h"
#include "PingPongPlayerPawn.h"
#include "Widget_Expectation.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

APingPongGameModeBase::APingPongGameModeBase()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
}

void APingPongGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* World = GetWorld();
	if (World && (!Player1Start || !Player2Start))
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			Player1Start = (APlayerStart*)FoundActors[0];
		}
		if (FoundActors.Num() > 1)
		{
			Player2Start = (APlayerStart*)FoundActors[1];
		}
	}

	APingPongPlayerController* CurrPlayer = NULL;
	APlayerStart* StartPos = NULL;
	if (Player1 == NULL)
	{
		Player1 = (APingPongPlayerController*)NewPlayer;
		CurrPlayer = Player1;
		StartPos = Player1Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init Player 1"));

		Player1->Widget();
		if (!Player1->GetWidget()->IsVisible())
		{
			Player1->GetWidget()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (Player2 == NULL)
	{
		Player2 = (APingPongPlayerController*)NewPlayer;
		CurrPlayer = Player2;
		StartPos = Player2Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init Player 2"));

		if (Player1->GetWidget()->IsVisible())
		{
			Player1->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PingPongGameMode: There are already two players in the game. New connections are not possible"));
		return;
	}

	APingPongPlayerPawn* NewPawn = Cast<APingPongPlayerPawn>(NewPlayer->GetPawn());
	if (!NewPawn)
	{
		NewPawn = World->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
	}

	if (StartPos && NewPawn)
	{
		NewPawn->SetActorLocation(StartPos->GetActorLocation());
		NewPawn->SetActorRotation(StartPos->GetActorRotation());
		NewPlayer->SetPawn(NewPawn);

		CurrPlayer->SetStartTransform(StartPos->GetActorTransform());
		CurrPlayer->Initialize();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
	}
}
