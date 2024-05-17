#pragma once

#include "CoreMinimal.h"
#include "GameBPFunctionLibrary.generated.h"

UCLASS()
class HOTPATCHERPROJ_API UGameBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category=Asset)
	static void CheckAndInitPakPlatform();

	UFUNCTION(BlueprintCallable)
	static bool MountPak(const FString& PakPath, int32 PakOrder, const FString& InMountPoint = TEXT(""));

	UFUNCTION(BlueprintCallable)
	static bool UnMountPak(const FString& PakPath);

	UFUNCTION(BlueprintCallable)
	static bool TestSpawnPakActor(const UObject* WorldContextObject);
};