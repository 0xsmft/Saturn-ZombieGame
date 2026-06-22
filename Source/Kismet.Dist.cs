using System;
using SaturnBuildTool;

public class KismetGame : GameDistTarget
{
    public override void Init()
    {
        base.Init();

        Name = "KismetGame";

        // When adding modules the name of the module has to match the file name but not the folder name...
        Modules.Add( "Kismet" );
    }
}
