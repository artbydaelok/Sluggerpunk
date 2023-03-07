// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingTextActor.h"

// Sets default values
AFloatingTextActor::AFloatingTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFloatingTextActor::BeginPlay()
{
	Super::BeginPlay();
	
	AnchorLocation = GetActorLocation();
}

// Called every frame
void AFloatingTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

