#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef double PointDataType;
namespace sketchupparser
{
    struct Point
    {
        long long vertex;
        long long texture;
        long long normal;
    };

    const bool verbose = true;
}
#endif