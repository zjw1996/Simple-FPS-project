// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MulticatastOnMissionComplete_Implementation(APawn* Instigator, bool bMissionSuccess)
{

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++) {
		APawn* Pawn = It->Get();
		if (Pawn && Pawn->IsLocallyControlled()) {
			Pawn->DisableInput(nullptr);
		}
	}
}
