# Mastering-Game-Development-with-Unreal-Engine-4
Mastering Game Development with Unreal Engine 4, Published by Packt

Please note, to build, you first need to right click the .uproject file and select your UE4 engine which will also generate the .sln file.

If you plan to build VS2017 project files all the time, I recommend making a batch file here with this line in it:

(Engine-install, like D:/Dev/UE4/)Engine/Binaries/DotNET/UnrealBuildTool.exe  -projectfiles -project="(Project-install directory, i.e. the one this file lives in)/Mastering.uproject" -game -engine -progress -2017

Where you insert the path to your preferred engine directory and project directory, where they're specified.  This can also be used from a command-prompt and guarantees that both your engine and project will be built as 2017 and not as VS2015 projects.  Note, though, if you right click the project file and change to a new engine, or generate project files from the Explorer fly-out menu, this will rebuild *both* game and project file by default as VS2015 again!  Something to be aware of for those with both versions installed who want to work in VS2017.