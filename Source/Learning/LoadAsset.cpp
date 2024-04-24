// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "HAL/PlatformFileManager.h"
#include "IPlatformFilePak.h"
#include "MassEntity/Public/MassCommands.h"

// Sets default values
ALoadAsset::ALoadAsset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALoadAsset::BeginPlay()
{
	Super::BeginPlay();
	TestLoadObject();
	TestFindObject();
}

void ALoadAsset::TestFindObject()
{
	//有问题
	UObject* obj = FindObject<UObject>(nullptr,TEXT("Cube"));
	UE_LOG(LogTemp, Warning, TEXT("TestFindObject:%d"), obj != nullptr);
}

void ALoadAsset::TestFindPackage()
{
	const FString packageName("/Game/Cube");
	UPackage* package = FindPackage(nullptr, *packageName);
	if (package && package->IsFullyLoaded())
	{
		TArray<UObject*> results;
		GetObjectsWithOuter(package, results);
		for (UObject* obj: results)
		{
			if (obj != nullptr && obj->IsA<UBlueprint>())
			{
				UBlueprint* bp = Cast<UBlueprint>(obj);
				AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(0,0,0), FRotator(0,0,0));
				actor->SetActorLabel(TEXT("Cube_TestFindPackage"));
			}
			UE_LOG(LogTemp, Warning, TEXT("TestFindPackage objName:%s"), *obj->GetName());
		}
	}
}

void ALoadAsset::TestLoadClass()
{
	FString path = "/Game/Cube.Cube_C";
	UClass* obj = LoadClass<UObject>(nullptr,*path);
	if (obj != nullptr)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(obj, FVector(0,0,0), FRotator(0,0,0));
	}
	UE_LOG(LogTemp, Warning, TEXT("TestLoadClass:%d"), obj != nullptr)
}

void ALoadAsset::TestStaticLoadClass()
{
	FString path = "/Game/Cube.Cube_C";
	UClass* obj = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
	if (obj != nullptr)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(obj, FVector(0,0,0), FRotator(0,0,0));
	}
	UE_LOG(LogTemp, Warning, TEXT("TestStaticLoadClass:%d"), obj != nullptr)
}

void ALoadAsset::TestStaticLoadObject()
{
	FString path = "/Game/Cube";
	UObject* obj = StaticLoadObject(UObject::StaticClass(), nullptr, *path);
	UBlueprint* bp = Cast<UBlueprint>(obj);
	if (bp != nullptr)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(0,0,0), FRotator(0,0,0));
	}
	UE_LOG(LogTemp, Warning, TEXT("TestLoadObject:%d"), bp != nullptr)
}

void ALoadAsset::TestLoadObject()
{
	FString path = "/Game/Cube";
	UObject* obj = LoadObject<UObject>(nullptr, *path, nullptr);
	UBlueprint* bp = Cast<UBlueprint>(obj);
	if (bp != nullptr)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(0,0,0), FRotator(0,0,0));
		actor->SetActorLabel(TEXT("Cube_LoadObject"));
	}
	UE_LOG(LogTemp, Warning, TEXT("TestLoadObject:%d"), bp != nullptr);
}

void ALoadAsset::TestLoadPackage()
{
	const FString packageName("/Game/Cube");
	UPackage* package = LoadPackage(nullptr, *packageName, LOAD_None);
	if (package != nullptr && package->IsFullyLoaded())
	{
		TArray<UObject*> results;
		GetObjectsWithPackage(package,results);
		for (UObject* obj: results)
		{
			if (obj != nullptr && obj->IsA<UBlueprint>())
			{
				UBlueprint* bp = Cast<UBlueprint>(obj);
				AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(0,0,0), FRotator(0,0,0));
				actor->SetActorLabel(TEXT("Cube_TestLoadPackage"));
				UE_LOG(LogTemp, Warning, TEXT("TestLoadPackage:%s"), *package->GetName())
			}
		}
	}
}

