// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
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

	float DisableRagdollDelay = 0;
	float GettingUpTimeDelay = 0;
	float CrouchingTime = 0; //�����̵� �ð� 

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave* ShootSound; //�ѱ� �߻� ����

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
		UChildActorComponent* Weapon; //���� �޽�

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming�� ���� ���� CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming�� �� Zoom�� CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* GettingUpAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<class ABullet> ProjectileClass; //���⿡ �����ִ� Projectile Class -> Weapon Ŭ������ �Ű��� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const; //�÷��̾ Aiming ������ ��ȯ

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDead() const; 

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
		void StopSlide();

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
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

};