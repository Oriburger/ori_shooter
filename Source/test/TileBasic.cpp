// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBasic.h"
#include "MainCharacter.h"

// Sets default values
ATileBasic::ATileBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT")); //RootComponent ����
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot); //��Ʈ ������Ʈȭ

	EdgeArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EDGE_ARROW"));
	EdgeArrowComponent->SetupAttachment(RootComponent);
	EdgeArrowComponent->SetRelativeLocationAndRotation(FVector(6000.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	EdgeArrowComponent->ArrowSize = 2.0f;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_TRIGGER"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetRelativeScale3D(FVector(1.0f, 8.75f, 5.0f));
	BoxTrigger->SetRelativeLocation(FVector(500.0f, 0.0f, 170.0f));

	for (int idx = 0; idx < ObstacleCount; idx++)
	{
		//ObstacleSpawnPoint.Add(CreateDefaultSubobject<UBoxComponent>(TEXT("OBSTACLE")));
		//ObstacleSpawnPoint[idx]->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ATileBasic::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ATileBasic::OnEndOverlap); //������ �̺�Ʈ�� �߰�
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

bool ATileBasic::IsOverlapped() const
{
	return OverlapFlag;
}

void ATileBasic::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag(TEXT("Player"))) return; //�������� ���Ͱ� �÷��̾ �ƴϸ� return;
	OverlapFlag = true;
}