// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
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
	CameraBoomNormal->TargetArmLength = 250.0f;
	CameraBoomNormal->bEnableCameraLag = true;
	CameraBoomNormal->CameraLagSpeed = 10.0f;
	CameraBoomNormal->SocketOffset.Y = 50;

	CameraBoomAiming = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_AIMING"));
	CameraBoomAiming->SetupAttachment(RootComponent);
	CameraBoomAiming->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomAiming->bUsePawnControlRotation = true;
	CameraBoomAiming->TargetArmLength = 150.0f;
	CameraBoomAiming->bEnableCameraLag = true;
	CameraBoomAiming->CameraLagSpeed = 10.0f;
	CameraBoomAiming->SocketOffset.Y = 50;
	CameraBoomAiming->SocketOffset.Z = 60;

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
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetCharacterMovement()->IsCrouching())
	{
		CrouchingTime += DeltaTime;
		if (CrouchingTime > 1.0f)
		{
			StopSlide();
			CrouchingTime = 0;
		}
	}

	if (bIsInGame)
	{
		MoveForward(1);

		CharacterMaxWalkSpeed += DeltaTime*10;
		CharacterAimingWalkSpeed += DeltaTime*10;
		GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxWalkSpeed;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//입력을 함수와 바인딩
	//PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
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

void AMainCharacter::MoveForward(float Value)
{
	//현재 Controller의 X 방향으로 Value 만큼 이동
	FVector Direction = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X);
	if (abs(Value) > 0) this->SetActorRotation({0, Controller->GetControlRotation().Yaw, 0});
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	//현재 Controller의 Y 방향으로 Value 만큼 이동
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (abs(Value) > 0) this->SetActorRotation({ 0, Controller->GetControlRotation().Yaw, 0 });
	AddMovementInput(Direction, Value*10);
	MoveForward(abs(Value*10));
}

void AMainCharacter::Fire()
{
	if (!GetWorld() || !ProjectileClass || !bIsAimed) return;
	if (GetCharacterMovement()->IsFalling()) return;

	UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	FHitResult LineTraceHitResult; //LineTracing의 결과가 담길 변수
	FVector TraceBeginLocation = FollowingCamera->GetComponentLocation(); //Trace는 카메라에서 시작
	FVector TraceEndLocation = TraceBeginLocation + (FollowingCamera->GetComponentRotation()).Vector() * 200000.0f;
								//End는 Camera로부터 20000.0f 떨어진 지점까지


	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
										, ECollisionChannel::ECC_Visibility); //LineTrace

	FVector BeginLocation = Weapon->GetSocketLocation(TEXT("Muzzle")); //발사 시작은 Muzzle 소켓의 위치 
	FRotator BeginRotation = UKismetMathLibrary::FindLookAtRotation(BeginLocation, LineTraceHitResult.ImpactPoint);
							 //Muzzle 소켓에서 LineTrace이 Hit된 위치까지의 Rotataion이 발사각


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator(); 
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(ProjectileClass, BeginLocation, BeginRotation);
	
	if (Projectile)
	{
		FVector LaunchDirection = BeginRotation.Vector();
		Projectile->FireInDirection(LaunchDirection);
	}

	GameStatic->PlaySoundAtLocation(GetWorld(), ShootSound, this->GetActorLocation(), this->GetActorRotation(), 1.0f);
}	

void AMainCharacter::Aim()
{
	UE_LOG(LogTemp, Warning, TEXT("Aim"));

	bIsAimed = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterAimingWalkSpeed;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomAiming, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void AMainCharacter::StopAim()
{
	UE_LOG(LogTemp, Warning, TEXT("AimStop"));

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
	if (GetCharacterMovement()->IsFalling()) return;
	UE_LOG(LogTemp, Warning, TEXT("Crouch"));
	ACharacter::Crouch();
}

void AMainCharacter::StopSlide()
{
	UE_LOG(LogTemp, Warning, TEXT("UnCrouch"));
	ACharacter::UnCrouch();
}

void AMainCharacter::StartJump()
{
	if (GetCharacterMovement()->IsFalling()) return;
	ACharacter::Jump();
}

float AMainCharacter::TakeDamage(const float damage)
{
	CharacterCurrHP = (CharacterCurrHP - damage < 0 ? 0 : CharacterCurrHP - damage);

	if (CharacterCurrHP == 0) Die();

	return CharacterCurrHP;
}

void AMainCharacter::Die()
{
	SetPlayerRagdoll(true);
}

void AMainCharacter::SetPlayerRagdoll(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag);
	if (flag) GetCharacterMovement()->DisableMovement();
	else GetCharacterMovement()->Activate();
}