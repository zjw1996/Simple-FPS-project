// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSobjectiveActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSobjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSobjectiveActor();

protected:
	UPROPERTY(VisibleAnywhere,Category = "Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* PickupFX;

	void PlayEffects();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
