// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "FPSWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSWaypoint : public ATargetPoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditInstanceOnly, Category = "AI")
    ATargetPoint *Next;
};
