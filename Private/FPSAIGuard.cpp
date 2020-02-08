// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "NET/UnrealNetwork.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bPatrol = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);

	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnPawnHear);



}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();
	if(bPatrol)
		MoveToNextPatrolPoint();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(),32.0f,12,FColor::Red,false,10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->CompleteMission(SeenPawn, false);
	}

	StopAI();
}

void AFPSAIGuard::OnPawnHear(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Black, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation,this,&AFPSAIGuard::ResetOrientation,3.0f);

	StopAI();

}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
	if (bPatrol) {
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void AFPSAIGuard::StopAI()
{
	AController* AIControll = GetController();
	if (AIControll)
		AIControll->StopMovement();
}

void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentPatrolPoint) {
		FVector Delte = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delte.Size();
		if (DistanceToGoal <50.0f)
		{
			MoveToNextPatrolPoint();
		}
	}
}

