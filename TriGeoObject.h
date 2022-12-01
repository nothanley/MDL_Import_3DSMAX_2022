#pragma once
#include "simpobj.h"
#include "Animatable.h"
#include "object.h"
#include "objectClassId.h"

Class_ID TRIGEOOBJECT_CLASS_ID = Class_ID(0x36145071, 0x636e376c);
//Class_ID TRIGEOM_CLASS_ID = Class_ID(0x6ed9143f, 0x4326d1a);

class TriGeoObject : public SimpleObject2
{
public:

    TriGeoObject() : objSize(2.0) { }

    // Member variable
    double objSize;

    // From BaseObject
    CreateMouseCallBack* GetCreateMouseCallBack() { return NULL; }

    // From SimpleObject
    void BuildMesh(TimeValue t)
    {
        ivalid = FOREVER;
    }

    ////From Animatable
    Class_ID ClassID() { return TRIGEOOBJECT_CLASS_ID; }
    SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
    //void GetClassName(TSTR& s) { s = L"Sample Geometric Object"; }

    void DeleteThis() { delete this; }
};