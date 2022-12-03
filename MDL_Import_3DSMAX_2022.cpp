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
#include "TriGeoObject.h"
#include "MTLReader.h"


#define ImportMDL_MAX_CLASS_ID Class_ID(0x2b8fbc0d, 0xad973e12)

#include <maxtypes.h>
#include <stdmat.h>
#include <mtl.h>
#include <maxscript/maxscript.h>
#include <maxscript/util/listener.h>
#include <codecvt>
#include <mesh.h>
#include "simpobj.h"
#include "MNNormalSpec.h"
#include "MeshNormalSpec.h"
#include <iskin.h>
#include "modstack.h"
#include "triobj.h"
#include <experimental/filesystem>
#pragma once

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
	void importSkeleton(MDLReader& model, INode* rootNode); //Imports model skeleton
	void setVertices(Mesh& mesh, MdlSubObj& model); //Sets mesh verts
	void setTriangles(Mesh& mesh, MdlSubObj& model); //Sets mesh triangles
	void setNormals(Mesh& mesh, MdlSubObj& model); //Sets mesh normals
	void setMeshSkin(INode* node, MDLReader& model, MdlSubObj&mObj); //Sets mesh skin
	void setMaps(Mesh& mesh, MdlSubObj& model); //Sets mesh uvs
	void setMaterial( MdlSubObj& model, INode*node, std::vector<StdMat2*>* sceneMaterials, std::vector<string>* sceneMatNames); //Sets mesh material
	void setMatLib( std::string path , MTLReader& mdlMtls, std::vector<StdMat2*>* mats, std::vector<string>* matNames); //Sets mesh material
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



#define BONE_FP_INTERFACE_ID Interface_ID(0x438aff72, 0xef9675ac)
# define M_PI           3.14159265358979323846f  /* pi */

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


std::string fpString( std::string filePath ) {

	std::size_t botDirPos = filePath.find_last_of("\\");
	std::string file = filePath.substr(botDirPos, filePath.length());

	file.erase(std::remove(file.begin(), file.end(), '\\'), file.end());

	size_t lastindex = file.find_last_of(".");
	file = file.substr(0, lastindex);

	return file;
}

std::string dirString(std::string filePath) {

	std::size_t botDirPos = filePath.find_last_of("\\");
	std::string dir = filePath.substr(0, botDirPos);

	return dir;
}


int ImportMDL_MAX::DoImport(const TCHAR* fileName, ImpInterface* /*importerInt*/, Interface* ip, BOOL suppressPrompts)
{
#pragma message(TODO("Implement the actual file import here and"))

	
	//read mdl
	MDLReader model;
	MTLReader modelMtls;
	std::string filePath = BinaryUtils::wchar_to_string(std::wstring(fileName));
	model.openFile( filePath.c_str() );

	//check mats
	bool hasMats = false;
	std::string matPath = BinaryUtils::ReplaceAll( BinaryUtils::str_tolower(filePath), std::string(".mdl") , std::string(".mtls") );
	if (std::experimental::filesystem::exists(matPath)) {
		modelMtls.openFile(matPath.c_str());
		hasMats = true;
	}

	//debug print inputFile
	wchar_t* fName = BinaryUtils::string_to_wchar(fpString(filePath));
	the_listener->edit_stream->printf(L"File: %s\n", fileName);

	//build scene root
	TriGeoObject* rootObj = new TriGeoObject();
	INode* rootNode = ip->CreateObjectNode(rootObj);
	rootNode->SetName(fName);

	//build-skeleton
	if ( model.isSkinMesh() ) { importSkeleton(model, rootNode); }

	//build material library
	std::vector<StdMat2*>* sceneMaterials = new std::vector<StdMat2*>;
	std::vector<string>* sceneMatNames = new std::vector<string>;
	if (hasMats) { setMatLib( matPath , modelMtls , sceneMaterials, sceneMatNames);}

	for (int i = 0; i < model.getModelCount(); i++) {

		MdlSubObj mObj = model.subModels[i];
		const wchar_t* modelName = BinaryUtils::string_to_wchar(mObj.name);
		const wchar_t* meshName = BinaryUtils::string_to_wchar(mObj.meshName);

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
		setMaps(triMesh, mObj);

		//Viewport Update
		triMesh.InvalidateGeomCache();

		//adds mesh to obj
		TimeValue t(0);
		obj->mesh = triMesh;

		//Builds object node and inits time and matrix;
		INode* node = ip->CreateObjectNode(obj);
		node->SetName(meshName);

		//attach to root
		rootNode->AttachChild(node,1);

		//set Skin  - this is pretty slow, this could be optimized
		if (mObj.isSkinned) { setMeshSkin(node,model,mObj); }

		//set Material
		if (hasMats) { setMaterial( mObj, node, sceneMaterials, sceneMatNames); }
		ip->RedrawViews(ip->GetTime());

	}

	//rotate root
	{
		Matrix3 rot; rot.SetRotate(0, 0, 0); rot.SetScale(Point3(0, 0, 0));
		rot.SetTranslate(Point3(0, 0, 0)); rot.SetRotateX(-90 * M_PI / 180);
		rootNode->SetNodeTM(0, rot);
	}

	return true;

#pragma message(TODO("return TRUE If the file is imported properly"))

	return FALSE;
}

