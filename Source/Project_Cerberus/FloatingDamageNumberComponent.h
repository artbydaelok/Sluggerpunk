// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingDamageNumberComponent.generated.h"

class AFloatingTextActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_CERBERUS_API UFloatingDamageNumberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFloatingDamageNumberComponent();

private:
	// the floating combat text actor to use to display the text
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingDamageNumberComponent", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFloatingTextActor> FloatingTextActorClass = nullptr; 

	// vertical space between numbers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingDamageNumberComponent", Meta = (AllowPrivateAccess = "true"))
	float TextVerticalSpacing = 40.0f;

	// amount of offset to apply when spawning a new text actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingDamageNumberComponent", Meta = (AllowPrivateAccess = "true"))
	float TextVerticalOffset = 100.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Adds the specified text at the specified location in world space
	UFUNCTION(BlueprintCallable, Category = "FloatingDamageNumberComponent")
	void AddFloatingText(const FText& text, FVector world_location);

private:
	// callback for when floating actor is destroyed
	UFUNCTION()
		void OnTextDestroyed(AActor* destroyed_actor);

	// An array of all active floating text/numbers
	UPROPERTY()
		TArray<AFloatingTextActor*> ActiveTextActors;
};

