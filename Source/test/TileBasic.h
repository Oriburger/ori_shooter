// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "TileBasic.generated.h"

UCLASS()
class TEST_API ATileBasic : public AActor
{
	GENERATED_BODY()

private:
	bool OverlapFlag = false; //�÷��̾ BoxTrigger�� Overlapped �Ǿ��°�?
	const int32 ObstacleCount = 5;
	//static int SpawnedTileCnt = 0;

public:	
	// Sets default values for this actor's properties
	ATileBasic();
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent; //Ÿ���� ���� ����/��ġ�� ��Ÿ���� Arrow

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* BoxTrigger; //�÷��̾��� Overlapped�� �����ϴ� ����

	UPROPERTY(VisibleDefaultsOnly)
		TArray<UBoxComponent*> ObstacleSpawnPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnPoint() const; //Arrow�� Transform ��ȯ
	
	UFUNCTION()
		bool IsOverlapped() const; //�÷��̾ BoxTrigger�� Overlapped �Ǿ��°�?

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
