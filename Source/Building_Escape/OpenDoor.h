// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	float InitialYaw;	//Initial Z-Angle of Door
	float CurrentYaw;	//Current Z-Angle of Door
	
	UPROPERTY(EditAnywhere)
		float TargetYaw = 90.f;	  //Target Z-Angle of Door

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
		float CloseDoorTime=0.5f;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 4.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoors = 50.f;
};
