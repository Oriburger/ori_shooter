// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Sound/SoundCue.h"
#include "LaserBase.generated.h"

UCLASS()
class MOUSPRINT_API ALaserBase : public AActor
{
	GENERATED_BODY()

private:
	float DamageDelay = 0.25f;

public:	
	// Sets default values for this actor's properties
	ALaserBase();

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundCue* HitSound; //�ǰ� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UArrowComponent* ArrowComponent; //������ �߻� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FHitResult GetLineTraceEndLocation();

	UFUNCTION()
		void CheckHitAndApplyDamage(FHitResult OutHit);

	UFUNCTION()
		void UpdateDamageDelay(const float DeltaTime);
};
