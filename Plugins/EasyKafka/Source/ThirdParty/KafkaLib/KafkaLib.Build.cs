// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
using UnrealBuildTool;
using System;
using System.IO;

	public class KafkaLib : ModuleRules
	{
		public KafkaLib(ReadOnlyTargetRules Target) : base(Target)
		{
			Type = ModuleType.External;
		    CppStandard = CppStandardVersion.Cpp17;
		    PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

        /*
		 rdkafka++.lib is not used but shipped in case of future updates
		Only on Hololens we are using the bundled lz4
		 */

        string librdkafka = Path.Combine(ModuleDirectory, "bin");
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string LibPath = Path.Combine(ModuleDirectory, "lib/Win64");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "lz4.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "rdkafka.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "rdkafka++.lib"));

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "OpenSSL",
                "zlib"
            }
            );

        }
        else if (Target.Platform == UnrealTargetPlatform.HoloLens)
        {
            string LibPath = Path.Combine(ModuleDirectory, "lib/WinArm64");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "rdkafka.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "rdkafka++.lib"));
            /*
			 Windows Kits libs
			 */
            PublicAdditionalLibraries.Add(Path.Combine(Target.WindowsPlatform.WindowsSdkDir, "Lib", Target.WindowsPlatform.WindowsSdkVersion, "um/arm64", "kernel32.Lib"));
            PublicAdditionalLibraries.Add(Path.Combine(Target.WindowsPlatform.WindowsSdkDir, "Lib", Target.WindowsPlatform.WindowsSdkVersion, "um/arm64", "Secur32.Lib"));

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "zlib",
                "OpenSSLens"//OpenSSL libs for WinArm64 https://github.com/sha3sha3/UE-OpenSSL-Hololens.git
            }
            );

        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
            string LibPath = Path.Combine(ModuleDirectory, "lib/Linux64");
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "liblz4.a"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "librdkafka.a"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "librdkafka++.a"));


            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "OpenSSL",
                "zlib"
            }
            );
        }
		#if UE_5_0_OR_LATER
		else if(Target.Platform == UnrealTargetPlatform.LinuxArm64)
		#else
		else if (Target.Platform == UnrealTargetPlatform.LinuxAArch64)
		#endif

		{
			#if UE_5_0_OR_LATER
			string PluginBuildPath="$(PluginDir)/Binaries/LinuxArm64";
			#else
			string PluginBuildPath = "$(PluginDir)/Binaries/LinuxAArch64";
			#endif

			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/librdkafka.so"));
			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/liblz4.so"));
			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/libcrypto.so"));
			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/libssl.so"));
			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/libz.so"));
			PublicAdditionalLibraries.Add(Path.Combine(librdkafka, "Arm64/libzstd.so"));

			RuntimeDependencies.Add("$(TargetOutputDir)/librdkafka.so.1", Path.Combine(librdkafka, "Arm64/librdkafka.so"));
			RuntimeDependencies.Add("$(TargetOutputDir)/liblz4.so", Path.Combine(librdkafka, "Arm64/liblz4.so"));
			RuntimeDependencies.Add("$(TargetOutputDir)/libcrypto.so.3", Path.Combine(librdkafka, "Arm64/libcrypto.so"));
			RuntimeDependencies.Add("$(TargetOutputDir)/libssl.so.3", Path.Combine(librdkafka, "Arm64/libssl.so"));
			RuntimeDependencies.Add("$(TargetOutputDir)/libz.so.1", Path.Combine(librdkafka, "Arm64/libz.so"));
			RuntimeDependencies.Add("$(TargetOutputDir)/libzstd.so.1", Path.Combine(librdkafka, "Arm64/libzstd.so"));

			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "librdkafka.so.1"), Path.Combine(librdkafka, "Arm64/librdkafka.so"));
			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "liblz4.so"), Path.Combine(librdkafka, "Arm64/liblz4.so"));
			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "libcrypto.so.3"), Path.Combine(librdkafka, "Arm64/libcrypto.so"));
			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "libssl.so.3"), Path.Combine(librdkafka, "Arm64/libssl.so"));
			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "libz.so.1"), Path.Combine(librdkafka, "Arm64/libz.so"));
			RuntimeDependencies.Add(String.Format("{0}/{1}", PluginBuildPath, "libzstd.so.1"), Path.Combine(librdkafka, "Arm64/libzstd.so"));
		}
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);

	}

	}
