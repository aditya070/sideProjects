#pragma once
#include <map>
#include <string>
#include <array>
namespace sketchupparser
{
    class MaterialFile
    {
    public:
        MaterialFile();
        virtual ~MaterialFile();
        bool initializeFilename(const std::string & filename);
        bool loadData();
        
        struct materialInfo
        {
            std::array<float, 3> Kd;
            std::array<float, 3> Ka;
            std::array<float, 3> Ks;
            std::array<float, 3> Ke;
            float Ni;
            float Ns;
            float illum;

            bool isValid;
        };

        materialInfo getMaterialInfo(const std::string& name);
    private:
        std::string filename;
        std::map<std::string, materialInfo> materialMap;
    };
}
