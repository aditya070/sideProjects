#ifndef REVITPARSER_JSONIMPORTER_H
#define REVITPARSER_JSONIMPORTER_H

#include "string"
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <map>

namespace pt = boost::property_tree;

namespace modelconverter {

/****************************************************************
* Class Name : JSONImport
* File       : JSONImporter.h
* Author     : Ayush
* Created    : 11/Sep/2018
* Description: Read model data from Helix JSON file
*****************************************************************/

class JSONImport
{
public:
    JSONImport()                                            = default;
    virtual ~JSONImport()                                   = default;

    void                                                    ImportJSONFile (std::string );
    std::vector <std::pair<std::string, std::string>>       get_objFileName_Category();
    std::string                                             getMaterialLocation(const std::string& objFileLocation);
    const std::vector<std::string> &                        getFamilyNameList() const;
    const std::vector<std::string> &                        getTypeNameList() const;

private:
    void                                                    traverseJSON(boost::property_tree::ptree::const_iterator&, const pt::ptree&);
    std::vector <std::pair<std::string, std::string>>       objFileName_Category;
    std::map <std::string, std::string>                     material_Category; // filename to material map
    std::vector<std::string>                                familyNameList;
    std::vector<std::string>                                typeNameList;
};

} // namespace modelconverter
#endif //REVITPARSER_JSONIMPORTER_H