void ALoadAsset::TestFSoftObjectPathTryLoad()
{
	FSoftObjectPath  path("/Game/Cube");
	UObject* obj = path.TryLoad();
	if (obj != nullptr)
	{
		UBlueprint* bp = Cast<UBlueprint>(obj);
		if (bp != nullptr)
		{
			AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(1,1,1), FRotator(0,0,0));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("TestFSoftObjectPathTryLoad: %d"), obj != nullptr)
}

void ALoadAsset::TestFStreamableManager_RequestSyncLoad()
{
	FSoftObjectPath path( "/Game/Cube");
	TSharedPtr<FStreamableHandle> handle = UAssetManager::GetStreamableManager().RequestSyncLoad(path);
	UObject* obj = handle ? handle->GetLoadedAsset() : nullptr;
	if (obj != nullptr && obj->IsA<UBlueprint>())
	{
		UBlueprint* bp = Cast<UBlueprint>(obj);
		if (bp != nullptr)
		{
			AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(2,2,2), FRotator(0,0,0));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestSyncLoad: %d"), obj != nullptr);
}

void ALoadAsset::TestFStreamableManager_LoadSynchronous()
{
	FSoftObjectPath path("Blueprint'/Game/Cube.Cube'");
	UObject* obj = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (obj != nullptr && obj->IsA<UBlueprint>())
	{
		UBlueprint* bp = Cast<UBlueprint>(obj);
		if (bp != nullptr)
		{
			AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(3,3,3), FRotator(0,0,0));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_LoadSynchronous:%d"), obj != nullptr);
}

void ALoadAsset::TestLoadPackageAsync()
{
	const FString pckPath("/Game/Cube");
	FName pckName(pckPath);
	LoadPackageAsync(pckPath, FLoadPackageAsyncDelegate::CreateLambda([this](const FName& PackageName, const UPackage* ResPackage, EAsyncLoadingResult::Type Result){
		TArray<UObject*> Results;
		GetObjectsWithOuter(ResPackage, Results, false);
		UE_LOG(LogTemp, Warning, TEXT("TestLoadPackageAsync Package1:%d"), Results.Num());
		for (UObject* obj: Results)
		{
			if (obj != nullptr && obj->IsA<UBlueprint>())
			{
				UBlueprint* bp = Cast<UBlueprint>(obj);
				GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(0,0,0), FRotator(0,0,0));
				UE_LOG(LogTemp, Warning, TEXT("TestLoadPackageAsync Package2:%s"), *obj->GetName());
			}
			if (obj != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("TestLoadPackageAsync Package3:%s"), *obj->GetName());
			}
		}
	}));
}

