// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSWaypoint.h"

FVector AFPSWaypoint::Location()
{
    return GetActorLocation();
}

AFPSWaypoint *AFPSWaypoint::GetNextWaypoint()
{
    return Cast<AFPSWaypoint>(Next) ;
}
