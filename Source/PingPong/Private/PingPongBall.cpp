// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"

#include "DrawDebugHelpers.h"
#include "PingPongPlayerController.h"
#include "Widget_Expectation.h"
#include "Components/SphereComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "PingPong/PingPongGameModeBase.h"

APingPongBall::APingPongBall()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball Body Collider"));
	SetRootComponent(BodyCollision);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void APingPongBall::BeginPlay()
{
	Super::BeginPlay();

	BodyMesh->SetStaticMesh(LoadBodyMesh());
	BodyMesh->SetMaterial(0, LoadMaterial());
}

void APingPongBall::Server_StartMove_Implementation()
{
	IsMoving = true;
}

bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}

void APingPongBall::Server_Move_Implementation(float DeltaTime)
{
	FVector Forward = GetActorForwardVector();
	FVector CurrLoc = GetActorLocation();
	FVector NewLoc = CurrLoc + Forward * MoveSpeed * DeltaTime;
	FHitResult HitResult;
	if (!SetActorLocation(NewLoc, true, &HitResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball %s Collided with %s"), *GetName(), *HitResult.GetActor()->GetName());

		FVector MoveVector = Forward - CurrLoc;
		MoveVector.Normalize();
		FVector ResetPosition = CurrLoc + MoveVector * DeltaTime * 5 * MoveSpeed;
		DrawDebugDirectionalArrow(GetWorld(), NewLoc + MoveVector * 300, NewLoc, 30, FColor::Yellow, true, 3.f, 0, 3);

		FVector ImpactCorrectio = HitResult.ImpactPoint + HitResult.ImpactNormal * 300;
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + HitResult.ImpactNormal * 300, 30, FColor::Blue, true, 3, 0, 3);

		float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(MoveVector, HitResult.ImpactNormal)));

		MoveVector = MoveVector.RotateAngleAxis(AimAtAngle * 2, FVector(0, 0, 1));
		FVector NewTargetMove = NewLoc + MoveVector * 300;
		NewTargetMove.Z = CurrLoc.Z;
		DrawDebugDirectionalArrow(GetWorld(), NewLoc, NewTargetMove, 30, FColor::Yellow, true, 3.f, 0, 3);

		//SetActorLocation(CurrLoc);
		SetActorLocation(ResetPosition);
		FRotator CurrRotation = GetActorRotation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrLoc, NewTargetMove);
		NewRotation.Pitch = CurrRotation.Pitch;
		//NewRotation.Yaw += FMath::RandRange(-10, 10);
		SetActorRotation(NewRotation);

		Multicast_HitEffect();
	}
}

bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}

void APingPongBall::Server_StopMove_Implementation()
{
	IsMoving = false;
}

bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}

void APingPongBall::Multicast_HitEffect_Implementation()
{
	UWorld* World = GetWorld();
	if (World && LoadHitEffect())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LoadHitEffect(), GetActorLocation());

		Points++;
		MoveSpeed += 10;
	}
}

UStaticMesh* APingPongBall::LoadBodyMesh()
{
	if (BodyMeshRef.IsPending())
	{
		const FSoftObjectPath& AssetRef = BodyMeshRef.ToStringReference();

		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		BodyMeshRef = Cast<UStaticMesh>(StreamableManager.LoadSynchronous(AssetRef));
	}

	return BodyMeshRef.Get();
}

UMaterial* APingPongBall::LoadMaterial()
{
	if (MaterialRef.IsPending())
	{
		const FSoftObjectPath& AssetRef = MaterialRef.ToStringReference();

		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		MaterialRef = Cast<UMaterial>(StreamableManager.LoadSynchronous(AssetRef));
	}

	return MaterialRef.Get();
}

UParticleSystem* APingPongBall::LoadHitEffect()
{
	UParticleSystem* HitEffect = LoadObject<UParticleSystem>(NULL, TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"), NULL, LOAD_None, NULL);

	return HitEffect;
}

void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() != ENetMode::NM_Client)
	{
		if (GetWorld()->GetAuthGameMode<APingPongGameModeBase>()->GetPlayer2() != NULL)
		{
			Server_Move(DeltaTime);
		}
	}
}

void APingPongBall::StartMove()
{
	Server_StartMove();
}

void APingPongBall::StopMove()
{
	Server_StopMove();
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongBall, IsMoving);
}

void APingPongBall::Destroyed()
{
	Super::Destroyed();

	MoveSpeed -= (Points - 1) * 10;
	Points = 1;
	Destroy();
}

