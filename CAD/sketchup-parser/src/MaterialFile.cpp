#include "MaterialFile.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>

using namespace std;
namespace sketchupparser
{
    MaterialFile::MaterialFile():filename("")
    {
        materialMap.clear();
    }


    MaterialFile::~MaterialFile()
    {
        materialMap.clear();
    }

    // trim from start (in place)
    static void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    bool MaterialFile::initializeFilename(const std::string & filenameInput)
    {
        auto file = filenameInput;
        ltrim(file);
        ifstream f(file.c_str());
        bool result = f.good();
        if (result)
            filename = file;
        return result;
    }

    bool MaterialFile::loadData()
    {
        if (filename.empty())
            return false;
        string line;
        
        ifstream myfile(filename);
        while (!myfile.eof())
        {
            getline(myfile, line);
            std::istringstream materialLine(line);
            string firstString;
            materialLine >> firstString;
            if (firstString == "newmtl")
            {
                string materialName;
                materialLine >> materialName;
                if (!materialName.empty())
                {
                    materialInfo materialContainer;
                    for (int i = 0; i < 7; i++)
                    {
                        getline(myfile, line);
                        std::istringstream readLine(line);
                        string nextWord;
                        readLine >> nextWord;

                        if (nextWord == "Kd")
                        {
                            string color[3];
                            readLine >> color[0] >> color[1] >> color[2];
                            for (int j = 0; j < 3; j++)
                            {
                                materialContainer.Kd[j] = stof(color[j]);
                            }

                        }

                        if (nextWord == "Ka")
                        {
                            string color[3];
                            readLine >> color[0] >> color[1] >> color[2];
                            for (int j = 0; j < 3; j++)
                            {
                                materialContainer.Ka[j] = stof(color[j]);
                            }

                        }

                        if (nextWord == "Ks")
                        {
                            string color[3];
                            readLine >> color[0] >> color[1] >> color[2];
                            for (int j = 0; j < 3; j++)
                            {
                                materialContainer.Ks[j] = stof(color[j]);
                            }

                        }

                        if (nextWord == "Ke")
                        {
                            string color[3];
                            readLine >> color[0] >> color[1] >> color[2];
                            for (int j = 0; j < 3; j++)
                            {
                                materialContainer.Ke[j] = stof(color[j]);
                            }

                        }


                        if (nextWord == "Ni")
                        {
                            string value;
                            readLine >> value;
                            materialContainer.Ni = stof(value);
                        }
    

                        if (nextWord == "Ns")
                        {
                            string value;
                            readLine >> value;
                            materialContainer.Ns = stof(value);
                        }


                        if (nextWord == "illum")
                        {
                            string value;
                            readLine >> value ;
                            materialContainer.illum = stof(value);
                        }

                    }
                    materialContainer.isValid = true;
                    materialMap[materialName] = materialContainer;
                }
            }
        }
        return true;
    }

    MaterialFile::materialInfo MaterialFile::getMaterialInfo(const std::string& name)
    {
        materialInfo output;
        auto it = materialMap.find(name);
        if (it != materialMap.end())
        {
            output = it->second;
        }
        else
        {
            output.isValid = false;
        }
        return output;
    }
}