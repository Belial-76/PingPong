// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGates.h"

#include "PingPongBall.h"
#include "PingPongPlayerPawn.h"
#include "Components/BoxComponent.h"

APingPongGates::APingPongGates()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Gates Body Collision"));
	SetRootComponent(BodyCollision);
	BodyCollision->OnComponentBeginOverlap.AddDynamic(this, &APingPongGates::OnBeginOverlap);

	LBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Body Mesh"));
	LBodyMesh->SetupAttachment(RootComponent);

	RBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Body Mesh"));
	RBodyMesh->SetupAttachment(RootComponent);
}

void APingPongGates::BeginPlay()
{
	Super::BeginPlay();
	
}

void APingPongGates::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongGates::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if (APingPongBall* Ball = Cast<APingPongBall>(Actor))
	{
		Points = Ball->Points;
		Ball->Destroyed();
	}
}