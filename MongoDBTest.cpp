#include <MongoObjectsUni.hpp>
#include "MongoDBTest.h"

#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MongoDBUniProvider"
#pragma link "Uni"
#pragma link "UniProvider"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
static const String _Server 	= "128.199.141.172";
static const int 	_Port 	= 27017;
static const String _Db      = "MTR_CMS";
static const String _auth    = "authSource=MTR_CMS";
static const String _uid     = "MTRCMS";
static const String _pwd     = "4ca72fd9a32b226c46b6247f15d57daf";

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TMongoDocument* doc = new TMongoDocument("");
//	UniTable1->Edit();
	UniQuery1->Open();
	while (! UniQuery1->Eof ) {
		doc = (TMongoDocument*)UniQuery1->GetObject("Members");
		Memo1->Lines->Add( doc->Text );
        Memo1->Lines->Add("######################");
		UniQuery1->Next();
	}
	delete doc;
}

void __fastcall TdmMongo::InitiateDB() {
	#ifdef __MACOS__
		return;
	#else
		// windows
		con->SpecificOptions->Values["MongoDB.BSONLibrary"] = "C:\\Program Files (x86)\\Devart\\UniDAC for RAD Studio 10.3\\Bin\\Win32\\libbson-1.0.dll";
		con->SpecificOptions->Values["MongoDB.ClientLibrary"] = "C:\\Program Files (x86)\\Devart\\UniDAC for RAD Studio 10.3\\Bin\\Win32\\libmongoc-1.0.dll";
		con->SpecificOptions->Values["ConnectionOptions"] = _auth;

	#endif
	con->Server 	= _Server;
	con->Port   	= _Port;
	con->Database   = _Db;
	con->Username   = _uid;
	con->Password   = _pwd;

	// tblUsers options
	tblUsers->TableName     = "Users";
	tblUsers->SpecificOptions->Values["ComplexAsString"] 	= "False";
	tblUsers->SpecificOptions->Values["AllowAddField"]      = "True";
}

//---------------------------------------------------------------------------
bool __fastcall TdmMongo::ConnectToDB() {
	try {
		InitiateDB();
		con->Open();
	} catch ( const Exception& ex ) {
		ShowMessage(ex.Message);
    }

	return con->Connected;
}
//---------------------------------------------------------------------------

