// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "Containers/Array.h"
#include "TileGenerator.generated.h"

UCLASS()
class TEST_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	ATileBasic* curr;
	ATileBasic* prev;

public:	
	// Sets default values for this actor's properties
	ATileGenerator();

	UPROPERTY(VisibleDefaultsOnly)
		bool bIsSpawningTile = false;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
	//	TSubclassOf<class ATileBasic> SpawnTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > TileClassArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FTransform GetNextSpawnTransform() const;

	UFUNCTION(BlueprintCallable)
		void SetCurrTile(ATileBasic * tmp);

	UFUNCTION()
		ATileBasic* SpawnTile(const bool _bIsInit, const int TileIdx);

	UFUNCTION(BlueprintCallable)
		void DestroyPrevTile();
};
