#include "OBJMesh.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <iostream>

using namespace std;
namespace sketchupparser
{
    OBJMesh::OBJMesh()
    {
    }

    OBJMesh::~OBJMesh()
    {
    }

    // trim from start (in place)
    static void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    bool OBJMesh::initializeFilename(const std::string & filenameInput)
    {
        auto file = filenameInput;
        ltrim(file);
        ifstream f(file.c_str());
        bool result = f.good();
        if (result)
            filename = file;
        return result;
    }

    bool OBJMesh::loadData()
    {
        if (filename.empty())
            return false;
        string line;
        string v;
        array<string, 3> temp;
        int n = 0;

        if (verbose)
            std::cout << "Started loading faces from file: " << getFileName() << std::endl;
        ifstream myfile(filename);// "D:\Projects\obj_sketch\sketchup-parser\testFiles\WeaponBox.obj");
        while (!myfile.eof())
        {
            getline(myfile, line);
            if (line[0] == 'v') //vertex
            {
                std::istringstream vertexLine(line);
                vertexLine >> v >> temp[0] >> temp[1] >> temp[2];

                array<PointDataType, 3> vert;
                for (int i = 0; i < 3; i++)
                {
                    if (!stringToPointDataType(temp[i], vert[i]))
                        vert[i] = 0;
                }

                vertex.push_back(vert);
            }
            else if (line[0] == 'vt') // texture
            {
                std::istringstream vertexLine(line);
                vertexLine >> v >> temp[0] >> temp[1];

                array<PointDataType, 2> tex;
                for (int i = 0; i < 2; i++)
                {
                    if (!stringToPointDataType(temp[i], tex[i]))
                        tex[i] = 0;
                }

                texture.push_back(tex);
            }
            else if (line[0] == 'vn')
            {
                std::istringstream vertexLine(line);
                vertexLine >> v >> temp[0] >> temp[1] >> temp[2];

                array<PointDataType, 3> vert;
                for (int i = 0; i < 3; i++)
                {

                    if (!stringToPointDataType(temp[i], vert[i]))
                        vert[i] = 0;
                }

                normal.push_back(vert);
            }
            else if (line[0] == 'f')
            {
                // Read face data
                std::istringstream face(line);
                std::string dummy;
                face >> dummy;
                std::vector<std::string> polygon;
                polygon.reserve(3);
                while (!face.eof())
                {
                    face >> dummy;
                    polygon.push_back(dummy);
                }
                // Fill in the containers
                std::vector<Point>polygonVec;
                polygonVec.reserve(polygon.size());
                for (auto pointString : polygon)
                {
                    std::istringstream pointStream(pointString);

                    Point p; /// v/t/n or v//n or v/t//

                    if (std::getline(pointStream, dummy, '/'))
                        p.vertex = stoull(dummy);

                    if (std::getline(pointStream, dummy, '/'))
                    {
                        if (dummy != "")
                            p.texture = stoull(dummy);
                        else
                            p.texture = 0;
                    }
                    if (std::getline(pointStream, dummy, '/'))
                    {
                        if (dummy != "")
                            p.normal = stoull(dummy);
                        else
                            p.normal = 0;
                    }
                    polygonVec.push_back(p);
                }
                //update data structure
                faces.push_back(polygonVec);
            }
        }

        if (verbose)
            std::cout << "Completed loading faces from file: " << getFileName() << std::endl;
        return true;
    }

    bool OBJMesh::stringToPointDataType(const std::string& input, PointDataType& value)
    {
        value = stod(input);
        return true;
    }
}