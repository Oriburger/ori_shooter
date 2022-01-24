// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBasic.h"
#include "MainCharacter.h"

// Sets default values
ATileBasic::ATileBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->Tags = { "Tile" };

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT")); //RootComponent ����
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot); //��Ʈ ������Ʈȭ

	EdgeArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EDGE_ARROW"));
	EdgeArrowComponent->SetupAttachment(RootComponent);
	EdgeArrowComponent->SetRelativeLocationAndRotation(FVector(1000.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	EdgeArrowComponent->ArrowSize = 2.0f;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_TRIGGER"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetRelativeScale3D(FVector(1.0f, 8.75f, 10.0f));
	BoxTrigger->SetRelativeLocation(FVector(500.0f, 0.0f, 300.0f));
	
	for (int idx = 0; idx < MaxObstacleCount; idx++)
	{
		FName ObstacleName = FName("OBSTACLE" + FString::FromInt(idx));
		ObstacleSpawnPointArray.Add(CreateDefaultSubobject<USphereComponent>(ObstacleName));
		ObstacleSpawnPointArray[idx]->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ATileBasic::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ATileBasic::OnEndOverlap); //������ �̺�Ʈ�� �߰�

	InitObstacle();

	if (ObstaclePointUseCount > ObstacleSpawnPointArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable \"UseObstacle\" is Overflowed!!!"));
	}
}

// Called every frame
void ATileBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform ATileBasic::GetNextSpawnPoint() const
{
	return EdgeArrowComponent->GetComponentTransform(); 
}

void ATileBasic::SetTileSpawnInfo(const int32 _ObstaclePointUseCount, const int32 _MaxSpawnObstacleCount
	, const float _ObstacleSpawnPercentage)
{
	ObstaclePointUseCount = _ObstaclePointUseCount;
	MaxSpawnObstacleCount = _MaxSpawnObstacleCount;
	ObstacleSpawnPercentage = _ObstacleSpawnPercentage;
}

bool ATileBasic::InitObstacle()
{
	if (!GetWorld() || !ObstacleClass || ObstacleSpawnPointArray.Num() == 0 || ObstaclePointUseCount == 0) return false;

	const FActorSpawnParameters SpawnParams;

	const int32 endIdx = FMath::RandRange(0, ObstaclePointUseCount - 1);
	int32 idx = endIdx;

	do
	{
		if (SpawnedObstacleCnt >= MaxSpawnObstacleCount) break;
		if (!ObstacleSpawnPointArray.IsValidIndex(idx)) break;

		const auto target = ObstacleSpawnPointArray[idx];
		if (FMath::RandRange(1, 100) >= 100.0f - ObstacleSpawnPercentage)
		{
			AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClass, target->GetComponentLocation(), target->GetComponentRotation());
			if (SpawnedObstacle == nullptr) continue;
			SpawnedObstacleArr.Push(SpawnedObstacle);
			SpawnedObstacleCnt++;
		}
		idx = (idx + 1) % ObstaclePointUseCount;
	} while (idx != endIdx);
	

	return true;
}

bool ATileBasic::IsOverlapped() const {	return OverlapFlag;   }

int32 ATileBasic::GetTileIdx() const {	return TileIdx == -1 ? 0 : TileIdx;    }

void ATileBasic::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag(TEXT("Player"))) return; //�������� ���Ͱ� �÷��̾ �ƴϸ� return;
	OverlapFlag = true;
}

void ATileBasic::DestroyObstacle()
{
	//������ ��ֹ����� ���� ��ȯ
	if (SpawnedObstacleArr.Num() == 0) return;

	for(int idx=SpawnedObstacleArr.Num()-1; idx>=0; idx--)
	{
		if (SpawnedObstacleArr.IsValidIndex(idx)) continue;
		AActor* DestroyTarget = SpawnedObstacleArr[idx];
		if (DestroyTarget != nullptr && IsValid(DestroyTarget)) DestroyTarget->Destroy();
		SpawnedObstacleArr.RemoveAt(idx);
	}
}
