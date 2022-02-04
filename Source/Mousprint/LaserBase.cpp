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
	ArrowComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	ArrowComponent->ArrowSize = 1.0f;

	NELaserComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LASER"));
	NELaserComponent->SetupAttachment(RootComponent);
	NELaserComponent->SetColorParameter("Color", FLinearColor(0.0f, 1.0f, 0.1f, 0.5f));

	NELaserImpactComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LASER_IMPACT"));
	NELaserImpactComponent->SetupAttachment(RootComponent);
	NELaserImpactComponent->SetColorParameter("Color", FLinearColor(0.0f, 1.0f, 0.1f, 0.1f));
}

// Called when the game starts or when spawned
void ALaserBase::BeginPlay()
{
	Super::BeginPlay();
	NELaserComponent->SetWorldLocation(GetActorLocation());
}

// Called every frame
void ALaserBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDamageDelay(DeltaTime);
	CheckHitAndApplyDamage(UpdateLaserEndLocation());
}

FHitResult ALaserBase::UpdateLaserEndLocation()
{
	FHitResult LineTraceHitResult; //LineTracing�� ����� ��� ����
	FVector TraceBeginLocation = this->GetActorLocation();
	FVector TraceEndLocation = TraceBeginLocation + (GetActorRotation().Vector().ForwardVector) * 1000.0f;
	FCollisionQueryParams TraceCollisionQuery = FCollisionQueryParams::DefaultQueryParam;
	TraceCollisionQuery.AddIgnoredActor(this->GetUniqueID());  //�÷��̾��� ī�޶� Hit���� �ʵ��� ����


	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
		, ECollisionChannel::ECC_Camera, TraceCollisionQuery); //LineTrace ����


	//LineTrace�� ���� �������� ����ġ�� Impact��ġ ����
	if (LineTraceHitResult.bBlockingHit)
	{
		NELaserComponent->SetVectorParameter("LaserEnd", LineTraceHitResult.Location);
		NELaserImpactComponent->SetWorldLocation(LineTraceHitResult.Location);
	}
	else
	{
		NELaserComponent->SetVectorParameter("LaserEnd", TraceEndLocation);
		NELaserImpactComponent->SetWorldLocation(TraceEndLocation);
	}

	return LineTraceHitResult;
}

void ALaserBase::CheckHitAndApplyDamage(FHitResult OutHit)
{
	if (!IsValid(OutHit.GetActor()) || !OutHit.GetActor()->ActorHasTag("Player")) return;
	if (DamageDelay != 0.0f) return;

	AActor* HitActor = OutHit.GetActor();
	UGameplayStatics::ApplyPointDamage(HitActor, 10.0f, HitActor->GetActorLocation(),
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