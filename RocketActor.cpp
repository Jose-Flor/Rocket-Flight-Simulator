
#include "RocketActor.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

ARocketActor::ARocketActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
    RootComponent = RocketMesh;

    // Initialize Thrust and Fuel
    Stage1Thrust = 8000.0f;
    Stage2Thrust = 12000.0f;
    FuelAmountStage1 = 100.0f;
    FuelAmountStage2 = 80.0f;
    TotalMass = 5000.0f;

    GravityConstant = 9.8f;
    EscapeVelocity = 11200.0f; // Escape velocity for Earth in m/s
    MaxTemperature = 1500.0f;
    CurrentTemperature = 0.0f;
    DragCoefficient = 0.02f;
    AtmosphereDensity = 1.2f;
    ReentryDragMultiplier = 5.0f;

    bIsLaunching = false;
    bStage2Activated = false;
    bInOrbit = false;
    bAutopilotEnabled = true;
    TargetOrbitAltitude = 200000.0f; // Example target orbit altitude in meters
    bHeatShieldDeployed = false;
    bHasFailed = false;
}

void ARocketActor::BeginPlay()
{
    Super::BeginPlay();
}

void ARocketActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsLaunching && CurrentFuel > 0.0f && !bHasFailed)
    {
        ApplyThrust(DeltaTime);
        ApplyDrag();
        UpdateMassLoss(DeltaTime);
        AtmosphericEffects(DeltaTime);

        if (bAutopilotEnabled)
            AutoPilot(DeltaTime);

        CheckForFailures();
    }

    if (CurrentFuel <= 0 && !bStage2Activated)
    {
        StageSeparation();
    }

    CalculateOrbitalVelocity();
}

void ARocketActor::Launch()
{
    bIsLaunching = true;
}

void ARocketActor::StageSeparation()
{
    bStage2Activated = true;
    CurrentThrust = Stage2Thrust;
    CurrentFuel = FuelAmountStage2;
}

void ARocketActor::ApplyThrust(float DeltaTime)
{
    FVector ThrustVector = FVector(0, 0, 1) * CurrentThrust;
    RocketMesh->AddForce(ThrustVector);
    float FuelConsumptionRate = bStage2Activated ? 0.7f : 1.0f;
    CurrentFuel -= FuelConsumptionRate * DeltaTime;
}

void ARocketActor::ApplyDrag()
{
    float Altitude = GetActorLocation().Z;
    AtmosphereDensity = FMath::Exp(-Altitude / 10000.0f);
    FVector Velocity = RocketMesh->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * AtmosphereDensity * Velocity.SizeSquared() * Velocity.GetSafeNormal();

    RocketMesh->AddForce(DragForce);
}

void ARocketActor::CalculateOrbitalVelocity()
{
    FVector Velocity = RocketMesh->GetPhysicsLinearVelocity();
    float Speed = Velocity.Size();

    if (Speed >= EscapeVelocity)
    {
        bInOrbit = true;
        bAutopilotEnabled = false;
    }
}

void ARocketActor::AutoPilot(float DeltaTime)
{
    if (CurrentAltitude < TargetOrbitAltitude)
    {
        FVector UpVector = FVector(0, 0, 1);
        FVector AdjustmentForce = UpVector * 500.0f; 

        RocketMesh->AddForce(AdjustmentForce);
    }
    else
    {
        bAutopilotEnabled = false;
    }
}

void ARocketActor::ApplyGimbalControl()
{
    FVector AdjustDirection = FVector(0, 0, 1).RotateAngleAxis(5.0f, FVector::UpVector); // Gimbal angle of 5 degrees
    FVector GimbalForce = AdjustDirection * CurrentThrust;

    RocketMesh->AddForce(GimbalForce);
}

void ARocketActor::UpdateMassLoss(float DeltaTime)
{
    float MassLoss = CurrentFuel * 0.05f * DeltaTime; // Example mass loss rate
    TotalMass -= MassLoss;
    RocketMesh->SetMassOverrideInKg(NAME_None, TotalMass);
}

void ARocketActor::AtmosphericEffects(float DeltaTime)
{
    if (GetActorLocation().Z < 20000.0f && RocketMesh->GetPhysicsLinearVelocity().Size() > 2000.0f) // Example re-entry conditions
    {
        bHeatShieldDeployed = true;
        CurrentTemperature += ReentryDragMultiplier * DeltaTime;

        if (CurrentTemperature >= MaxTemperature)
        {
            bHasFailed = true;
            UE_LOG(LogTemp, Warning, TEXT("Rocket Overheated and Failed!"));
        }
    }
}

void ARocketActor::CheckForFailures()
{
    float FailureChance = FMath::RandRange(0.0f, 1.0f);

    if (FailureChance < 0.01f) // 1% chance for a failure
    {
        bHasFailed = true;
        UE_LOG(LogTemp, Warning, TEXT("Rocket experienced a critical failure!"));
    }
}
