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

	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetEnableGravity(false);
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->SetCollisionProfileName(FName("Projectile"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 9000.0f; //�߻�ü�� �ʱ� �ӷ�
	ProjectileMovementComponent->MaxSpeed = 10000.0f; //�߻�ü�� �ִ� �ӷ�
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit); //Hit �̺�Ʈ �߰�
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
				, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		const FVector force = this->GetVelocity() * 100;
		OtherComp->AddForceAtLocation(force, this->GetActorLocation());

		UE_LOG(LogTemp, Warning, TEXT("Destructable!"));
	}
}

void ABullet::FireInDirection(const FVector& ShootDirection) 
{
	//ShootDirection �������� InitialSpeed ��ŭ �߻�
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}