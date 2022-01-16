// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
#include "PathBase.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class MOUSPRINT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsDead = false;
	bool bIsRagdoll = false;
	bool bIsAimed = false;
	bool bIsInGame = true; //= false;

	float PathSpawnTime = 0;
	float DisableRagdollDelay = 0; //Ragdoll 상태에서 다시 풀릴때까지 걸리는 시간
	float GettingUpTimeDelay = 0; //플레이어가 넘어진 상태에서 다 일어나기까지 걸리는 시간
	float CrouchingTime = 0; //슬라이딩 시간 

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave* ShootSound; //총기 발사 사운드

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterCurrHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMinWalkSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMinAimingWalkSpeed = 750;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxWalkSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxAimingWalkSpeed = 750;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent* Weapon; //무기 메시

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming을 하지 않은 CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming을 한 Zoom된 CameraBoom

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* GettingUpAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class ABullet> ProjectileClass; //무기에 딸려있는 Projectile Class -> Weapon 클래스로 옮겨질 예정

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

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const; //플레이어가 Aiming 중인지 반환

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDead() const; 

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsGettingUp() const;

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

	UFUNCTION()
		void StartSlide();

	UFUNCTION()
		void TryStopSlide(const float DeltaTime, const bool force = false);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent
							, AController* EventInstigator, AActor * DamageCauser) override;

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