#ifndef SketchUpProducer_H
#define SketchUpProducer_H

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>

#include <vector>
#include <array>
#include "DataStructure.h"

namespace sketchupparser
{
    class OBJMesh; //forward declaration
    class SketchUpProducer
    {
    public:
        SketchUpProducer();
        SketchUpProducer(SUModelRef& model);
        ~SketchUpProducer();
        bool initlializeSketchup();
        bool convertFromObj(const OBJMesh& cadObj, const bool& isColor = false, const std::array<char, 4> &colorInput = { 0,0,0,0 });
        bool exportSketchUpAs(const std::string&);

        void setScale(const double& value) { scale = value; }
        
        //bool exportSkp

    private:
        SUModelRef model;
        PointDataType scale;
        bool localModel;

        bool addMaterial(SUFaceRef& face, const std::array<char, 4> &colorInput);
    };
}
#endif