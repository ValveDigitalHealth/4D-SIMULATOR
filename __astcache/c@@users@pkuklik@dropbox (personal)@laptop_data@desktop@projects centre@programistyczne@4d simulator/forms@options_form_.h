//---------------------------------------------------------------------------

#ifndef Options_Form_H
#define Options_Form_H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TOptions_Form : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Range_for_Uni_El_Edit;
	TButton *Close_Button;
	TLabel *Label2;
	TEdit *Bip_Spacing_Edit;
	void __fastcall Close_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOptions_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptions_Form *Options_Form;
//---------------------------------------------------------------------------
#endif