#include <bitmap.h>

void ImportMDL_MAX::setMatLib( std::string filePath, MTLReader& mdlMtls 
	, std::vector<StdMat2*>* mats , std::vector<string>* matNames ) {

	//check tex path
	std::string texPath = dirString(filePath) + "\\textures\\";

	for (int i = 0; i < mdlMtls.materialCount; i++) {

		MDLMaterial mat = mdlMtls.materials[i];
		std::string colorMap = mat.diffuse;

		//create default material
		StdMat2 *m = NewDefaultStdMat();
		m->SetName(BinaryUtils::string_to_wchar(mat.name));
		m->SetMtlFlag(MTL_DISPLAY_ENABLE_FLAGS, TRUE);

		//ambient mat
		m->SetAmbient(Color(0.588f, 0.588f, 0.588f), 0);
		m->SetDiffuse(Color(0.588f, 0.588f, 0.588f), 0);
		m->SetSpecular(Color(0.902f, 0.902f, 0.902f), 0);

		//getMTLsPath
		std::string bmpPath = texPath + colorMap + ".dds";
		std::string pngPath = texPath + colorMap + ".png";

		//setDiffuseTex
		if (std::experimental::filesystem::exists(bmpPath)){	//.dds
			BitmapTex *bmp = NewDefaultBitmapTex();
			bmp->SetMapName( BinaryUtils::string_to_wchar(bmpPath) );
			m->SetSubTexmap(ID_DI, bmp);
			mats->push_back(m); matNames->push_back(mat.name);
		}
		else if (std::experimental::filesystem::exists(pngPath)) { //.png
			BitmapTex* bmp = NewDefaultBitmapTex();
			bmp->SetMapName(BinaryUtils::string_to_wchar(pngPath));
			m->SetSubTexmap(ID_DI, bmp);
			mats->push_back(m); matNames->push_back(mat.name);
		}
		else {
			/* */
		}


	}

}

void ImportMDL_MAX::setMaterial( MdlSubObj& model, INode*node, 
	std::vector<StdMat2*>* sceneMaterials, std::vector<string>* matNames ) {

	std::string modelName = model.meshName;

	//find mat
	std::string modelID = BinaryUtils::ReplaceAll(BinaryUtils::str_tolower(modelName), std::string(":skinned"), std::string(""));
	modelID = BinaryUtils::ReplaceAll(modelID, std::string("_ALPHA"), std::string(""));
	auto item = std::find(matNames->begin(), matNames->end(), modelID);

	//add mat if exists
	if (item != matNames->end()) {
		int index = item - matNames->begin();

		StdMat2* modelMat = sceneMaterials->at(index);
		node->SetMtl(modelMat);
	}
	else {
		//create default material
		StdMat2* defaultMat = NewDefaultStdMat();
		defaultMat->SetName(BinaryUtils::string_to_wchar(modelID));
		defaultMat->SetMtlFlag(MTL_DISPLAY_ENABLE_FLAGS, TRUE);

		//ambient mat
		defaultMat->SetAmbient(Color(0.588f, 0.588f, 0.588f), 0);
		defaultMat->SetDiffuse(Color(0.588f, 0.588f, 0.588f), 0);
		defaultMat->SetSpecular(Color(0.902f, 0.902f, 0.902f), 0);

		node->SetMtl(defaultMat);
	}
	

}



