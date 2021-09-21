// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class FPSGame : ModuleRules
{
    public FPSGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

        bool isLuaSupported = false;

        // Check if we are on Windows
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLuaSupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LuaPackagePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../packages/"));
            string LibrariesPath = Path.Combine(LuaPackagePath, "lua.5.4.3", "build", "native", "lib", PlatformString, "v142", "Release");

           PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "lua.lib"));
           PublicIncludePaths.Add(Path.Combine(LuaPackagePath, "lua.5.4.3", "build", "native", "include"));
        }

        PublicDefinitions.Add(string.Format("WITH_LUA_BINDING={0}", isLuaSupported ? 1 : 0));
    }
}
