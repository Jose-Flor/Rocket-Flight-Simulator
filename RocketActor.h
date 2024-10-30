
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketActor.generated.h"

UCLASS()
class YOURPROJECT_API ARocketActor : public AActor
{
    GENERATED_BODY()

public:
    ARocketActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components and Rocket Mesh
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* RocketMesh;

    // Basic Rocket Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float Stage1Thrust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float Stage2Thrust;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float FuelAmountStage1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float FuelAmountStage2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float TotalMass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float GravityConstant;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Physics")
    float EscapeVelocity;

    // Additional Rocket Properties
    float DragCoefficient;
    float AtmosphereDensity;
    float CurrentAltitude;
    float OrbitalSpeed;

    bool bIsLaunching;
    bool bStage2Activated;
    bool bInOrbit;

    // Heat, Failure, and Environmental Properties
    float MaxTemperature;
    float CurrentTemperature;
    float ReentryDragMultiplier;
    bool bHasFailed;
    bool bHeatShieldDeployed;

    // Autopilot
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Control")
    bool bAutopilotEnabled;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rocket Control")
    float TargetOrbitAltitude;

    // Methods
    void Launch();
    void StageSeparation();
    void ApplyThrust(float DeltaTime);
    void ApplyDrag();
    void CalculateOrbitalVelocity();
    void AutoPilot(float DeltaTime);
    void ApplyGimbalControl();
    void UpdateMassLoss(float DeltaTime);
    void AtmosphericEffects(float DeltaTime);
    void CheckForFailures();

private:
    float CurrentThrust;
    float CurrentFuel;
};