void ImportMDL_MAX::setMaps(Mesh& mesh, MdlSubObj& mObj) {

	//sets map count
	int n = mObj.verticeCount;
	int f = mObj.faceCount;
	std::vector<int> mtris = mObj.getTriFaces();

	mesh.setNumMaps(mObj.uvCount+1, TRUE);
	mesh.setNumTVerts(n);
	mesh.setNumTVFaces(f);

	//apply map texcoords 
	for (int j = 1; j <= mObj.uvCount; j++) {

		std::vector<float> modelUVs = mObj.getUVMap(j-1);

		mesh.setMapSupport(j, TRUE);
		mesh.setNumMapVerts(j, n, FALSE);
		mesh.setNumMapFaces(j, f, FALSE);

		//tverts
		for (int i = 0; i < n; i++) {
			float u = modelUVs[(i * 2)];
			float v = modelUVs[(i * 2) + 1];
			float w = 1.0f;

			mesh.setMapVert(j, i, UVVert(u, -v+1.0f, w));
		}

		//tvface
		for (int k = 0; k < f; k++) {

			TVFace* mFaces = mesh.mapFaces(j);
			TVFace* mFace = &mFaces[k];

			mFace->setTVerts(
				mtris[1 + (k * 3)],
				mtris[0 + (k * 3)],
				mtris[2 + (k * 3)]);

		}

	}

}



// Locate a TriObject in an Object if it exists
TriObject* GetTriObject(Object* o)
{
	if (o && o->CanConvertToType(triObjectClassID))
		return (TriObject*)o->ConvertToType(0, triObjectClassID);
	while (o->SuperClassID() == GEN_DERIVOB_CLASS_ID && o)
	{
		IDerivedObject* dobj = (IDerivedObject*)(o);
		o = dobj->GetObjRef();
		if (o && o->CanConvertToType(triObjectClassID))
			return (TriObject*)o->ConvertToType(0, triObjectClassID);
	}
	return nullptr;
}

Modifier* GetSkin(INode* node)
{
	Object* pObj = node->GetObjectRef();
	if (!pObj) return nullptr;
	while (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* pDerObj = (IDerivedObject*)(pObj);
		int Idx = 0;
		while (Idx < pDerObj->NumModifiers())
		{
			// Get the modifier. 
			Modifier* mod = pDerObj->GetModifier(Idx);
			if (mod->ClassID() == SKIN_CLASSID)
			{
				// is this the correct Physique Modifier based on index?
				return mod;
			}
			Idx++;
		}
		pObj = pDerObj->GetObjRef();
	}
	return nullptr;
}


