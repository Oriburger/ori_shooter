// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(CollisionComponent); //��Ʈ ������Ʈȭ
	CollisionComponent->InitSphereRadius(20.0f);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 5500.0f; //�߻�ü�� �ʱ� �ӷ�
	ProjectileMovementComponent->MaxSpeed = 10000.0f; //�߻�ü�� �ִ� �ӷ�
	ProjectileMovementComponent->bShouldBounce = true;
	
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin); //������ �̺�Ʈ �߰�
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	 -- �߰� ���� --
	*/
}

void ABullet::FireInDirection(const FVector& ShootDirection) 
{
	//ShootDirection �������� InitialSpeed ��ŭ �߻�
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}