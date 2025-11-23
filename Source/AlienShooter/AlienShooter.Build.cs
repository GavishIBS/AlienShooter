// Copyright (c) 2024 Gavish Bansal
// 
// All rights reserved.

using System.IO;
using UnrealBuildBase;
using UnrealBuildTool;

public class AlienShooter : ModuleRules
{
	public AlienShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Chaos", "GeometryCollectionEngine", "ChaosSolverEngine", "FieldSystemEngine",
			"Niagara", "NavigationSystem", "PhysicsCore", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam", "ApplicationCore"});

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            //string SteamLibPath = "C:/Program Files/Epic Games/UE_5.4/Engine/Source/ThirdParty/Steamworks/Steamv157/sdk/redistributable_bin/win64";

            string EngineDir = Unreal.EngineDirectory.FullName;
            string SteamLibPath = Path.Combine(
                EngineDir,
                "Source",
                "ThirdParty",
                "Steamworks",
                "Steamv157",
                "sdk",
                "redistributable_bin",
                "win64"
            );

            PublicAdditionalLibraries.Add(SteamLibPath + "/steam_api64.lib");
            PublicDelayLoadDLLs.Add("steam_api64.dll");

            // Use NonUFS so it gets copied properly during packaging
            RuntimeDependencies.Add("$(TargetOutputDir)/steam_api64.dll", SteamLibPath + "/steam_api64.dll", StagedFileType.NonUFS);
        }


        PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
