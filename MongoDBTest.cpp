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
