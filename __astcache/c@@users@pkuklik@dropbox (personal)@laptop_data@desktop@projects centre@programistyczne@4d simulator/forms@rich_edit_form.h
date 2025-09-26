//---------------------------------------------------------------------------

#ifndef Rich_Edit_FormH
#define Rich_Edit_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TRichEdit_Form : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit;
	TButton *Close_Button;
	void __fastcall Close_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TRichEdit_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRichEdit_Form *RichEdit_Form;
//---------------------------------------------------------------------------
#endif
