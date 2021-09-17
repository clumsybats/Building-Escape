// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();//Checking for physics handle
	
	SetUpInputComponent();//checking for input component
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found in %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//physics is enabled
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle in %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineRotation = PlayerViewPointRotation.Vector();
	return PlayerViewPointLocation + (LineRotation * Reach);
}

FVector UGrabber::GetPlayerPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	return PlayerViewPointLocation;
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	UE_LOG(LogTemp, Warning, TEXT("Key Pressed"));
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Key Released"));

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If physics handle is attached then move the object we are holding 

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}

}
	
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Draw a Test Line from player showing the reach
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5
	);*/

	//Ray-cast out to a certain distance

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* HitActor = Hit.GetActor();

	//log out to unreal

	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *HitActor->GetName());
	}

	return Hit;
}
