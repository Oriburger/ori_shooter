// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MobBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = {"Player"};

	CameraBoomNormal = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_NORMAL"));
	CameraBoomNormal->SetupAttachment(RootComponent);
	CameraBoomNormal->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomNormal->bUsePawnControlRotation = true;
	CameraBoomNormal->ProbeChannel = ECollisionChannel::ECC_Visibility;
	CameraBoomNormal->TargetArmLength = 250.0f;
	CameraBoomNormal->bEnableCameraLag = true;
	CameraBoomNormal->CameraLagSpeed = 10.0f;
	CameraBoomNormal->SocketOffset.Y = 30;
	CameraBoomNormal->SocketOffset.Z = 40;

	CameraBoomAiming = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_AIMING"));
	CameraBoomAiming->SetupAttachment(RootComponent);
	CameraBoomAiming->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomAiming->bUsePawnControlRotation = true;
	CameraBoomAiming->ProbeChannel = ECollisionChannel::ECC_Visibility;
	CameraBoomAiming->TargetArmLength = 150.0f;
	CameraBoomAiming->bEnableCameraLag = true;
	CameraBoomAiming->CameraLagSpeed = 5.0f;
	CameraBoomAiming->SocketOffset.Y = 80;
	CameraBoomAiming->SocketOffset.Z = 35;

	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOLLOWING_CAMERA"));
	FollowingCamera->SetupAttachment(CameraBoomNormal);
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 350;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//this->bUseControllerRotationYaw = false;

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("WEAPON"));	
	Weapon->SetupAttachment(this->GetMesh(), TEXT("Weapon"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit); //Hit �̺�Ʈ �߰�
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsInGame) return;

	//�����̵� �ڵ� ���� ����
	if (GetCharacterMovement()->IsCrouching())
	{
		CrouchingTime += DeltaTime;
		if (CrouchingTime > 0.5f) StopSlide(); //�����̵� �ϴ� �ð��� 0.5��
	}

	//Ragdoll ���� ����
	if (DisableRagdollDelay > 0) //Ragdoll ���°� �ǰ�, �ٽ� Ǯ���������� ������
	{
		DisableRagdollDelay = FMath::Max(0.0f, DisableRagdollDelay - DeltaTime);
		if (DisableRagdollDelay == 0) SetPlayerRagdoll(false); 
	}
	else if (GettingUpTimeDelay > 0) //GettingUp AnimMontage�� ����Ǵ� ����, bIsRagdoll �� ������ ������
	{
		GettingUpTimeDelay = FMath::Max(0.0f, GettingUpTimeDelay - DeltaTime);
		if (GettingUpTimeDelay == 0)
		{
			bIsRagdoll = false;
			CharacterMaxWalkSpeed = FMath::Max(CharacterMinWalkSpeed, CharacterMaxWalkSpeed * 90 / 100);
			CharacterMaxAimingWalkSpeed = FMath::Max(CharacterMinAimingWalkSpeed, CharacterMaxAimingWalkSpeed * 90 / 100);
		}
	}

	//MoveForward(1);

	CharacterMaxWalkSpeed += DeltaTime*5;
	CharacterMaxAimingWalkSpeed += DeltaTime*5;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxWalkSpeed;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�Է��� �Լ��� ���ε�
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::StopAim);

	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMainCharacter::StartSlide);
	//PlayerInputComponent->BindAction("Slide", IE_Released, this, &AMainCharacter::StopSlide);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
}

bool AMainCharacter::GetPlayerIsAiming() const { return bIsAimed;  }

bool AMainCharacter::GetPlayerIsDead() const { return bIsDead;  }

void AMainCharacter::MoveForward(float Value)
{
	if (!GetWorld() || bIsDead || bIsRagdoll) return;
	//���� Controller�� X �������� Value ��ŭ �̵�
	FVector Direction = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X);
	if (abs(Value) > 0) this->SetActorRotation({0, Controller->GetControlRotation().Yaw, 0});
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	if (!GetWorld() || bIsDead || bIsRagdoll) return;
	//���� Controller�� Y �������� Value ��ŭ �̵�
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (abs(Value) > 0) this->SetActorRotation({ 0, Controller->GetControlRotation().Yaw, 0 });
	AddMovementInput(Direction, Value*10);
	MoveForward(abs(Value*10));
}

