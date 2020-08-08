#ifndef OBJMESH_H
#define OBJMESH_H

#include <string>
#include <vector>
#include <array>
#include "DataStructure.h"
namespace sketchupparser
{
    class OBJMesh
    {
    public:
        OBJMesh();
        ~OBJMesh();
        // check if file exists
        bool initializeFilename(const std::string & filename);
        // fill up the data container
        bool loadData();

        // Convert a string to a PointDataType
        bool stringToPointDataType(const std::string&, PointDataType& value);

        /// get data
        std::vector<std::vector<Point>> getFaces()const { return faces; }
        std::vector<std::array<PointDataType, 3>> getVertices() const { return vertex; }
        std::vector<std::array<PointDataType, 2>> getTextures() const { return texture; }
        std::string getFileName()const { return filename; }

    private:
        std::string filename;
        ///<summary> OBJ data structures. Vector index is the position. </summary>
        std::vector<std::vector<Point>> faces;///  a set of points represent a face. 0 stands for invalid vakue of point structure. texture=0 imply point doesn't have texture.
        std::vector<std::array<PointDataType, 3>> vertex;// x y z
        std::vector<std::array<PointDataType, 3>> normal;
        std::vector<std::array<PointDataType, 2>> texture;
    private:
        OBJMesh(const OBJMesh&);
    };
}
#endif
