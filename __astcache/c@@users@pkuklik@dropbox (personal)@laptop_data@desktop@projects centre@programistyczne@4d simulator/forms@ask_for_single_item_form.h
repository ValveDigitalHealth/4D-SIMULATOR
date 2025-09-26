//---------------------------------------------------------------------------

#ifndef Ask_For_Single_Item_FormH
#define Ask_For_Single_Item_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TAsk_For_Single_Item_Form1 : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit_Box;
	TButton *OK_Button;
	TButton *Cancel_Button;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAsk_For_Single_Item_Form1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAsk_For_Single_Item_Form1 *Ask_For_Single_Item_Form1;
//---------------------------------------------------------------------------
#endif
