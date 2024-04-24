// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadAsset.generated.h"
class FPakPlatformFile;
UCLASS()
class LEARNING_API ALoadAsset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoadAsset();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void TestFindObject();
	virtual void TestFindPackage();
	
	virtual void TestLoadClass();
	virtual void TestStaticLoadClass();
	virtual void TestLoadObject();
	virtual void TestStaticLoadObject();
	virtual void TestLoadPackage();
	virtual void TestFSoftObjectPathTryLoad();

	virtual void TestFStreamableManager_RequestSyncLoad();
	virtual void TestFStreamableManager_LoadSynchronous();

	virtual void TestLoadPackageAsync();
	virtual void TestFStreamableManager_RequestAsyncLoad();
	virtual void TestFStreamableManager_AsyncLoadComplete();

	virtual void TestLoadPak();
	virtual void TestCreatePak();

};
