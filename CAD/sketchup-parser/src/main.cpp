#include "JSONImporter.h"
#include "SketchUpProducer.h"
#include "OBJMesh.h"
#include "MaterialFile.h"
#include <SketchUpAPI/common.h>
#include <SketchUpAPI/unicodestring.h>
#include <iostream>

int main(int argc, char* argv[]) {

    std::string inputCadFile;
    std::string outputCadFileInput;
    std::string scaleFactor; double scaleFactorValue = 1;
    std::string jsonFile;
    std::string directory;
    std::string materialFile;
    //Arguments to the executable
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
            continue;
        std::string argument(argv[i]);
        if (argument == "-inputfile") 
            inputCadFile = std::string(argv[i + 1]);
        if (argument == "-outputfile")
            outputCadFileInput = std::string(argv[i + 1]);
        if (argument == "-scale")
            scaleFactor = std::string(argv[i + 1]); 
        if(argument == "-inputjson")
            jsonFile = std::string(argv[i + 1]);
        if (argument == "-inputdirectory")
            directory = std::string(argv[i + 1]);
        if (argument == "-inputmaterialfile")
            materialFile = std::string(argv[i + 1]);
    }
    //Process input
    if (inputCadFile.empty() && jsonFile.empty())
    {
        std::cout << "No argument for input file is passed." << std::endl;
        return 0;
    }
    if (!jsonFile.empty() && outputCadFileInput.empty())
        outputCadFileInput = jsonFile + ".skp";
    if (!inputCadFile.empty() && outputCadFileInput.empty())
        outputCadFileInput = inputCadFile + ".skp";

    if (!scaleFactor.empty())
        scaleFactorValue = stod(scaleFactor);
    else
        scaleFactorValue = 39.3701; // convert from meter to inch by default. Sketchup unit default is inch.

    if (!directory.empty())
         directory += "/";
    /// Json parser
    std::vector<std::pair<std::string, std::string>> objList;
    modelconverter::JSONImport parser;
    if (!jsonFile.empty())
    {
        parser.ImportJSONFile(jsonFile);
        objList = parser.get_objFileName_Category();
        if (directory.empty())
        {
            auto pos = jsonFile.rfind('/');
            if (pos != std::string::npos)
            {
                directory = jsonFile.substr(0, pos);
            }
        }
            
    }
    else if (!inputCadFile.empty())
    {
        objList.push_back(std::pair<std::string, std::string>(inputCadFile, inputCadFile));
    }
    
    using namespace sketchupparser;

    /// Read material information
    MaterialFile materialInfoContainer;
    if (materialInfoContainer.initializeFilename(materialFile))
    {
        materialInfoContainer.loadData();
    }
    SketchUpProducer convertor;
    if (convertor.initlializeSketchup())
    {
        convertor.setScale(scaleFactorValue);
        for (const auto& obj_file_container : objList)
        {
            std::string filePathName = directory + obj_file_container.first + ".obj";
            
            // Read Cad
            OBJMesh cadObj;
            if (cadObj.initializeFilename(filePathName))
                cadObj.loadData();
            else
            {
                std::cout << " File can't be found at the following path:" << filePathName << std::endl;
                continue;
            }
            // adding material info
            MaterialFile::materialInfo materialDef = materialInfoContainer.getMaterialInfo(parser.getMaterialLocation(obj_file_container.first));
            std::array<char, 4> faceColor;
            if (materialDef.isValid)
            {
                for (int loop = 0; loop < 3; loop++)
                    faceColor[loop] = materialDef.Ka[loop] * 255;
                faceColor[3] = 255; //opaque
            }
            // Export Cad
            convertor.convertFromObj(cadObj, materialDef.isValid, faceColor);

        }
        convertor.exportSketchUpAs(outputCadFileInput);
    }
    return 1;
}
