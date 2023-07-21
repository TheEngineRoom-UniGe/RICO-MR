// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"
#include "ChunkDownloader.h"
#include "Misc/CoreDelegates.h"
#include "AssetRegistryModule.h"
#include <HttpModule.h>

void UBaseGameInstance::OnManifestUpdateComplete(bool bSuccess)
{
    bIsDownloadManifestUpToDate = bSuccess;
}

void UBaseGameInstance::Init()
{
    Super::Init();

#ifdef PLATFORM_WINDOWS
    return;
#endif

	UBaseGameInstance::OriginalPlatform = &FPlatformFileManager::Get().GetPlatformFile();

	// See if the pak platform is valid
	if (!PakPlatform)
	{
		UBaseGameInstance::PakPlatform = MakeShared<FPakPlatformFile>();

	}
    
	// Initialize the pak platform file. This is where the MountPak delegate gets set.
	UBaseGameInstance::PakPlatform->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT(""));

    FPlatformFileManager::Get().SetPlatformFile(*UBaseGameInstance::PakPlatform);

    bIsDownloadingManifest = true;

	const FString DeploymentName = "Patch";
	const FString ContentBuildId = "PatchingKey";

    // initialize the chunk downloader
    TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetOrCreate();
    Downloader->Initialize("Hololens", 8);

    // load the cached build ID
    Downloader->LoadCachedBuild(DeploymentName);

    // update the build manifest file
    TFunction<void(bool bSuccess)> UpdateCompleteCallback = [&](bool bSuccess) 
    {
        bIsDownloadManifestUpToDate = bSuccess;
        bIsManifestDownloaded = true;
    };
    Downloader->UpdateBuild(DeploymentName, ContentBuildId, UpdateCompleteCallback);
}

void UBaseGameInstance::Shutdown()
{
    Super::Shutdown();

    // Shut down ChunkDownloader
    FChunkDownloader::Shutdown();
}

void UBaseGameInstance::GetLoadingProgress(int32& BytesDownloaded, int32& TotalBytesToDownload, float& DownloadPercent, int32& ChunksMounted, int32& TotalChunksToMount, float& MountPercent) const
{

#ifdef PLATFORM_WINDOWS
    return;
#endif

    //Get a reference to ChunkDownloader
    TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();

    //Get the loading stats struct
    FChunkDownloader::FStats LoadingStats = Downloader->GetLoadingStats();

    //Get the bytes downloaded and bytes to download
    BytesDownloaded = LoadingStats.BytesDownloaded;
    TotalBytesToDownload = LoadingStats.TotalBytesToDownload;

    //Get the number of chunks mounted and chunks to download
    ChunksMounted = LoadingStats.ChunksMounted;
    TotalChunksToMount = LoadingStats.TotalChunksToMount;

    //Calculate the download and mount percent using the above stats
    if (bIsPakFilesDownloaded)
        DownloadPercent = 1.0;
    else
        DownloadPercent = (float)BytesDownloaded / (float)TotalBytesToDownload;
    MountPercent = (float)ChunksMounted / (float)TotalChunksToMount;

}

void UBaseGameInstance::OnLoadingModeComplete(bool bSuccess)
{
    OnDownloadComplete(bSuccess);
}

void UBaseGameInstance::OnMountComplete(bool bSuccess)
{
    bIsPakFilesMounted = true;
    // Set the original platform file again after doing the pak file update.
    FPlatformFileManager::Get().SetPlatformFile(*UBaseGameInstance::OriginalPlatform);
    UBaseGameInstance::PakPlatform->~FPakPlatformFile();

    OnPatchComplete.Broadcast(bSuccess);
}

bool UBaseGameInstance::PatchGame()
{
    // make sure the download manifest is up to date
    if (bIsDownloadManifestUpToDate && !bIsPatchStarted)
    {
        bIsDownloadingPakFiles = true;
        
        // get the chunk downloader
        TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();

        // report current chunk status
        for (int32 ChunkID : ChunkDownloadList)
        {
            int32 ChunkStatus = static_cast<int32>(Downloader->GetChunkStatus(ChunkID));
            UE_LOG(LogTemp, Display, TEXT("Chunk %i status: %i"), ChunkID, ChunkStatus);
        }

        TFunction<void(bool bSuccess)> DownloadCompleteCallback = [&](bool bSuccess) {OnDownloadComplete(bSuccess); };
        Downloader->DownloadChunks(ChunkDownloadList, DownloadCompleteCallback, 1);

        // start loading mode
        TFunction<void(bool bSuccess)> LoadingModeCompleteCallback = [&](bool bSuccess) {OnLoadingModeComplete(bSuccess); };
        Downloader->BeginLoadingMode(LoadingModeCompleteCallback);
        return true;
    }

    // we couldn't contact the server to validate our manifest, so we can't patch
    UE_LOG(LogTemp, Display, TEXT("Manifest Update Failed. Can't patch the game"));

    return false;

}

void UBaseGameInstance::OnDownloadComplete(bool bSuccess)
{
    bIsPakFilesDownloaded = true;

    if (bSuccess && !bIsPatchStarted)
    {
        bIsMountingPakFiles = true;
        UE_LOG(LogTemp, Display, TEXT("Download complete"));

        bIsPatchStarted = true;

        // get the chunk downloader
        TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();

        FJsonSerializableArrayInt DownloadedChunks;

        for (int32 ChunkID : ChunkDownloadList)
        {
            DownloadedChunks.Add(ChunkID);
        }

        //Mount the chunks
        TFunction<void(bool bSuccess)> MountCompleteCallback = [&](bool bSuccess) {OnMountComplete(bSuccess); };
        Downloader->MountChunks(DownloadedChunks, MountCompleteCallback);

        OnPatchComplete.Broadcast(true);


    }
    else if(!bIsPatchStarted)
    {

        UE_LOG(LogTemp, Display, TEXT("Load process failed"));

        // call the delegate
        OnPatchComplete.Broadcast(false);
    }


}