FSoftObjectPath path4("/Game/Cube.Cube");
void ALoadAsset::TestFStreamableManager_RequestAsyncLoad()
{
	//单资源
	
	FSoftObjectPath path1("/Game/Cube.Cube");
	UAssetManager::GetStreamableManager().RequestAsyncLoad(path1, [path1, this]()
	{
		UObject* obj = path1.ResolveObject();
		UBlueprint* bp = Cast<UBlueprint>(obj);
		AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(5,5,5), FRotator(0,0,0));
		UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestAsyncLoad1:%d,%d"), obj != nullptr, actor != nullptr);
	});

	FSoftObjectPath path2("/Game/Cube.Cube");
	FStreamableDelegate delegate2;
	delegate2.BindLambda([this](FSoftObjectPath objPath)
	{
		UObject* obj = objPath.ResolveObject();
		UBlueprint* bp = Cast<UBlueprint>(obj);
		AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(6,6,6), FRotator(0,0,0));
		UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestAsyncLoad2:%d,%d"), obj != nullptr, actor != nullptr);
	},path2);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(path2, delegate2);

	FSoftObjectPath path3("/Game/Cube.Cube");
	FStreamableDelegate delegate3 = FStreamableDelegate::CreateLambda([this](FSoftObjectPath objPath)
	{
		UObject* obj = objPath.ResolveObject();
		UBlueprint* bp = Cast<UBlueprint>(obj);
		AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(7,7,7), FRotator(0,0,0));
		UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestAsyncLoad3:%d,%d"), obj != nullptr, actor != nullptr);
	}, path3);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(path3, delegate3);

	FStreamableDelegate delegate4 = FStreamableDelegate::CreateUObject(this, &ALoadAsset::TestFStreamableManager_AsyncLoadComplete);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(path4, delegate4);

	//多资源
	FSoftObjectPath path5("/Game/Cylinder.Cylinder");
	FSoftObjectPath path6("/Game/Sphere.Sphere");
	TArray<FSoftObjectPath> pathArr;
	pathArr.AddUnique(path5);
	pathArr.AddUnique(path6);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(pathArr, [pathArr, this]
	{
		for (int32 i=0; i < pathArr.Num(); ++i)
		{
			UObject* obj = pathArr[i].ResolveObject();
			UBlueprint* bp = Cast<UBlueprint>(obj);
			AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(300 + i * 300,0,0), FRotator(0,0,0));
			UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestAsyncLoad6:%d,%d"), obj != nullptr, actor != nullptr);
		}
	});	
}

void ALoadAsset::TestFStreamableManager_AsyncLoadComplete()
{
	UObject* obj = path4.ResolveObject();
	UBlueprint* bp = Cast<UBlueprint>(obj);
	AActor* actor = GetWorld()->SpawnActor<AActor>(bp->GeneratedClass, FVector(-300,6,6), FRotator(0,0,0));
	UE_LOG(LogTemp, Warning, TEXT("TestFStreamableManager_RequestAsyncLoad:%d,%d"), obj != nullptr, actor != nullptr);
}
 
void ALoadAsset::TestLoadPak()
{
	const FString PakFilePath = "E:/Test_Pak.pak";
	const FString MountPoint(*FPaths::ProjectContentDir());
	FPakPlatformFile* PakPlatformFile;

	if (!FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")))
	{
		PakPlatformFile = new FPakPlatformFile();
		PakPlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT(""));
		FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	}
	else
	{
		PakPlatformFile = (FPakPlatformFile*)FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
	}
	const TRefCountPtr<FPakFile> TmpPak = new FPakFile(PakPlatformFile, *PakFilePath, false);
	bool MountRet = PakPlatformFile->Mount(*PakFilePath, 0, *MountPoint);
	UE_LOG(LogTemp, Warning, TEXT("TestLoadPackage Mount:%d"), MountRet);
	UE_LOG(LogTemp, Warning, TEXT("TestLoadPackage MountPoint:%s, PakFilename:%s"), *TmpPak->PakGetMountPoint(), *TmpPak->PakGetPakFilename());
	TArray<FString>FoundFileNames;
	TmpPak->FindPrunedFilesAtPath(FoundFileNames, *TmpPak->GetMountPoint(),true);
	UE_LOG(LogTemp, Warning, TEXT("TestLoadPackage FoundFileNames:%d"), FoundFileNames.Num());
	for (auto FoundFileName: FoundFileNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestLoadPackage FoundFileName:%s"), *FoundFileName);
	}
}

void ALoadAsset::TestCreatePak()
{
	/*
	UnrealPak路径  pak文件路径  pak内容路径
	"C:\Program Files\Epic Games\UE_5.2\Engine\Binaries\Win64\UnrealPak.exe" "E:/Test_Pak.pak" -create="E:\Unreal Projects\Learning\Saved\Cooked\Windows\Learning\Content"

	UnrealPak路径  pak文件路径  查看pck内容
	"C:\Program Files\Epic Games\UE_5.2\Engine\Binaries\Win64\UnrealPak.exe" "E:/Test_Pak.pak" -list
	*/
}


