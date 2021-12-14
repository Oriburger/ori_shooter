// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "TileGenerator.generated.h"

UCLASS()
class TEST_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	TArray<ATileBasic*> SpawnedTileArr; //������ Ÿ���� �ּҸ� �����ϴ� �迭

public:	
	// Sets default values for this actor's properties
	ATileGenerator();

	UPROPERTY(VisibleDefaultsOnly)
		bool bIsSpawningTile = false; //Ÿ���� ��ġ�� �����Ǵ� ���� �����ϴ� ����

	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		int32 MaxSpawnTileCnt = 5; //�ʿ� ������ �ִ� Ÿ�� ����, SpawnedTileArr�� �ִ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > TileClassArray; //������ ������ Ÿ���� BP���� ���ҷ� �������

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnTransform() const; //���� ������ Ÿ���� ��ġ�� ��ȯ

	UFUNCTION()
		ATileBasic* SpawnTile(const bool _bIsInit, const int TileIdx); //Ÿ���� ����
																	   //_bIsInit : ���� ���� �� true�� ���� 
																	   //TileIdx : ������ Ÿ�� Ŭ������ idx�� ����

	UFUNCTION()
		void DestroyTile(ATileBasic* target); //Target Ÿ���� ����
};
