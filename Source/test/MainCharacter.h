// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class TEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsAimed;
	bool bIsInGame = true; //= false;

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave* ShootSound; //�ѱ� �߻� ����

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterMaxHP = 100;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterCurrHP = 100;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterMaxWalkSpeed = 1500;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterAimingWalkSpeed = 800;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent* Weapon; //���� �޽�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming�� ���� ���� CameraBoom

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming�� �� Zoom�� CameraBoom

	UPROPERTY(BlueprintReadWrite, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

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

	UFUNCTION()
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
		void StartCrouch();

	UFUNCTION()
		void StopCrouch();

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		virtual float TakeDamage(const float damage);

	UFUNCTION()
		void Die();

	UFUNCTION()
		void SetPlayerRagdoll(const bool flag);
};