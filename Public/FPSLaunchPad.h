// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UParticleSystem;
UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* LaunchMeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* PushFX;
	UPROPERTY(BlueprintReadWrite, Category = "Speed")
		float LaunchPower;
	UPROPERTY(BlueprintReadWrite, Category = "Speed")
		float LaunchPadAngle;
	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
