//**************************************************************************/
// Copyright (c) 1998-2020 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Plugin Wizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "MDL_Import_3DSMAX_2022.h"
#include "MDLReader/MDLReader.h"

#define ImportMDL_MAX_CLASS_ID Class_ID(0x2b8fbc0d, 0xad973e12)

class ImportMDL_MAX : public SceneImport
{
public:
	// Constructor/Destructor
	ImportMDL_MAX();
	virtual ~ImportMDL_MAX();

	int ExtCount() override; // Number of extensions supported
	const TCHAR* Ext(int n) override; // Extension #n (i.e. "3DS")
	const TCHAR* LongDesc() override; // Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR* ShortDesc() override; // Short ASCII description (i.e. "3D Studio")
	const TCHAR* AuthorName() override; // ASCII Author name
	const TCHAR* CopyrightMessage() override; // ASCII Copyright message
	const TCHAR* OtherMessage1() override; // Other message #1
	const TCHAR* OtherMessage2() override; // Other message #2
	unsigned int Version() override; // Version number * 100 (i.e. v3.01 = 301)
	void ShowAbout(HWND hWnd) override; // Show DLL's "About..." box
	int DoImport(const TCHAR* name, ImpInterface* i, Interface* gi, BOOL suppressPrompts = FALSE) override; // Import file
};


class ImportMDL_MAXClassDesc : public ClassDesc2 
{
public:
	int           IsPublic() override                               { return TRUE; }
	void*         Create(BOOL /*loading = FALSE*/) override         { return new ImportMDL_MAX(); }
	const TCHAR*  ClassName() override                              { return GetString(IDS_CLASS_NAME); }
	const TCHAR*  NonLocalizedClassName() override                  { return _T("ImportMDL_MAX"); }
	SClass_ID     SuperClassID() override                           { return SCENE_IMPORT_CLASS_ID; }
	Class_ID      ClassID() override                                { return ImportMDL_MAX_CLASS_ID; }
	const TCHAR*  Category() override                               { return GetString(IDS_CATEGORY); }

	const TCHAR*  InternalName() override                           { return _T("ImportMDL_MAX"); } // Returns fixed parsable name (scripter-visible name)
	HINSTANCE     HInstance() override                              { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetImportMDL_MAXDesc()
{
	static ImportMDL_MAXClassDesc ImportMDL_MAXDesc;
	return &ImportMDL_MAXDesc; 
}




INT_PTR CALLBACK ImportMDL_MAXOptionsDlgProc(HWND hWnd, UINT message, WPARAM, LPARAM lParam)
{
	static ImportMDL_MAX* imp = nullptr;

	switch (message)
	{
	case WM_INITDIALOG:
		imp = (ImportMDL_MAX*)lParam;
		CenterWindow(hWnd, GetParent(hWnd));
		return TRUE;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return 1;
	}

	return 0;
}

//--- ImportMDL_MAX -------------------------------------------------------
ImportMDL_MAX::ImportMDL_MAX()
{

}

ImportMDL_MAX::~ImportMDL_MAX()
{

}

int ImportMDL_MAX::ExtCount()
{
#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR* ImportMDL_MAX::Ext(int /*n*/)
{
#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("MDL");
}

const TCHAR* ImportMDL_MAX::LongDesc()
{
#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("WWE 2K22 3D Package");
}

const TCHAR* ImportMDL_MAX::ShortDesc()
{
#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("WWE2K Model");
}

const TCHAR* ImportMDL_MAX::AuthorName()
{
#pragma message(TODO("Return ASCII Author name"))
	return _T("");
}

const TCHAR* ImportMDL_MAX::CopyrightMessage()
{
#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR* ImportMDL_MAX::OtherMessage1()
{
	// TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR* ImportMDL_MAX::OtherMessage2()
{
	// TODO: Return other message #2 in any
	return _T("");
}

unsigned int ImportMDL_MAX::Version()
{
#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void ImportMDL_MAX::ShowAbout(HWND /*hWnd*/)
{
	// Optional
}

#include <maxscript/maxscript.h>
#include <maxscript/util/listener.h>
#include <codecvt>
#include <mesh.h>
#include "simpobj.h"
#include "TriGeoObject.h"

int ImportMDL_MAX::DoImport(const TCHAR* fileName, ImpInterface* /*importerInt*/, Interface* ip, BOOL suppressPrompts)
{
#pragma message(TODO("Implement the actual file import here and"))

	
	//read mdl
	MDLReader model;
	std::string filePath = BinaryUtils::wchar_to_string(std::wstring(fileName));
	model.openFile( filePath.c_str() );


	the_listener->edit_stream->printf(L"File: %s\n", fileName);
	for (int i = 0; i < model.getModelCount(); i++) {
		MdlSubObj mesh = model.subModels[i];
		const wchar_t* meshName = BinaryUtils::string_to_wchar(mesh.name);

		the_listener->edit_stream->printf(L"Mesh: %s\nVertices: %d\nTriangles: %d\n",
			meshName, mesh.verticeCount, mesh.faceCount);

		//creates new obj from custom class and adds it as a node
		TriGeoObject* obj = new TriGeoObject();
		INode* node = ip->CreateObjectNode(obj);

		Mesh modelMesh;
		modelMesh.setNumVerts(mesh.verticeCount);
		modelMesh.setNumFaces(mesh.faceCount);

		//collectVerts
		for (int j = 0; j < mesh.verticeCount; j++) {
			Point3 vertPos;
			vertPos.x = mesh.getVertices()[0 + (j * 3)];
			vertPos.y = mesh.getVertices()[1+ (j * 3)];
			vertPos.z = mesh.getVertices()[2 + (j * 3)];

			modelMesh.setVert(j, vertPos);
		}
		//collectTris
		for (int j = 0; j < mesh.faceCount; j++) {

			modelMesh.faces[j].setVerts(
				mesh.getTriFaces()[0 + (j * 3)],
				mesh.getTriFaces()[1 + (j * 3)],
				mesh.getTriFaces()[2 + (j * 3)]);

			modelMesh.faces[j].setEdgeVisFlags(1, 1, 0);
		}

		modelMesh.InvalidateGeomCache();

		obj->mesh = modelMesh;

		//Builds the actual geometry and inits time and matrix;
		node->SetName(meshName);
		
		
	}

	

	return true;

#pragma message(TODO("return TRUE If the file is imported properly"))

	return FALSE;
}
