// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <IPlatformFilePak.h>
#include "BaseGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPatchCompleteDelegate, bool, Succeeded);

UCLASS()
class MR_HRI_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintPure, Category = "Patching|Stats")
		void GetLoadingProgress(int32& FilesDownloaded, int32& TotalFilesToDownload, float& DownloadPercent, int32& ChunksMounted, int32& TotalChunksToMount, float& MountPercent) const;

	UPROPERTY(BlueprintAssignable, Category = "Patching");
	FPatchCompleteDelegate OnPatchComplete;

	/** Starts the game patching process. Returns false if the patching manifest is not up to date. */
	UFUNCTION(BlueprintCallable, Category = "Patching")
		bool PatchGame();

	TSharedPtr<FPakPlatformFile> PakPlatform=nullptr;

	class IPlatformFile* OriginalPlatform;

	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsDownloadManifestUpToDate;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsPatchStarted = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsDownloadingManifest = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsManifestDownloaded = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsDownloadingPakFiles = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsPakFilesDownloaded = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsMountingPakFiles = false;
	UPROPERTY(BlueprintReadOnly, Category = "Patching|Stats")
	bool bIsPakFilesMounted = false;

protected:
	
	void OnManifestUpdateComplete(bool bSuccess);
	/** List of Chunk IDs to try and download */
	UPROPERTY(EditDefaultsOnly, Category = "Patching")
	TArray<int32> ChunkDownloadList;
	/** Called when the chunk download process finishes */
	void OnDownloadComplete(bool bSuccess);

	/** Called whenever ChunkDownloader's loading mode is finished*/
	void OnLoadingModeComplete(bool bSuccess);

	/** Called when ChunkDownloader finishes mounting chunks */
	void OnMountComplete(bool bSuccess);

	

};
