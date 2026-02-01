using System;
using SaturnBuildTool;

public class KismetGame : Target
{
    public override void Init()
    {
        base.Init();

        Name = "KismetGame";
        Architectures = new[] { ArchitectureKind.x86_64 };

        BuildConfigs = new[] { ConfigKind.Dist };
        OutputType = LinkerOutput.Executable;

        // When adding modules the name of the module has to match the file name but not the folder name...
        Modules.AddRange( new string[] {
            "Saturn",
            "Kismet"
        } );

        Links.AddRange( new string[] {
            "ole32.lib",
            "kernel32.lib",
            "comdlg32.lib",
            "shell32.lib",
            "Advapi32.lib",
            "Saturn.lib",
        } );

    }
}
