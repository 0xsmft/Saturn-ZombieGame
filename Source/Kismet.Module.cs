using System;
using System.IO;

using SaturnBuildTool;

public class KismetModule : GameModule
{
    public override void Init()
    {
        base.Init();

        // Add our source dir as an include.
        Includes.Add( "Source/Kismet" );

        // Add our source dir.
        SourcePaths.Add( "Source/Kismet" );

        // NOTE: When using UseTargetDirectory we don't have to specify a SourcePath
        SourceDirectoryOptions = SourceDirectoryOptions.Custom;
        OutputDirectoryOptions = OutputDirectoryOptions.UseTargetDirectory;
    }
}
