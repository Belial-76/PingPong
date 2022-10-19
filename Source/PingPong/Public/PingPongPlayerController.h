// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

class APingPongPlatform;
class UWidget_Expectation;

UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FTransform StartTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APingPongPlatform> PlatformClass;

	UPROPERTY()
	APingPongPlatform* Platform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidget_Expectation> ExpectationWidgetClass;
	UPROPERTY()
	UWidget_Expectation* ExpectationWidget;

public:
	APingPongPlayerController();

	UFUNCTION()
	void SetStartTransform(FTransform NewStartTransform);

	UFUNCTION(Server, Reliable, WithValidation)
	void Initialize();

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatform(TSubclassOf<APingPongPlatform> SpawnPlatformClass);

	virtual void SetupInputComponent() override;

	void Widget();
	UWidget_Expectation* GetWidget() { return ExpectationWidget; }

protected:
	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlatformMoveRight(float AxisValue);
};
