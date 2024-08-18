// Fill out your copyright notice in the Description page of Project Settings.


#include "ModLoader.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManagerGeneric.h"
#include "IPlatformFilePak.h"

// Sets default values
AModLoader::AModLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AModLoader::BeginPlay()
{
	Super::BeginPlay();
    MountPakFile("Paks/mods/modtest.pak");
}

// Called every frame
void AModLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AModLoader::MountPakFile(FString PakFilePath)
{
    FString PakFullPath = FPaths::ProjectContentDir() + PakFilePath;

    if (FPaths::FileExists(PakFullPath))
    {
        // Get the platform file manager
        IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

        // Get the Pak platform file
        FPakPlatformFile* PakPlatform = new FPakPlatformFile();

        if (PakPlatform)
        {
            // Initialize the Pak file
            PakPlatform->Initialize(&PlatformFile, TEXT(""));

            // Mount the Pak file
            if (PakPlatform->Mount(*PakFullPath, 0, nullptr))
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Mounted Pak file: %s"), *PakFullPath));
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Failed to mount Pak file: %s"), *PakFullPath));
            }
        }
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("Pak file not found: %s"), *PakFullPath);
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Pak file not found: %s"), *PakFullPath));
    }
}
