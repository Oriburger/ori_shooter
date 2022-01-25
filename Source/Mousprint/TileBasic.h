// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "TileBasic.generated.h"

UCLASS()
class MOUSPRINT_API ATileBasic : public AActor
{
	GENERATED_BODY()

private:
	TArray<AActor*> SpawnedObstacleArr; //������ ��ֹ��� �ּҸ� �����ϴ� �迭

	bool OverlapFlag = false; //�÷��̾ BoxTrigger�� Overlapped �Ǿ��°�?

public:	
	// Sets default values for this actor's properties
	ATileBasic();

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 MaxObstacleCount = 5;
	
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent; //Ÿ���� ���� ����/��ġ�� ��Ÿ���� Arrow

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* BoxTrigger; //�÷��̾��� Overlapped�� �����ϴ� ����

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<TSubclassOf<class AActor> > ObstacleClassArray;

	UPROPERTY(VisibleDefaultsOnly)
		TArray<USphereComponent*> ObstacleSpawnPointArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnPoint() const; //Arrow�� Transform ��ȯ

	UFUNCTION()
		bool InitObstacle();

	UFUNCTION()
		void DestroyObstacle();

	UFUNCTION()
		bool IsOverlapped() const; //�÷��̾ BoxTrigger�� Overlapped �Ǿ��°�?

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
