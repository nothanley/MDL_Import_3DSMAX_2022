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
	void importSkeleton(MDLReader& model); //Imports model skeleton
	void setVertices(Mesh& mesh, MdlSubObj& model); //Sets mesh verts
	void setTriangles(Mesh& mesh, MdlSubObj& model); //Sets mesh triangles
	void setNormals(Mesh& mesh, MdlSubObj& model); //Sets mesh normals
	void setMeshSkin(Mesh& mesh, MdlSubObj& model); //Sets mesh skin
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
	return _T("WWE2K22 MDL!");
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
#include "MNNormalSpec.h"
#include "MeshNormalSpec.h"

#define BONE_FP_INTERFACE_ID Interface_ID(0x438aff72, 0xef9675ac)

int ImportMDL_MAX::DoImport(const TCHAR* fileName, ImpInterface* /*importerInt*/, Interface* ip, BOOL suppressPrompts)
{
#pragma message(TODO("Implement the actual file import here and"))

	
	//read mdl
	MDLReader model;
	std::string filePath = BinaryUtils::wchar_to_string(std::wstring(fileName));
	model.openFile( filePath.c_str() );

	//debug print inputFile
	the_listener->edit_stream->printf(L"File: %s\n", fileName);

	//build-skeleton
	if ( model.isSkinMesh() ) { importSkeleton(model); }


	for (int i = 0; i < model.getModelCount(); i++) {

		MdlSubObj mObj = model.subModels[i];
		const wchar_t* meshName = BinaryUtils::string_to_wchar(mObj.name);

		//debug print models
		the_listener->edit_stream->printf(L"Mesh: %s\nVertices: %d\nTriangles: %d\n",
			meshName, mObj.verticeCount, mObj.faceCount);

		//new obj from custom class
		TriGeoObject* obj = new TriGeoObject();
		Mesh triMesh;

		//Update MAX Mesh
		setVertices(triMesh, mObj);
		setTriangles(triMesh, mObj);
		setNormals(triMesh, mObj);

		if (mObj.isSkinned) { setMeshSkin(triMesh,mObj); }

		//Viewport Update
		triMesh.InvalidateGeomCache();

		//adds mesh to obj
		TimeValue t(0);
		obj->ConvertToType(t, EPOLYOBJ_CLASS_ID);
		obj->mesh = triMesh;

		//Builds node and inits time and matrix;
		INode* node = ip->CreateObjectNode(obj);
		node->SetName(meshName);
		
		
	}

	

	return true;

#pragma message(TODO("return TRUE If the file is imported properly"))

	return FALSE;
}


void ImportMDL_MAX::setMeshSkin(Mesh&mesh , MdlSubObj& model) {



}


void ImportMDL_MAX::importSkeleton( MDLReader& model ) {

	FPInterface * fpBones = GetCOREInterface(BONE_FP_INTERFACE_ID);	//Load BoneSys Interface
	FunctionID createBoneID = fpBones->FindFn(L"createBone");	//Looks for 'createBone' function. 

	Point3 startPos;	//init position
	Point3 endPos;
	Point3 zAxis;

	FPValue result;	//initialize params for 'createBone' function
	FPStatus status;
	
	//get model rig
	std::vector<MDLBoneOBJ> bones = model.getArmature();

	//Construct Rig
	for (int i = 0; i < model.getBoneCount(); i++) {

		MDLBoneOBJ bone = bones[i];

		glm::mat4x4 tfmMat = bone.matrix;
		if (bone.hasParent)
		{
			glm::mat4x4 parMat = bones[bone.parent].matrix;
			tfmMat = MDLBoneOBJ::dot_4x4(tfmMat, parMat);

			bone.setMatrix(tfmMat);
			bones[i].setMatrix(tfmMat);
		}

		//define new points
		startPos = Point3( bone.head[0], bone.head[1], bone.head[2]);
		endPos = Point3(bone.tail[0], bone.tail[1], bone.tail[2]);
		zAxis = Point3(bone.zAxis[0], bone.zAxis[1], bone.zAxis[2]);

		//compile parameters
		FPParams params(3, TYPE_POINT3, &startPos, TYPE_POINT3, &endPos, TYPE_POINT3, &zAxis);

		//pass the params and invoke the function
		try { status = fpBones->Invoke(createBoneID, result, &params); }
		catch (...) { /*invoke failed...*/ return; }

		//configure bone node
		if (status == FPS_OK && result.type == TYPE_INODE){
			if (INode *n = result.n){
				n->SetName( BinaryUtils::string_to_wchar(bone.name) );

				float len = Length(startPos+.001f);
				float width = .001;

				if (bone.hasParent)
				{
					wchar_t* parName = BinaryUtils::string_to_wchar(bones[bone.parent].name);
					INode *pNode = GetCOREInterface()->GetINodeByName(parName);
					pNode->AttachChild(n, 1);
				}

				n->ShowBone(1);	}

		}
	}	


	fpBones->ReleaseInterface();
}

void ImportMDL_MAX::setVertices(Mesh& mesh, MdlSubObj& model) {

	mesh.setNumVerts(model.verticeCount);
	std::vector<float> mverts = model.getVertices();

	//collectVerts
	for (int j = 0; j < model.verticeCount; j++) {
		Point3 vertPos;
		vertPos.x = mverts[0 + (j * 3)];
		vertPos.y = mverts[1 + (j * 3)];
		vertPos.z = mverts[2 + (j * 3)];

		mesh.setVert(j, vertPos);
	}

}

void ImportMDL_MAX::setTriangles(Mesh& mesh, MdlSubObj& model) {

	mesh.setNumFaces(model.faceCount);
	std::vector<int> mtris = model.getTriFaces();

	//collectTris
	for (int j = 0; j < model.faceCount; j++) {

		mesh.faces[j].setVerts(
			mtris[1 + (j * 3)],
			mtris[0 + (j * 3)],
			mtris[2 + (j * 3)]
		);

		mesh.faces[j].setEdgeVisFlags(1, 1, 1);
	}

}

void ImportMDL_MAX::setNormals(Mesh& mesh, MdlSubObj& model) {


	mesh.checkNormals(TRUE);

	// clear any normals data if existed
	if (mesh.GetSpecifiedNormals()) mesh.ClearSpecifiedNormals();

	// create new and empty
	mesh.SpecifyNormals();
	MeshNormalSpec* nspec = mesh.GetSpecifiedNormals();
	if (NULL != nspec)
	{
		// initialize the internal spec normals data
		nspec->ClearAndFree();
		nspec->SetNumFaces(model.faceCount);
		nspec->SetNumNormals(model.verticeCount);

		//iterates over every norm
		Point3* norms = nspec->GetNormalArray();
		std::vector<float> mnorms = model.getNormals();
		for (int j = 0; j < model.verticeCount; j++) {
			Point3 vertPos;
			vertPos.x = mnorms[0 + (j * 4)];
			vertPos.y = mnorms[1 + (j * 4)];
			vertPos.z = mnorms[2 + (j * 4)];

			norms[j] = (vertPos);
		}

		//iterate over every face
		MeshNormalFace* pFaces = nspec->GetFaceArray();
		std::vector<int> mtris = model.getTriFaces();
		for (int j = 0; j < model.faceCount; j++) {

			pFaces[j].SpecifyNormalID(0, mtris[1 + (j * 3)]);
			pFaces[j].SpecifyNormalID(1, mtris[0 + (j * 3)]);
			pFaces[j].SpecifyNormalID(2, mtris[2 + (j * 3)]);

		}

		// build the normals
		nspec->SetAllExplicit(true);
		nspec->CheckNormals();

	}


}