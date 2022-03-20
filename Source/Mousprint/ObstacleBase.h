// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "ObstacleBase.generated.h"

UCLASS()
class MOUSPRINT_API AObstacleBase : public AActor
{
	GENERATED_BODY()

private:
	const float DefaultObstacleMoveSpeed = 3.0f; //��ֹ��� �⺻ �̵� �ӵ�
	const int32 MaxObstacleCnt = 10; //�� ��ֹ� ����
	TArray<bool> bIsArrived; //�ι�° ������ ������ �ߴ��� ����

public:	
	// Sets default values for this actor's properties
	AObstacleBase();
	
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<UChildActorComponent*> ObstacleComponentArray; //��ֹ� ������Ʈ

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 UseObstacleCount = 5; //������ ����� ��ֹ��� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<bool> bIsMovingObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<float> ObstacleMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TArray<USphereComponent*> PathComponentArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void UpdateObstacleLocation();

	UFUNCTION()
		void MoveObstacle(const int idx, const int target_idx);
};
