// Fill out your copyright notice in the Description page of Project Settings.

#include "Math/UnrealMathUtility.h"
#include "TileGenerator.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();

	SpawnedTileArr.Push(SpawnTile(true, 0)); //���� ���� �� �ϳ��� ����
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� �� Ÿ���� ���� MaxSpawnTileCnt���� �۰�, ���� �۾� ���� �ƴ϶��
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		bIsSpawningTile = true; //�ߺ� ���� ����
		SpawnedTileArr.Push(SpawnTile(false, 0)); //Spawn �� Ÿ���� Arr�� ����
		bIsSpawningTile = false;
	}
	if (SpawnedTileArr.IsValidIndex(1) //�÷��̾ 2��° Ÿ���� ������ ������ ��Ҵٸ�
		&& SpawnedTileArr[1]->IsOverlapped())
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

ATileBasic* ATileGenerator::SpawnTile(const bool _bIsInit, const int TileIdx)
{
	if (!GetWorld() || TileClassArray.Num() <= TileIdx) return nullptr; //Idx ������ Invalid ��� nullptr ��ȯ
	if (!TileClassArray[TileIdx] ) return nullptr; //BP���� TileClassArray[TileIdx]�� Ŭ���� ������ ���� �ʾҴٸ� nullptr��ȯ

	UE_LOG(LogTemp, Warning, TEXT("InitTile")); //����� Log

	FRotator BeginRotation = FRotator(0, 0, 0); 
	FVector BeginLocation;	
	if (_bIsInit) BeginLocation = FVector(0, 0, 0); //���� ���� �� �ʱ� ��ġ�� {0,0,0}
	else BeginLocation = GetNextSpawnTransform().GetLocation(); 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Ÿ���� �����ڴ� Generator
	SpawnParams.Instigator = GetInstigator(); 

	//Ÿ���� �����ϰ� ��ȯ
	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(TileClassArray[TileIdx], BeginLocation, BeginRotation);
	
	return NewTile;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target�� null �̶�� Destroy ���� ����
	target->Destroy();
}