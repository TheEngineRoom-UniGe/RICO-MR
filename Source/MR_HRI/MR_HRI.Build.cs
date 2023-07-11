// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MR_HRI : ModuleRules
{
	public MR_HRI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;//avoid using boost
        bUseRTTI = true;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"URoboSim", 
			"WebSockets", 
			"UROSBridge",
			"Json",
            "JsonUtilities",
			"Eigen"
			//"OpenSSL"
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
			"EasyKafka",
			"KafkaLib",
			"KafkaAdmin",
			"KafkaConsumer",
			"KafkaProducer",
            "ChunkDownloader",
            "OpenSSLens",
            "HTTP",
            "PakFile"
        });

        AddEngineThirdPartyPrivateStaticDependencies(Target,
			"Eigen"
		);

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        bEnableExceptions = true;

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
