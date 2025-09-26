//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Progress_Bar_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgress_Form *Progress_Form;
//---------------------------------------------------------------------------
__fastcall TProgress_Form::TProgress_Form(TComponent* Owner)
    : TForm(Owner)
{
    Proceed_Flag = true;
}
//---------------------------------------------------------------------------
void __fastcall TProgress_Form::FormResize(TObject *Sender)
{
    Progress_Form->Width = Stop_Button->Left + Stop_Button->Width + 40;
    Progress_Form->Height = Stop_Button->Top + Stop_Button->Height + 40;
}
//---------------------------------------------------------------------------
void __fastcall TProgress_Form::Stop_ButtonClick(TObject *Sender)
{
    Proceed_Flag = false;
}
//---------------------------------------------------------------------------

