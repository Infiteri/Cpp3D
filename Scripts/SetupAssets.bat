@ECHO OFF

echo Setting up assets...

if NOT EXIST "Bin\" (
  pushd ..
)

xcopy /E /I /Y /Q "Engine\Assets" "Bin\EngineAssets"
xcopy /E /I /Y /Q "UsedAssets" "Bin\Assets"

popd 
