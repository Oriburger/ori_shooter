// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "TileGenerator.generated.h"

UCLASS()
class MOUSPRINT_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	TArray<ATileBasic*> SpawnedTileArr; //������ Ÿ���� �ּҸ� �����ϴ� �迭
	bool prevCurveTileType = 0; //0 ���� 1 ������
	bool prevTileType; //0 ����, 1 Ŀ��
	int32 prevTileIdx = 0; 
	bool bIsSpawningTile = false; //Ÿ���� ��ġ�� �����Ǵ� ���� �����ϴ� ����

public:	
	// Sets default values for this actor's properties
	ATileGenerator();

	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		int32 MaxSpawnTileCnt = 5; //�ʿ� ������ �ִ� Ÿ�� ����, SpawnedTileArr�� �ִ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > CurveTileClassArray; //������ ������ Ÿ���� BP���� ���ҷ� �������

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > StraightTileClassArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnTransform() const; //���� ������ Ÿ���� ��ġ�� ��ȯ

	UFUNCTION()
		ATileBasic* SpawnTile(const bool _bIsInit, int TileIdx, bool bIsCurve); //Ÿ���� ����
																 //_bIsInit : ���� ���� �� true�� ���� 
																 //TileIdx : ������ Ÿ�� Ŭ������ idx�� ����
		UFUNCTION()
		void DestroyTile(ATileBasic* target); //Target Ÿ���� ����
};
