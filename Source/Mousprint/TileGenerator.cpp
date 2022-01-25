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
	if (InitialTile)
	{
		InitialTile->FinishSpawning(GetNextSpawnTransform());
		SpawnedTileArr.Push(InitialTile); //���� ���� �� �ϳ��� ����
	}
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� �� Ÿ���� ���� MaxSpawnTileCnt���� �۰�, ���� �۾� ���� �ƴ϶��
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Tick!"));
		bIsSpawningTile = true; //�ߺ� ���� ����

		ATileBasic* SpawnedTile = nullptr;
		int32 nextTileIdx = GetNextSpawnTileIdx();  
		FTileSpawnInfoTableRow * TileSpawnInfoRow = nullptr;
	
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : next tile idx is %d"), nextTileIdx);

		if (nextTileIdx != -1)
		{
			if(prevTileType && CurveTileClassArray.IsValidIndex(nextTileIdx))
				SpawnedTile = SpawnTile(CurveTileClassArray[nextTileIdx]);
			else if(!prevTileType && StraightTileClassArray.IsValidIndex(nextTileIdx))
				SpawnedTile = SpawnTile(StraightTileClassArray[nextTileIdx]);

			if (SpawnedTile != nullptr)
			{
				if (TileSpawnInfoTable != nullptr)
				{
					TileSpawnInfoRow = TileSpawnInfoTable->FindRow<FTileSpawnInfoTableRow>
						(FName(*(FString::FormatAsNumber(nextTileIdx))), FString(""));

					if (TileSpawnInfoRow != nullptr)
						SpawnedTile->SetTileSpawnInfo(TileSpawnInfoRow->TotalObstacleCount, TileSpawnInfoRow->MaxSpawnObstacleCount
							, TileSpawnInfoRow->SpawnPercentage_Obstacle);
				}
				SpawnedTile->FinishSpawning(GetNextSpawnTransform());
				SpawnedTileArr.Push(SpawnedTile);//Spawn �� Ÿ���� Arr�� ����
			}
		}

		bIsSpawningTile = false;
	}

	if (SpawnedTileArr.IsValidIndex(10) //�÷��̾ 2��° Ÿ���� ������ ������ ��Ҵٸ�
		&& SpawnedTileArr[10]->IsOverlapped())
	{
		ATileBasic* DestoyTarget = SpawnedTileArr[0];
		if (DestoyTarget != nullptr && IsValid(DestoyTarget))
		{
			DestoyTarget->DestroyObstacle(); //0��° Ÿ���� Destroy
			DestoyTarget->Destroy();
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
	FTileSpawnInfoTableRow* TileSpawnInfoRow = nullptr;

	/*--- ���� ������ Ÿ���� Idx�� ������ �������� ���� ---*/
	bool nextTileType = (FMath::RandRange(0, 100) < CurveTileSpawnPercentage);

	if (nextTileType) nextTileIdx = FMath::RandRange(SpawnTileMinIdx, SpawnTileMaxIdx);
	else nextTileIdx = FMath::RandRange(SpawnTileMinIdx + 1, SpawnTileMaxIdx); //�� ���� Ÿ���� ����

	if (nextTileIdx == prevTileIdx && nextTileType == prevTileType) return -1;

	/*--- ���� ������ Ÿ���� ���� Ȯ���� ��� (DB�κ��� ����) ---*/
	if (TileSpawnInfoTable != nullptr)
	{
		TileSpawnInfoRow = TileSpawnInfoTable->FindRow<FTileSpawnInfoTableRow>
			(FName(*(FString::FormatAsNumber(nextTileIdx))), FString(""));

		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : TileSpawnPercentage is %lf"), TileSpawnInfoRow->SpawnPercentage_Curve);
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : TileSpawnPercentage is %lf"), TileSpawnInfoRow->SpawnPercentage_Straight);

		if (nextTileType && FMath::RandRange(1, 100) > TileSpawnInfoRow->SpawnPercentage_Curve) return -1;
		else if (!nextTileType && FMath::RandRange(1, 100) > TileSpawnInfoRow->SpawnPercentage_Straight) return -1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : TileSpawnInfoTable Is Not Found!"));
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
	ATileBasic* NewTile = GetWorld()->SpawnActorDeferred<ATileBasic>(SpawnTarget, SpawnTransform);
	
	return NewTile;
}

bool ATileGenerator::SetSpawnTileIdxRange(const int32 start, const int32 finish)
{
	if (start > finish) return false;

	UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Range is %d to %d"), start, finish);
	SpawnTileMinIdx = start;
	SpawnTileMaxIdx = finish;
	return true;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target�� null �̶�� Destroy ���� ����
	target->Destroy();
}