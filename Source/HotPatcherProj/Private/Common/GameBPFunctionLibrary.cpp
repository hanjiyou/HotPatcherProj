

#include "Common/GameBPFunctionLibrary.h"

#include "IPlatformFilePak.h"
#include "HAL/IPlatformFileModule.h"
#include "Common/HotPatcherProjLog.h"
#include "GameFramework/Character.h"

void UGameBPFunctionLibrary::CheckAndInitPakPlatform()
{
	FPakPlatformFile* HandlePakPlatform = (FPakPlatformFile*)(FPlatformFileManager::Get().FindPlatformFile(FPakPlatformFile::GetTypeName()));
	if(!HandlePakPlatform)
	{
		UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform PakPlatformFile is nullptr"));
#if WITH_EDITOR
		IPlatformFileModule* PlatformFileModule = FModuleManager::LoadModulePtr<IPlatformFileModule>(FPakPlatformFile::GetTypeName());
		if(PlatformFileModule == nullptr)
		{
			return;
		}
		HandlePakPlatform = (FPakPlatformFile*)(PlatformFileModule->GetPlatformFile());

		IPlatformFile& TopmostPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		const TCHAR* Name = TEXT("");

		for(IPlatformFile* ChainElement = &TopmostPlatformFile; ChainElement; ChainElement = ChainElement->GetLowerLevel())
		{
			UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform 寻找插入位置 Name=%s"), ChainElement->GetName());
			if(ChainElement->GetLowerLevel() == nullptr)
			{
				HandlePakPlatform->Initialize(ChainElement, TEXT(""));
				HandlePakPlatform->InitializeAfterSetActive();
				Name = ChainElement->GetName();
			}
		}

		for(IPlatformFile* ChainElement = &TopmostPlatformFile; ChainElement; ChainElement = ChainElement->GetLowerLevel())
		{
			const TCHAR* ChainElementName = ChainElement->GetName();
			UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform 寻找被插入的节点 Name=%s"), ChainElementName);
			if(ChainElement->GetLowerLevel() != nullptr && FCString::Strcmp(ChainElement->GetLowerLevel()->GetName(), Name) == 0 && FCString::Strcmp(FPakPlatformFile::GetTypeName(), Name) != 0)
			{
				UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform 找到了需要插入的节点 Name = %s OldGetLowerLevel = %s NewLowerLevel = %s"), ChainElement->GetName(), ChainElement->GetLowerLevel()->GetName(), HandlePakPlatform->GetName());
				ChainElement->SetLowerLevel(HandlePakPlatform);
				break;
			}
		}
		if(TopmostPlatformFile.GetLowerLevel() == nullptr)
		{
			UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform 没寻找到被插队的节点 HandlePakPlatform 被放在最上层！"));
			FPlatformFileManager::Get().SetPlatformFile(*HandlePakPlatform);
		}
#endif
	}
	else
	{
		UE_LOG(LogHotPatcherProj, Warning, TEXT("hhh CheckAndInitPakPlatform FPakPlatformFile is OK"));
	}
}

bool UGameBPFunctionLibrary::MountPak(const FString& PakPath, int32 PakOrder, const FString& InMountPoint)
{
	bool bMounted = false;
	FPakPlatformFile* PakFileManager = (FPakPlatformFile*)(FPlatformFileManager::Get().GetPlatformFile(FPakPlatformFile::GetTypeName()));
	if(!PakFileManager)
	{
		UE_LOG(LogHotPatcherProj, Error, TEXT("MountPak PakFileManager is nullptr"));
		return false;
	}

	PakOrder = FMath::Max(0, PakOrder);
	if(FPaths::FileExists(PakPath) && FPaths::GetExtension(PakPath) == TEXT("pak"))
	{
		bool bIsEmptyMountPoint = InMountPoint.IsEmpty();
		const TCHAR* MountPoint = bIsEmptyMountPoint ? nullptr : InMountPoint.GetCharArray().GetData();

// #if !WITH_EDITOR
		if (PakFileManager->Mount(*PakPath, PakOrder, MountPoint))
		{
			UE_LOG(LogHotPatcherProj, Log, TEXT("Mounted = %s, Order = %d, MountPoint = %s"), *PakPath, PakOrder, !MountPoint ? TEXT("(NULL)") : MountPoint);
			bMounted = true;
		}
		else {
			UE_LOG(LogHotPatcherProj, Error, TEXT("Faild to mount pak = %s"), *PakPath);
			bMounted = false;
		}
// #else
		// UE_LOG(LogHotPatcherProj, Warning, TEXT("MountPak error, pak only mount on release environment!!!"));
// #endif
		
	}
	return bMounted;
}

bool UGameBPFunctionLibrary::UnMountPak(const FString& PakPath)
{
	FPakPlatformFile* PakFileMgr = (FPakPlatformFile*)FPlatformFileManager::Get().GetPlatformFile(FPakPlatformFile::GetTypeName());
	if (!PakFileMgr)
	{
		UE_LOG(LogHotPatcherProj, Log, TEXT("GetPlatformFile(TEXT(\"PakFile\") is NULL"));
		return false;
	}

	if (!FPaths::FileExists(PakPath))
		return false;

	return PakFileMgr->Unmount(*PakPath);
}

bool UGameBPFunctionLibrary::TestSpawnPakActor(const UObject* WorldContextObject)
{
	// /Script/Engine.Blueprint'/Game/Blueprints/PakBlueprint/BP_Actor2.BP_Actor2'
	const TCHAR* ActorClassPath = TEXT("/Script/Engine.Blueprint'/Game/Blueprints/PakBlueprint/BP_Actor2.BP_Actor2_C'");
	UClass* ActorClass = LoadClass<AActor>(nullptr, ActorClassPath);
	if(!ActorClass)
	{
		return false;
	}
	UWorld* World = WorldContextObject->GetWorld();
	if(!World)
	{
		return false;
	}

	FTransform SpawnActorTransform = World->GetFirstPlayerController()->GetCharacter()->GetActorTransform();
	World->SpawnActor<AActor>(ActorClass, SpawnActorTransform);

	return true;
}
