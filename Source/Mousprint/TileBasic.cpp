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

bool ATileBasic::InitObstacle()
{
	if (!GetWorld() || ObstacleClassArray.Num() == 0 || ObstacleSpawnPointArray.Num() == 0) return false;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Ÿ���� �����ڴ� Generator
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < MaxObstacleCount; i++)
	{
		FTransform SpawnTransform = ObstacleSpawnPointArray[i]->GetComponentTransform();
		const int32 ObstacleIdx = FMath::RandRange(0, ObstacleClassArray.Num() - 1);
		AActor* NewObstacle = GetWorld()->SpawnActor(ObstacleClassArray[ObstacleIdx], &SpawnTransform, SpawnParams);
		SpawnedObstacleArr.Push(NewObstacle);
	}
	return true;
}

bool ATileBasic::IsOverlapped() const {	return OverlapFlag; }


void ATileBasic::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//�������� ���Ͱ� �÷��̾ �ƴϸ� return;
	if (!OtherActor->ActorHasTag(TEXT("Player")) && !OtherComp->ComponentHasTag(TEXT("Ragdoll"))) return;
	OverlapFlag = true;
}

void ATileBasic::DestroyObstacle()
{
	//������ ��ֹ����� ���� ��ȯ

	UE_LOG(LogTemp, Warning, TEXT("TileBasic : DestroyObstacle()-----"));

	if (SpawnedObstacleArr.Num() == 0) return;

	UE_LOG(LogTemp, Warning, TEXT("TileBasic : Obstacle Total Count = %d"), SpawnedObstacleArr.Num());

	for(int idx = SpawnedObstacleArr.Num()-1; idx>=0; idx--)
	{
		if (!SpawnedObstacleArr.IsValidIndex(idx)) continue;
		AActor* DestroyTarget = SpawnedObstacleArr[idx];
		if (DestroyTarget != nullptr && IsValid(DestroyTarget)) DestroyTarget->Destroy();
		SpawnedObstacleArr.RemoveAt(idx);

		UE_LOG(LogTemp, Warning, TEXT("TileBasic : Removed Obstacle #%d"), idx);
	}
}
