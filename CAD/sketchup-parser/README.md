# SketchUp Parser #

Component for converting a raw geometry to a Helix Model to SketchUp Model.
Windows x64

## Development Environment Setup

### Install Git
```
sudo apt-get install git
```

### Clone Repository
```
git clone git@github.com:helix-re/sketchup-parser.git
```

## Build
```
Open SketchUp-Converter.sln in Visual Studio 2015
Build Debug/Release x64 Configuration
```

## Compilation Issue
```
Delete Release and Debug folders
```

## Launching Executable
```
Using the Command Prompt
SketchUp-Converter.exe -inputjson <absolute path> [-outpufile] [<absolute path>] [-inputmaterialfile] [<absolute path>] [-scale][<value>]
example:./SketchUp-Converter.exe -inputjson "D:/Projects/obj_sketch/60EastThird/60EastThird.json" -inputdirectory "D:/Projects/obj_sketch/60EastThird" -inputmaterialfile "D:/Projects/obj_sketch/60EastThird/60EastThird.mtl" -outputfile "D:/Projects/obj_sketch/60EastThird/result.skp" 
```



