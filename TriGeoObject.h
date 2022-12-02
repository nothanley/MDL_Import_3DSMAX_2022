#pragma once
#include "simpobj.h"
#include "Animatable.h"
#include "object.h"
#include "objectClassId.h"

Class_ID TRIGEOOBJECT_CLASS_ID = Class_ID(0x36145071, 0x636e376c);

extern TCHAR* GetString(int id);
extern HINSTANCE hInstance;

class TriGeoObject : public SimpleObject2
{
public:

    TriGeoObject(){ }

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



    void DeleteThis() { delete this; }


};

#include "resource.h"
#include <maxscript/maxscript.h>


class TriGeoObjectClassDesc : public ClassDesc2
{
public:
    int           IsPublic() override { return TRUE; }
    void* Create(BOOL /*loading = FALSE*/) override { return new TriGeoObject(); }
    const TCHAR* ClassName() override { return GetString(IDS_CLASS_NAME); }
    const TCHAR* NonLocalizedClassName() override { return _T("TriGeoObject"); }
    SClass_ID     SuperClassID() override { return GEOMOBJECT_CLASS_ID; }
    Class_ID      ClassID() override { return TRIGEOOBJECT_CLASS_ID; }
    const TCHAR* Category() override { return GetString(IDS_CATEGORY); }

    const TCHAR* InternalName() override { return _T("TriGeoObject"); } // Returns fixed parsable name (scripter-visible name)
    HINSTANCE     HInstance() override { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetTriGeoObjectClassDesc()
{
    static TriGeoObjectClassDesc TriGeoObjectDesc;
    return &TriGeoObjectDesc;
}