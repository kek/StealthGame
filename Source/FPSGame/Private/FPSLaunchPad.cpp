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

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
    DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
    DecalComp->SetupAttachment(RootComponent);
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
    AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

    if (MyPawn == nullptr) {
        return;
    } else {
    }

    UE_LOG(LogTemp, Log, TEXT("Overlapped with launch pad."));
}