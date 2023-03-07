// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingDamageNumberComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "FloatingTextActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFloatingDamageNumberComponent::UFloatingDamageNumberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloatingDamageNumberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFloatingDamageNumberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	// check if viewport size is bad
	if (viewportSize.X <= 0.0f || viewportSize.Y <= 0.0f)
		return;

	// Get the viewport scale (DPI scale)
	const int32 viewportX = FGenericPlatformMath::FloorToInt(viewportSize.X);
	const int32 viewportY = FGenericPlatformMath::FloorToInt(viewportSize.Y);
	const float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportX, viewportY));
	
	if (viewportScale <= 0.0f)
		return;
	
	// get the owner (player) camera location
	const FVector cameraLocation = GetOwner<APawn>()->GetController<APlayerController>()->PlayerCameraManager->GetCameraLocation();

	// Adjuting location of damage number location by the viewport size and scale so that they can stack
	for (int32 i = 1; i < ActiveTextActors.Num(); ++i)
	{
		// Calculate the vertical offset based on how far the camera is from the floating actor
		AFloatingTextActor* textActor = ActiveTextActors[i];
		const float distance = FVector::Dist(cameraLocation, textActor->GetAnchorLocation());
		const float verticalOffset = distance / (viewportSize.X / viewportScale);

		//Adjust the floating text actor's location by the calculated amount
		textActor->SetActorLocation(textActor->GetAnchorLocation() + FVector(0.0f, 0.0f, i * verticalOffset * TextVerticalSpacing));
	}
}

void UFloatingDamageNumberComponent::AddFloatingText(const FText& text, FVector world_location)
{
	if (FloatingTextActorClass == nullptr)
		return;

	const FTransform spawnTransform = FTransform(FRotator::ZeroRotator, world_location + FVector(0.0f, 0.0f, TextVerticalOffset));
	AFloatingTextActor* newTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, spawnTransform, GetOwner());

	if (newTextActor == nullptr)
		return;

	//Initialize and finish spawning the actor
	newTextActor->Initialize(text);
	newTextActor->OnDestroyed.AddDynamic(this, &UFloatingDamageNumberComponent::OnTextDestroyed);
	UGameplayStatics::FinishSpawningActor(newTextActor, spawnTransform);

	//Add new damage number to beginning of the array (index 0)
	ActiveTextActors.Insert(newTextActor, 0);
}

void UFloatingDamageNumberComponent::OnTextDestroyed(AActor* destroyed_actor)
{
	ActiveTextActors.Pop();
}