// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban

using System.IO;
using UnrealBuildTool;

public class OpenSSLens : ModuleRules
{
	public OpenSSLens(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		string OpenSSL111MPath = Path.Combine(ModuleDirectory, "1.1.1m");
		if (Target.Platform == UnrealTargetPlatform.HoloLens)
		{

			// Add includes
			PublicIncludePaths.Add(Path.Combine(OpenSSL111MPath, "include", "Hololens"));

			// Add Libs
			string LibPath = Path.Combine(OpenSSL111MPath, "lib", "Hololens");

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libssl.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libcrypto.lib"));
			PublicSystemLibraries.Add("crypt32.lib");
		}
		
	}
}
