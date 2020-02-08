// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaunchMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchMeshComp"));
	RootComponent = LaunchMeshComp ;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(50, 50, 30));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

	LaunchPower = 1500.0f;
	LaunchPadAngle = 35.0f;



}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();

	LaunchDirection.Pitch += LaunchPadAngle;

	FVector LaunchVelocity = LaunchDirection.Vector()* LaunchPower;

	ACharacter* MyPawnC = Cast<ACharacter>(OtherActor);

	if (MyPawnC) {
		//Launch Player , Both Booleans give consistent launch velocity by ignoring the current player velocity
		MyPawnC->LaunchCharacter(LaunchVelocity, true, true);

		//Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PushFX, GetActorLocation());
	}
	//Did not overlap a player , so check if it's a physics simulating actor we can launch
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		//Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PushFX, GetActorLocation());
	}
}


// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
