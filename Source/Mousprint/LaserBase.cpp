// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBase.h"

// Sets default values
ALaserBase::ALaserBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT")); //RootComponent ����
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot); //��Ʈ ������Ʈȭ

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("DIRECTION_ARROW"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->ArrowSize = 1.0f;
	
}

// Called when the game starts or when spawned
void ALaserBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALaserBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDamageDelay(DeltaTime);
	CheckHitAndApplyDamage(GetLineTraceEndLocation());
}

FHitResult ALaserBase::GetLineTraceEndLocation()
{
	FHitResult LineTraceHitResult; //LineTracing�� ����� ��� ����
	FVector TraceBeginLocation = this->GetActorLocation();
	FVector TraceEndLocation = TraceBeginLocation + (ArrowComponent->GetComponentRotation().Vector()) * 10000.0f;

	FCollisionQueryParams TraceCollisionQuery = FCollisionQueryParams::DefaultQueryParam;
	TraceCollisionQuery.AddIgnoredActor(this->GetUniqueID());  //�÷��̾��� ī�޶� Hit���� �ʵ��� ����

	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
		, ECollisionChannel::ECC_Camera, TraceCollisionQuery); //LineTrace ����
	
	return LineTraceHitResult;
}

void ALaserBase::CheckHitAndApplyDamage(FHitResult OutHit)
{
	if (!IsValid(OutHit.GetActor()) || !OutHit.GetActor()->ActorHasTag("Player")) return;
	if (DamageDelay != 0.0f) return;

	AActor* HitActor = OutHit.GetActor();
	UGameplayStatics::ApplyPointDamage(HitActor, 25.0f, HitActor->GetActorLocation(),
									  OutHit, nullptr, this, nullptr);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, this->GetActorLocation()
										, this->GetActorRotation(), 1.0f);
	DamageDelay = 0.25f;
}

void ALaserBase::UpdateDamageDelay(const float DeltaTime)
{
	if (DamageDelay <= 0) return;
	DamageDelay = FMath::Max(0.0f, DamageDelay - DeltaTime);
}