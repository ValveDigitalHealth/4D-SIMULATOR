//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Options_Form_.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptions_Form *Options_Form;
//---------------------------------------------------------------------------
__fastcall TOptions_Form::TOptions_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptions_Form::Close_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

