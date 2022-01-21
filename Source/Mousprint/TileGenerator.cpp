// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Tile Generator" };
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();
	SpawnedTileArr.Push(SpawnTile(true, 0, false)); //���� ���� �� �ϳ��� ����
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� �� Ÿ���� ���� MaxSpawnTileCnt���� �۰�, ���� �۾� ���� �ƴ϶��
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		bIsSpawningTile = true; //�ߺ� ���� ����
		bool bIsCurve = (FMath::RandRange(0, 100) > 90.0f);
		int32 nextTileIdx = -1;  
		ATileBasic* SpawnedTile = nullptr;

		if (bIsCurve) nextTileIdx = FMath::RandRange(1, CurveTileClassArray.Num() - 1);
		else nextTileIdx = FMath::RandRange(1, StraightTileClassArray.Num() - 1);
	
		if(nextTileIdx != prevTileIdx || bIsCurve != prevTileType)
			SpawnedTile = SpawnTile(false, nextTileIdx, bIsCurve); 
		
		if(SpawnedTile != nullptr)
			SpawnedTileArr.Push(SpawnedTile);//Spawn �� Ÿ���� Arr�� ����
		
		bIsSpawningTile = false;
	}
	if (SpawnedTileArr.IsValidIndex(10) //�÷��̾ 2��° Ÿ���� ������ ������ ��Ҵٸ�
		&& SpawnedTileArr[10]->IsOverlapped())
	{
		DestroyTile(SpawnedTileArr[0]); //0��° Ÿ���� Destroy
		SpawnedTileArr.RemoveAt(0); //�迭�κ��� ����
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (SpawnedTileArr.Num() == 0) return FTransform(); //�迭�� ����ٸ� �⺻ Transform ��ȯ
	return SpawnedTileArr.Last()->GetNextSpawnPoint(); //�� ���� Ÿ���� Arrow Transform ��ȯ
}

ATileBasic* ATileGenerator::SpawnTile(const bool _bIsInit, int TileIdx, bool bIsCurve)
{
	if (!GetWorld()) return nullptr; //Idx ������ Invalid ��� nullptr ��ȯ
	if (bIsCurve)
	{
		if (prevCurveTileType && TileIdx % 2 == 1) TileIdx -= 1;
		else if (!prevCurveTileType && TileIdx % 2 == 0) TileIdx += 1;
		prevCurveTileType = (TileIdx % 2 == 1);
	}

	TSubclassOf<class ATileBasic>& SpawnTarget = (bIsCurve ? CurveTileClassArray[TileIdx] : StraightTileClassArray[TileIdx]);
	if (SpawnTarget == nullptr) return nullptr;

	prevTileType = bIsCurve;
	prevTileIdx = TileIdx;

	//UE_LOG(LogTemp, Warning, TEXT("Tile Spawned")); //����� Log

	FRotator BeginRotation = GetNextSpawnTransform().GetRotation().Rotator();
	FVector BeginLocation;	
	if (_bIsInit) BeginLocation = FVector(0, 0, 0); //���� ���� �� �ʱ� ��ġ�� {0,0,0}
	else BeginLocation = GetNextSpawnTransform().GetLocation(); 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Ÿ���� �����ڴ� Generator
	SpawnParams.Instigator = GetInstigator(); 

	//Ÿ���� �����ϰ� ��ȯ
	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(SpawnTarget, BeginLocation, BeginRotation);
	
	return NewTile;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target�� null �̶�� Destroy ���� ����
	target->Destroy();
}