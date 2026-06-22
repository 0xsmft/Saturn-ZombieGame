using System;
using SaturnBuildTool;

public class KismetEditor : GameDevelopmentTarget
{
    public override void Init()
    {
        base.Init();

        Name = "Kismet";

        // When adding modules the name of the module has to match the file name but not the folder name...
        Modules.AddRange( new string[] {
            "Kismet"
        } );

        // Include directories relative to root folder (solution directory)
        Includes.Add( "Kismet/Source/Kismet" );
    }
}
