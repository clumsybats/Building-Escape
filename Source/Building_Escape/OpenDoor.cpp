// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenDoor component on %s actor is there but no PressurePlate"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (TotalMassOfActors() > MassToOpenDoors) //Twice to not crash
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (((GetWorld()->GetTimeSeconds()) - DoorLastOpened)>=CloseDoorTime)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{

	/*UE_LOG(LogTemp, Warning, TEXT ("%s"),*GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);*/
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed); //DeltaTime is used to remove fps locking
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed); //DeltaTime is used to remove fps locking
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}