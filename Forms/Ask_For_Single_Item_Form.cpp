//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ask_For_Single_Item_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAsk_For_Single_Item_Form1 *Ask_For_Single_Item_Form1;
//---------------------------------------------------------------------------
__fastcall TAsk_For_Single_Item_Form1::TAsk_For_Single_Item_Form1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAsk_For_Single_Item_Form1::Cancel_ButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAsk_For_Single_Item_Form1::OK_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAsk_For_Single_Item_Form1::FormResize(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Width = OK_Button->Left + OK_Button->Width + 100;
	Ask_For_Single_Item_Form1->Height = OK_Button->Top + OK_Button->Height + 100;
}
//---------------------------------------------------------------------------
