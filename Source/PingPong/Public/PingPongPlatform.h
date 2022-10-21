// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "PingPongPlatform.generated.h"

class UBoxComponent;

UCLASS()
class PINGPONG_API APingPongPlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BodyCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> BodyMeshRef;

	TSharedPtr<FStreamableHandle> AssetHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100;

public:	
	APingPongPlatform();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void LoadBodyMesh();

	void OnBodyMeshLoaded();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);
};
