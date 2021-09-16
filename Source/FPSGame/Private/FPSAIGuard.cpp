// Fill out your copyright notice in the Description page of Project Settings.
#include "FPSAIGuard.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include <FPSWaypoint.h>

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
    PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
    GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
    Super::BeginPlay();
    OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn *SeenPawn)
{
    if (SeenPawn == nullptr) {
        UE_LOG(LogTemp, Log, TEXT("Seen nothing"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Seen something"));
    DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 3.0f);
    AFPSGameMode *GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GM) {
        GM->CompleteMission(SeenPawn, false);
    }
    SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn *NoiseInstigator, const FVector &Location, float Volume)
{
    if (GuardState == EAIState::Alerted) {
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Heard something"));
    DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 3.0f);
    FVector Direction = Location - GetActorLocation();
    Direction.Normalize();
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0.0f;
    NewLookAt.Roll = 0.0f;
    SetActorRotation(NewLookAt, ETeleportType::TeleportPhysics);
    GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
    GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 5.0f);
    SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
    if (GuardState == EAIState::Alerted) {
        return;
    }
    SetActorRotation(OriginalRotation);
    SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
    if (GuardState == NewState) {
        return;
    }
    GuardState = NewState;
    OnStateChanged(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
    if (Target && GuardState == EAIState::Idle) {
        MarkTarget();
        LookAtTarget();
        MoveForward();
        ChooseNewTarget();
    }

    Super::Tick(DeltaTime);
}

void AFPSAIGuard::MoveForward()
{
    AddMovementInput(GetActorForwardVector(), 0.5f);
}

void AFPSAIGuard::ChooseNewTarget()
{
    FVector TargetLocation = Target->GetActorLocation();
    FVector TargetDirection = TargetLocation - GetActorLocation();

    AFPSWaypoint *NextTarget = Target->GetNextWaypoint();
    FVector NextTargetLocation = NextTarget->GetActorLocation();
    FVector NextTargetDirection = NextTargetLocation - GetActorLocation();

    float distanceToTarget = TargetDirection.Size();
    float distanceToNextTarget = NextTargetDirection.Size();

    bool nextTargetIsFurtherAway = distanceToNextTarget > distanceToTarget;

    if (TargetDirection.Size() < 100.0f && nextTargetIsFurtherAway) {
        Target = Target->GetNextWaypoint();
        UE_LOG(LogTemp, Log, TEXT("Moving to new target"));
    }
}

void AFPSAIGuard::LookAtTarget()
{
    FVector TargetLocation = Target->GetActorLocation();
    FVector TargetDirection = TargetLocation - GetActorLocation();

    FRotator NewLookAt = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
    NewLookAt.Pitch = 0.0f;
    NewLookAt.Roll = 0.0f;
    SetActorRotation(NewLookAt, ETeleportType::TeleportPhysics);
}

void AFPSAIGuard::MarkTarget()
{
    FVector TargetLocation = Target->GetActorLocation();

    DrawDebugSphere(GetWorld(), TargetLocation, 32.0f, 12, FColor::Red, false, 3.0f);
}