// Get or Create the Skin Modifier
Modifier* GetOrCreateSkin(INode* node)
{
	Modifier* skinMod = GetSkin(node);
	if (skinMod)
		return skinMod;

	Object* pObj = node->GetObjectRef();
	IDerivedObject* dobj = nullptr;
	if (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		dobj = static_cast<IDerivedObject*>(pObj);
	else {
		dobj = CreateDerivedObject(pObj);
	}
	//create a skin modifier and add it
	skinMod = (Modifier*)CreateInstance(OSM_CLASS_ID, SKIN_CLASSID);
	dobj->SetAFlag(A_LOCK_TARGET);
	dobj->AddModifier(skinMod);
	dobj->ClearAFlag(A_LOCK_TARGET);
	node->SetObjectRef(dobj);
	return skinMod;
}

void ImportMDL_MAX::setMeshSkin( INode *node , MDLReader &model , MdlSubObj &mObj ) {

	//create a skin modifier and add it
	Modifier* skinMod = GetOrCreateSkin(node);
	TriObject* triObject = GetTriObject(node->GetObjectRef());
	Mesh& m = triObject->GetMesh();

	if (ISkin* skin = (ISkin*)skinMod->GetInterface(I_SKIN)) {
		ISkinImportData* iskinImport = (ISkinImportData*)skinMod->GetInterface(I_SKINIMPORTDATA);

		// Set the num weights to 4.
		int numBonesPerVertex = 4;
		int numWeightsPerVertex = 4;

		//v5+ dependency
		IParamBlock2* params = skinMod->GetParamBlockByID(2/*advanced*/);
		params->SetValue(numBonesPerVertex, 0, numWeightsPerVertex);

		//v6+ dependency
		BOOL ignore = TRUE;
		params->SetValue(0xE/*ignoreBoneScale*/, 0, ignore);


		// Create Bone List (Should only use affected)
		Tab<INode*> bones;
		for (size_t i = 0; i < mObj.weightedBones.size() ; ++i) {
			
			int wIndex = mObj.weightedBones[i];
			MDLBoneOBJ bone = model.bones[wIndex];
			
			INode* boneRef = GetCOREInterface()->GetINodeByName(
					BinaryUtils::string_to_wchar(bone.name)	);

			bones.Append(1, &boneRef);
			iskinImport->AddBoneEx(boneRef, TRUE);

		}

		ObjectState os = node->EvalWorldState(0);


		//Assign Weights
		std::vector<float> bi = mObj.blendIndices;
		std::vector<float> bw = mObj.blendWeights;

		for (size_t i = 0; i < mObj.verticeCount; ++i) {

			Tab<INode*> wBones;
			Tab<float> weights;

			for (int j = 0; j < 4; j++) 
			{
				int boneIndex = bi[(i * 4) + j];
				float boneWeight = bw[(i * 4) + j];
				MDLBoneOBJ bone = model.bones[boneIndex];

				INode* boneRef = GetCOREInterface()->GetINodeByName( BinaryUtils::string_to_wchar(bone.name) );
				wBones.Append(1, &boneRef);
				weights.Append(1, &boneWeight);
			} 

			BOOL add = iskinImport->AddWeights(node, i, wBones, weights);

		}

		node->EvalWorldState(0);

	}

}



void ImportMDL_MAX::importSkeleton( MDLReader& model , INode* rootNode ) {

	FPInterface * fpBones = GetCOREInterface(BONE_FP_INTERFACE_ID);	//Load BoneSys Interface
	FunctionID createBoneID = fpBones->FindFn(L"createBone");	//Looks for 'createBone' function. 

	Point3 startPos;	//init position
	Point3 endPos;
	Point3 zAxis;

	FPValue result;	//initialize params for 'createBone' function
	FPStatus status;
	
	//get model rig
	std::vector<MDLBoneOBJ> bones = model.getArmature();
	std::vector<INode*> boneNodes; /* Use a vector to avoid potential conflicts with existing bones. */

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
				boneNodes.push_back(n);

				if (bone.hasParent)
				{
					int parIndex = bones[bone.parent].id;
					INode *pNode = boneNodes[parIndex];
					pNode->AttachChild(n, 1);
				}
				else {
					rootNode->AttachChild(n, 1);
				}

				n->BoneAsLine(1);
				n->SetWireColor(RGB(150, 150, 150));
				n->ShowBone(2);	
			}

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

	int bufType = 3;
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

		if ( model.normals.size() != model.getVertices().size() ) { bufType = 4; }	//RGBA-8 vs float-32 bit storage

		//iterates over every norm
		Point3* norms = nspec->GetNormalArray();
		std::vector<float> mnorms = model.getNormals();
		for (int j = 0; j < model.verticeCount; j++) {
			Point3 vertPos;
			vertPos.x = mnorms[0 + (j * bufType)];
			vertPos.y = mnorms[1 + (j * bufType)];
			vertPos.z = mnorms[2 + (j * bufType)];

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