// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Neokaiju : ModuleRules
{
	public Neokaiju(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", // Core/Default Modules
			"GameplayAbilities", "GameplayTags", "GameplayTasks", // Required for Gameplay Ability System
            "FMODStudio", // Required for FMOD Studio sounds
			"LevelSequence", "MovieScene", // Required for doing LevelSequence stuff in C++
			"Slate", "SlateCore" // Required for BindKey https://forums.unrealengine.com/development-discussion/c-gameplay-programming/47821-why-does-bindkey-cause-link-error
			});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