void AMainCharacter::Fire()
{
	if (!GetWorld() || !ProjectileClass || !bIsAimed || bIsDead || bIsRagdoll) return;
	if (GetCharacterMovement()->IsCrouching()) return;

	//UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	FHitResult LineTraceHitResult; //LineTracing�� ����� ��� ����
	FVector TraceBeginLocation = FollowingCamera->GetComponentLocation(); //Trace�� ī�޶󿡼� ����
	FVector TraceEndLocation = TraceBeginLocation + (FollowingCamera->GetForwardVector()) * 200000.0f; //End�� Camera�κ��� 20000.0f ������ ��������
	FCollisionQueryParams TraceCollisionQuery = FCollisionQueryParams::DefaultQueryParam;
	TraceCollisionQuery.AddIgnoredActor(this->GetUniqueID());


	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
		, ECollisionChannel::ECC_Camera, TraceCollisionQuery); //LineTrace

	FVector BeginLocation = Weapon->GetSocketLocation(TEXT("Muzzle")); //�߻� ������ Muzzle ������ ��ġ 
	//Muzzle ���Ͽ��� LineTrace�� Hit�� ��ġ������ Rotataion�� �߻簢
	FRotator BeginRotation = UKismetMathLibrary::FindLookAtRotation(BeginLocation, LineTraceHitResult.ImpactPoint);

	//���� LineTrace�� Hit �� Ÿ���� ���ٸ�, ī�޶��� Rotation�� ���� 
	if (LineTraceHitResult.GetActor() == nullptr && LineTraceHitResult.GetComponent() == nullptr)
	{
		BeginRotation = FollowingCamera->GetComponentRotation();
	}
	//UE_LOG(LogTemp, Warning, TEXT("ActorName : %s"), *UKismetSystemLibrary::GetDisplayName(LineTraceHitResult.GetActor()));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(ProjectileClass, BeginLocation, BeginRotation);

	if (Projectile)
	{
		FVector LaunchDirection = BeginRotation.Vector();
		Projectile->FireInDirection(LaunchDirection);
	}

	if (FireAnimMontage != nullptr && ShootSound != nullptr)
	{
		PlayAnimMontage(FireAnimMontage);
		GameStatic->PlaySoundAtLocation(GetWorld(), ShootSound, this->GetActorLocation(), this->GetActorRotation(), 1.0f);
	}
}

void AMainCharacter::Aim()
{
	if (bIsDead || bIsRagdoll) return;
	if (GetCharacterMovement()->IsCrouching()) return;
	//UE_LOG(LogTemp, Warning, TEXT("Aim"));

	bIsAimed = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxAimingWalkSpeed;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomAiming, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void AMainCharacter::StopAim()
{
	//UE_LOG(LogTemp, Warning, TEXT("AimStop"));

	bIsAimed = false;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomNormal, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
} 

void AMainCharacter::StartSlide()
{
	if (GetCharacterMovement()->IsFalling() || bIsDead || bIsRagdoll) return;
	if (GetCharacterMovement()->IsCrouching()) return;

	StopAim();
	//UE_LOG(LogTemp, Warning, TEXT("Crouch"));
	ACharacter::Crouch();
}

void AMainCharacter::StopSlide()
{
	//UE_LOG(LogTemp, Warning, TEXT("UnCrouch"));
	ACharacter::UnCrouch();
	CrouchingTime = 0;
}

void AMainCharacter::StartJump()
{
	if (GetCharacterMovement()->IsFalling() || bIsDead) return;
	StopSlide();
	ACharacter::Jump();
}

float AMainCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent
	, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//UE_LOG(LogTemp, Warning, TEXT("Take Damage"));

	PlayAnimMontage(HitAnimMontage, 1.5f);

	CharacterCurrHP = FMath::Max(0.0f, CharacterCurrHP-Damage);
	if (CharacterCurrHP == 0) Die();

	return FinalDamage;
}

void AMainCharacter::Die()
{
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("DEAD"));
	SetPlayerRagdoll(true);
}

void AMainCharacter::SetPlayerRagdoll(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag); //�÷��̾��� Ragdoll ���¸� flag�� ����
	if (flag) GetCharacterMovement()->Deactivate(); //true���, ĳ������ Movement�� ��Ȱ��ȭ ��Ų��

	else //Mesh ��ġ�� �ʱ�ȭ ��Ű�� �Ͼ�� �ִϸ��̼��� ���
	{
		FAttachmentTransformRules ResetTransform = FAttachmentTransformRules::KeepRelativeTransform; 

		GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetComponentLocation(), false, nullptr, ETeleportType::TeleportPhysics);
		GetMesh()->AttachToComponent(GetCapsuleComponent(), ResetTransform);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -68.0f), false, nullptr, ETeleportType::TeleportPhysics);
		GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

		if(GettingUpAnimMontage!=nullptr)
			GettingUpTimeDelay = PlayAnimMontage(GettingUpAnimMontage, 1.5f) / 1.5f;

		GetCharacterMovement()->Activate();
	}
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld() || OtherActor == nullptr) return;

	if (OtherActor->ActorHasTag("Obstacle"))
	{
		SetPlayerRagdoll(true);
		bIsRagdoll = true;
		DisableRagdollDelay = 1.5f;
	}

	else if (OtherActor->ActorHasTag("Mob") && Cast<AMobBase>(OtherActor)->GetIsExploding())
	{
		
	}
}