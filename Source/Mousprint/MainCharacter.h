// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Bullet.h"
#include "PathBase.h"
#include "Sound/SoundWave.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class MOUSPRINT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsDizzy = false;
	bool bIsDead = false;
	bool bIsRagdoll = false;
	bool bIsAimed = false;
	bool bIsInGame = true; //= false;

	float PathSpawnTime = 0; //PathActor �������� DeltaTime ��� �� 
	float DisableRagdollDelay = 0; //Ragdoll ���¿��� �ٽ� Ǯ�������� �ɸ��� �ð�
	float GettingUpTimeDelay = 0; //�÷��̾ �Ѿ��� ���¿��� �� �Ͼ����� �ɸ��� �ð�
	float DizzyTime = 0; //�÷��̾��� ��Ʈ�� 0�� �� ���, ������ ����������� �ð�
	float CrouchingTime = 0; //�����̵� �ð� 

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Effect)
		USoundWave* HitSound; //������ ����

	UPROPERTY(EditAnywhere, Category = Effect)
		USoundWave* ShootSound; //�ѱ� �߻� ����

	UPROPERTY(EditAnywhere, Category = Effect)
		TSubclassOf<class UCameraShakeBase> DamageCameraShake;

	UPROPERTY(EditAnywhere, Category = Effect)
		TSubclassOf<class UCameraShakeBase> GasCameraShake;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	/* ----- Charater State + Tutorial ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterCurrHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMinWalkSpeed = 1000; //�÷��̾��� �ּ� �̵� �ӵ� ���� (�� �� �̻����� �پ���� ����)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxWalkSpeed = 1400; //�÷��̾��� �ִ� �̵� �ӵ� ���� (�� �� �̻����� �þ�� ����)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterWalkSpeed = 1000; //�÷��̾��� �̵� �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanSlide = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanAim = true;


	/* ----- Basic Component ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent* Weapon; //���� �޽�

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming�� ���� ���� CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming�� �� Zoom�� CameraBoom

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

	/* ------ Anim Montages ------ */
	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* DizzyAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* GettingUpAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* RushAnimMontage;

	/* ----- Spawn Class ----- */
	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class ABullet> ProjectileClass; //���⿡ �����ִ� Projectile Class -> Weapon Ŭ������ �Ű��� ����

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class APathBase> PathClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ---�÷��̾��� ����---- */
	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const; //�÷��̾ Aiming ������ ��ȯ

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDead() const; 

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsGettingUp() const;

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDizzy() const;

	/* --- Axis & Action Input --- */
	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value); 

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void Aim();

	UFUNCTION()
		void StopAim();

	UFUNCTION(BlueprintCallable)
		void StartSlide();

	UFUNCTION()
		void TryStopSlide(const float DeltaTime, const bool force = false);

	UFUNCTION()
		void StartRush();

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	/* --- Etc ---- */

	UFUNCTION()
		void UpdateDizzyState(const float DeltaTime);

	UFUNCTION()
		void UpdateRagdollState(const float DeltaTime);

	UFUNCTION()
		void UpdateCharacterSpeed(const float DeltaTime);

	UFUNCTION()
		void UpdateGasCamShakeEffect() const;

	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent
							, AController* EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable)
		bool SetWalkSpeedLimit(const float MinValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void SetPlayerRagdoll(const bool flag);

	UFUNCTION()
		bool SpawnPathActor(const float DeltaTime);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

};