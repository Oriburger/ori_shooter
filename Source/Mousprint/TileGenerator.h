// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "Containers/Array.h"
#include "TileGenerator.generated.h"

UCLASS()
class MOUSPRINT_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	TArray<ATileBasic*> SpawnedTileArr; //������ Ÿ���� �ּҸ� �����ϴ� �迭
	int32 Stage = 0;
	int32 StraightTileMinIdx = 0; //������ Ÿ�� ������ ���� Idx
	int32 StraightTileMaxIdx = 0; //������ Ÿ�� ������ �� Idx
	int32 CurveTileMinIdx = 0; //������ Ÿ�� ������ ���� Idx
	int32 CurveTileMaxIdx = 0; //������ Ÿ�� ������ �� Idx

	bool bIsStageChanged = false;
	bool prevCurveTileType = 0; //0 ���� 1 ������
	bool prevTileType; //������ ������ Ÿ�� ���� - 0 ����, 1 Ŀ��
	int32 prevTileIdx = -1; //������ ������ Ÿ���� Idx

	bool bIsSpawningTile = false; //Ÿ���� ��ġ�� �����Ǵ� ���� �����ϴ� ����


public:	
	// Sets default values for this actor's properties
	ATileGenerator();
	
	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		float CurveTileSpawnPercentage = 10.0f;

	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		int32 MaxSpawnTileCnt = 5; //�ʿ� ������ �ִ� Ÿ�� ����, SpawnedTileArr�� �ִ� ũ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > TileClassArray; //������ ������ Ÿ���� BP���� ���ҷ� �������

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > BeginTileClassArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnNextTile();

	UFUNCTION()
		void DestroyHeadTile();

	UFUNCTION()
		FTransform GetNextSpawnTransform() const; //���� ������ Ÿ���� ��ġ�� ��ȯ

	UFUNCTION()
		int32 GetNextSpawnTileIdx();

	UFUNCTION()
		ATileBasic* SpawnTile(TSubclassOf<class ATileBasic>& SpawnTarget); //Ÿ���� ����
																 //_bIsInit : ���� ���� �� true�� ���� 
																 //TileIdx : ������ Ÿ�� Ŭ������ idx�� ����
	UFUNCTION(BlueprintCallable)
		bool SetSpawnTileIdxRange(const int32 stage_, const int32 start_straight, const int32 finish_straight
								, const int32 start_curve, const int32 finish_curve);

	UFUNCTION()
		void DestroyTile(ATileBasic* target); //Target Ÿ���� ����
};
