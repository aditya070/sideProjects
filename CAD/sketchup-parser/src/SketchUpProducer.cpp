#include "OBJMesh.h"
#include "SketchUpProducer.h"
#include <SketchUpAPI/model/material.h>
#include <SketchUpAPI/model/rendering_options.h>
#include <SketchUpAPI/model/typed_value.h>
#include <iostream>

namespace sketchupparser
{

    SketchUpProducer::SketchUpProducer() :model(SU_INVALID), localModel(false)
    {

    }

    SketchUpProducer::SketchUpProducer(SUModelRef& m) :model(m), localModel(false)
    {
    }


    SketchUpProducer::~SketchUpProducer()
    {
        if (localModel)
        {
            SUModelRelease(&model);
            SUTerminate();
            localModel = false;
        }
    }

    bool SketchUpProducer::initlializeSketchup()
    {
        SUInitialize();
        SUResult res = SUModelCreate(&model);
        if (res != SU_ERROR_NONE)
            return false;
        localModel = true;
        return true;
    }

    bool SketchUpProducer::convertFromObj(const OBJMesh& cadObj, const bool& isColor, const std::array<char, 4> &colorInput)
    {
        bool output = true;
        // Get the entity container of the model
        SUEntitiesRef entities = SU_INVALID;
        SUModelGetEntities(model, &entities);
        auto obj_faces = cadObj.getFaces();
        auto obj_vertices = cadObj.getVertices();

        if (verbose)
            std::cout << "Started adding faces from file: " << cadObj.getFileName()<<std::endl;
        for (auto objFaces : obj_faces)
        {
            // Create a loop input describing the vertex ordering for a face's outer loop
            SULoopInputRef outer_loop = SU_INVALID;
            SULoopInputCreate(&outer_loop);
            for (size_t i = 0; i < objFaces.size(); ++i) {
                SULoopInputAddVertexIndex(outer_loop, i);
            }
            // Create the face
            SUFaceRef face = SU_INVALID;
            std::vector<SUPoint3D> vertices;
            vertices.resize(objFaces.size());
            for (size_t i = 0; i < objFaces.size(); ++i)
            {
                auto pointArray = obj_vertices[objFaces[i].vertex - 1];
                vertices[i].x = scale * pointArray[0];
                vertices[i].y = scale * pointArray[1];
                vertices[i].z = scale * pointArray[2];
            }
            auto createFace_result=SUFaceCreate(&face, vertices.data(), &outer_loop);
            if (createFace_result == SU_ERROR_NONE)
            {
                if(isColor)
                    addMaterial(face, colorInput);
            }
            else
            {
                if(verbose)
                   std::cout << "Warning: face creation is not successful." << std::endl;
                continue;
            }
            // Add the face to the entities
            auto result = SUEntitiesAddFaces(entities, 1, &face);
            if (result != SU_ERROR_NONE)
            {
                if(verbose)
                    std::cout << "Error: adding faces from file failed: "<< cadObj.getFileName() << std::endl;
                output = false;
                continue;
            }
        }
        if (verbose)
            std::cout << "Completed adding faces from file: " << cadObj.getFileName() << std::endl;
        return output;
    }
    bool SketchUpProducer::addMaterial(SUFaceRef& face,const std::array<char, 4> &colorInput)
    {
        SUMaterialRef mat = SU_INVALID;
        SUMaterialCreate(&mat);
        SUColor color;
        color.red = colorInput[0];
        color.blue = colorInput[1];
        color.green = colorInput[2];
        color.alpha = colorInput[3];

         SUMaterialSetColor(mat, &color);

      auto result =  SUFaceSetFrontMaterial(face, mat);
      if (result != SU_ERROR_NONE)
      {
          if(verbose)
            std::cout << "There was an error adding color";
          return false;
      }
      return true;
    }

    bool SketchUpProducer::exportSketchUpAs(const std::string& outputLocation)
    {
        if (SUModelSaveToFile(model, outputLocation.c_str()) == SU_ERROR_NONE)
        {
            std::cout << "New sketch up file is created:" << outputLocation << std::endl;
            return true;
        }
        std::cout << "There was an error exporting the file." << std::endl;
        return false;
    }
}