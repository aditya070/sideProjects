#include "JSONImporter.h"
//#include "logger.h"
#include "JSONNodes.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <JSONImporter.h>
#include<iostream>

// Short alias for namespace
namespace mc = modelconverter;
namespace pt = boost::property_tree;

/**
 * Imports JSON file data
 * @param fileName
 */
void mc::JSONImport::ImportJSONFile(std::string fileName)
{
    try
    {
        // Create a root
        pt::ptree root;

        // Load the json file in this ptree
        pt::read_json(fileName, root);

        boost::property_tree::ptree::const_iterator v = root.begin();
        traverseJSON(v, root);
        std::cout<< "pause: Traversing json file.";
    }
    catch(std::exception & err)
    {
        std::string strError = {err.what()};
        std::cout<<"Error: " << strError<< std::endl;
    }
    catch (...)
    {
        std::cout<<"Unknown Error using Teigha BIM"<<std::endl;
    }
}

/**
 * Traverses JSON file recursively and fetches obj file name and element category
 * @param v
 * @param tree
 */
void mc::JSONImport::traverseJSON(boost::property_tree::ptree::const_iterator& v, const pt::ptree& tree)
{
    if(tree.end() == v)
    {
        return;
    }
    else
    {
        if(v->second.empty())
        {
            // normal key:value
            std::string key = v->first;
            std::string value = v->second.data();

            std::string categoryCheck;
            // Read obj file name and element category
            try
            {
                categoryCheck = tree.get<std::string>(GEOMETRY, "not found");    //not found string for all other cases when for elementGeometries node is not present
            }
            catch (std::exception & err)
            {
                std::string strError = { err.what() };
                std::cout << "Error While Traversing: " << strError << std::endl;
                key = "";
                value = "";
            }
            if( key == PRIMITIVE && categoryCheck.empty() )
            {
                auto location = tree.get<std::string>(GEOMETRY + ".." + MESH + ".." + LOCATION);
                objFileName_Category.push_back(std::pair <std::string, std::string> (location, value));
                material_Category[location]= tree.get<std::string>(GEOMETRY + ".." + MESH + ".." + MATERIAL);
            }

            // Read family name and type name of element
            if( key == NAME && value == "ALL_MODEL_FAMILY_NAME")
            {
                std::string familyName;
                familyName = tree.get<std::string>(VALUE, "not found");
                familyNameList.push_back(familyName);
            }
            if( key == NAME && value == "ELEM_TYPE_PARAM")
            {
                std::string typeName;
                typeName = tree.get<std::string>(VALUE, "not found");
                typeNameList.push_back(typeName);
            }
        }
        else
        {
            // subtree
            auto subTree = v->second;
            boost::property_tree::ptree::const_iterator vSubtree = subTree.begin();
            traverseJSON(vSubtree, subTree);
        }
        traverseJSON(++v, tree);
    }
}

/**
 * Getter functon for objFileName_Category
 * @return
 */
std::vector<std::pair<std::string, std::string>> mc::JSONImport::get_objFileName_Category()
{
    return objFileName_Category;
}

/**
 * Getter functon for family list
 * @return
 */
const std::vector<std::string> &mc::JSONImport::getFamilyNameList() const {
    return familyNameList;
}

/**
 * Getter functon for type list
 * @return
 */
const std::vector<std::string> &mc::JSONImport::getTypeNameList() const {
    return typeNameList;
}

std::string   mc::JSONImport::getMaterialLocation(const std::string& objFileLocation)
{
    auto it = material_Category.find(objFileLocation);
    if (it != material_Category.end())
        return it->second;
    return std::string("");
}