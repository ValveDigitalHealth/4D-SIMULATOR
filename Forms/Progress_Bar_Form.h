//---------------------------------------------------------------------------

#ifndef Progress_Bar_FormH
#define Progress_Bar_FormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TProgress_Form : public TForm
{
__published:	// IDE-managed Components
    TProgressBar *ProgressBar1;
    TButton *Stop_Button;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall Stop_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

    bool Proceed_Flag;

    __fastcall TProgress_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProgress_Form *Progress_Form;
//---------------------------------------------------------------------------
#endif
