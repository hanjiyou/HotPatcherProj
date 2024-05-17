@echo off

set ExecutePath="E:/linglong/ue5/Engine/Build/BatchFiles/RunUAT.bat"
set ProjectPath="E:\LearnSpace\HotPatcherDatasRoot\HotPatcherProj/HotPatcherProj.uproject"
set ArchivedDirectory="E:\LearnSpace\HotPatcherDatasRoot\HotPatcherProj\Saved\Package\Windows_ShareShader"
set LogFilePath="E:\LearnSpace\HotPatcherDatasRoot\HotPatcherProj\Saved\Package_Share.log"

echo ExecutePath %ExecutePath%
echo ProjectPath %ProjectPath%

 %ExecutePath% BuildCookRun -project=%ProjectPath% -noP4 -platform=Win64 -clientconfig=Development -build -NoCompileEditor -SkipBuildEditor ^
 -cook -SkipSoftReferences -NoGameAlwaysCook -SkipCookingEditorContent -NoDefaultMaps ^
 -stage -package -archive -archivedirectory=%ArchivedDirectory% -UTF8Output > %LogFilePath%