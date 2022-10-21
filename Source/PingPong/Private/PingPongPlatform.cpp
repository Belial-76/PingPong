// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"

APingPongPlatform::APingPongPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Platform Body Collider"));
	SetRootComponent(BodyCollision);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();

	LoadBodyMesh();
}

void APingPongPlatform::LoadBodyMesh()
{
	FStreamableDelegate LoadMeshDelegate;
	LoadMeshDelegate.BindUObject(this, &APingPongPlatform::OnBodyMeshLoaded);

	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();

	AssetHandle = StreamableManager.RequestAsyncLoad(BodyMeshRef.ToStringReference(), LoadMeshDelegate);
}

void APingPongPlatform::OnBodyMeshLoaded()
{
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset());
	if (LoadedMesh)
	{
		BodyMesh->SetStaticMesh(LoadedMesh);
	}
}

void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	if (AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("APingPongPlatform::Server_MoveRight_Implementation"));
	}

	FVector CurrLocation = GetActorLocation();
	FVector NextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;
	if (!SetActorLocation(NextLocation, true))
	{
		SetActorLocation(CurrLocation);
	}
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}

