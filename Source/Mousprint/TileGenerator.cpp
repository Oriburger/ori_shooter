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
	
	ATileBasic* InitialTile = SpawnTile(BeginTileClassArray[Stage - 1]);
	if (InitialTile) SpawnedTileArr.Push(InitialTile); //���� ���� �� �ϳ��� ����
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� �� Ÿ���� ���� MaxSpawnTileCnt���� �۰�, ���� �۾� ���� �ƴ϶��
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Tick!"));
		//bIsSpawningTile = true; //�ߺ� ���� ����

		ATileBasic* SpawnedTile = nullptr;
		int32 nextTileIdx = GetNextSpawnTileIdx();  
	
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : next tile idx is %d"), nextTileIdx);

		if (nextTileIdx != -1 && TileClassArray.IsValidIndex(nextTileIdx))
		{
			SpawnedTile = SpawnTile(TileClassArray[nextTileIdx]);
			if(SpawnedTile != nullptr)	SpawnedTileArr.Push(SpawnedTile);//Spawn �� Ÿ���� Arr�� ����
		}
		//bIsSpawningTile = false;
	}

	if ((SpawnedTileArr.IsValidIndex(10) && SpawnedTileArr[10]->IsOverlapped()) //�÷��̾ 10��° Ÿ���� ������ ������ ��Ҵٸ�
		|| (SpawnedTileArr.IsValidIndex(11) && SpawnedTileArr[11]->IsOverlapped()))
	{
		ATileBasic* DestoyTarget = SpawnedTileArr[0];
		if (DestoyTarget != nullptr && IsValid(DestoyTarget))
		{
			UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Tile Removed"));
			DestoyTarget->DestroyObstacle();
			DestoyTarget->Destroy();  //0��° Ÿ���� Destroy
		}
		SpawnedTileArr.RemoveAt(0); //�迭�κ��� ����
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (SpawnedTileArr.Num() == 0) return FTransform(); //�迭�� ����ٸ� �⺻ Transform ��ȯ
	return SpawnedTileArr.Last()->GetNextSpawnPoint(); //�� ���� Ÿ���� Arrow Transform ��ȯ
}

int32 ATileGenerator::GetNextSpawnTileIdx() 
{
	int32 nextTileIdx = -1;

	/*--- ���� ������ Ÿ���� Idx�� ������ �������� ���� ---*/
	bool nextTileType = (FMath::RandRange(0, 100) < CurveTileSpawnPercentage);

	if (nextTileType)
	{
		do
		{
			nextTileIdx = FMath::RandRange(CurveTileMinIdx, CurveTileMaxIdx); //�� ���� Ÿ���� ����
		} while (prevTileType == nextTileType && prevTileIdx == nextTileIdx);
	}
	else
	{
		do
		{
			nextTileIdx = FMath::RandRange(StraightTileMinIdx, StraightTileMaxIdx); //�� ���� Ÿ���� ����
		} while (prevTileType == nextTileType && prevTileIdx == nextTileIdx);
	}

	/*--- � Ÿ�Ϸ� ���� Ÿ�ϵ��� ��ġ�� ���� ���� ---*/
	if (nextTileType)
	{
		if (prevCurveTileType && nextTileIdx % 2 == 1) nextTileIdx -= 1;
		else if (!prevCurveTileType && nextTileIdx % 2 == 0) nextTileIdx += 1;
		prevCurveTileType = (nextTileIdx % 2 == 1);
	}

	prevTileType = nextTileType;
	prevTileIdx = nextTileIdx; 

	return nextTileIdx;
}

ATileBasic* ATileGenerator::SpawnTile(TSubclassOf<class ATileBasic>& SpawnTarget)
{
	if (!GetWorld()) return nullptr; 
	if (SpawnTarget == nullptr || !IsValid(SpawnTarget)) return nullptr;	

	//SpawnTransform �� ���� ����
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetNextSpawnTransform().GetLocation());
	SpawnTransform.SetRotation(GetNextSpawnTransform().GetRotation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Ÿ���� �����ڴ� Generator
	SpawnParams.Instigator = GetInstigator(); 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Ÿ���� �����ϰ� ��ȯ (SpawnActorDeferred �Լ� : BeginPlay ȣ���� ������Ŵ)
	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(SpawnTarget, SpawnTransform, SpawnParams);
	
	return NewTile;
}

bool ATileGenerator::SetSpawnTileIdxRange(const int32 start_straight, const int32 finish_straight
										, const int32 start_curve, const int32 finish_curve)
{
	if (start_straight > finish_straight || start_curve > finish_curve) return false;

	StraightTileMinIdx = start_straight;
	StraightTileMaxIdx = finish_straight;
	CurveTileMinIdx = start_curve;
	CurveTileMaxIdx = finish_curve;

	return true;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target�� null �̶�� Destroy ���� ����
	target->Destroy();
}