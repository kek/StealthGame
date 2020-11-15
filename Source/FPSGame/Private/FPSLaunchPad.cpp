// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
    OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
    OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    OverlapComp->SetBoxExtent(FVector(200.0f));
    RootComponent = OverlapComp;

    OverlapComp->SetHiddenInGame(true, true);
    UE_LOG(LogTemp, Log, TEXT("Initializing AFPSLaunchPad!"));

    OverlapComp->OnComponentBeginOverlap.AddDynamic(this,
        &AFPSLaunchPad::HandleOverlap);
}

void AFPSLaunchPad::HandleOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("AFPSLaunchPad HandleOverlap."));
    
    const FVector ForwardDir = GetActorRotation().Vector();
    const FVector TotalForce = ForwardDir * ForwardForce + FVector(0, 0, 1) * UpForce;
    ACharacter* MyPawn = Cast<ACharacter>(OtherActor);
    if (MyPawn) {
        MyPawn->LaunchCharacter(TotalForce, true, true);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchpadEffect, GetActorLocation());
    } else if (OtherComp && OtherComp->IsSimulatingPhysics()) {
        UE_LOG(LogTemp, Log, TEXT("othercomp is simulating physics."));
        OtherComp->AddImpulse(TotalForce, NAME_None, true);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchpadEffect, GetActorLocation());
    } else {
        UE_LOG(LogTemp, Log, TEXT("othercomp is not simulating physics."));
    }

    UE_LOG(LogTemp, Log, TEXT("Overlapped with launch pad."));
}