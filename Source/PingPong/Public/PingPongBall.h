// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "PingPongBall.generated.h"

class USphereComponent;
class UWidget_Expectation;

UCLASS()
class PINGPONG_API APingPongBall : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* BodyCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Params")
	float MoveSpeed = 110;

	UParticleSystem* HitEffect;

	UPROPERTY(Replicated)
	bool IsMoving = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> BodyMeshRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> MaterialRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> HitEffectRef;

	TSharedPtr<FStreamableHandle> AssetHandleMesh;
	TSharedPtr<FStreamableHandle> AssetHandleEffect;
	
	UPROPERTY(Replicated)
	int Points = 1;

public:	
	APingPongBall();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartMove();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void LoadBodyMesh(); //UStaticMesh*
	void OnBodyMeshLoaded();

	UMaterial* LoadMaterial();

	void LoadHitEffect(); //UParticleSystem*
	void OnHitEffectLoaded();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartMove();

	UFUNCTION(BlueprintCallable)
	void StopMove();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	virtual void Destroyed() override;

	int GetPoints() { return Points; }
};
