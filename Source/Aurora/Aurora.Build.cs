// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Aurora : ModuleRules
{
	public Aurora(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Core
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", "Engine",
			"InputCore"
		});
		
		// Enhanced Input
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput"
		});
		
		// GAS
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
		
		// UI
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG"
		});
		
		// Navigation
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NavigationSystem"
		});
		
		// VFX
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Niagara"
		});
		
		// AI
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AIModule"
		});
		
		// MVVM
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ModelViewViewModel"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
