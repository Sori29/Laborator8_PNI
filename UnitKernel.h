//---------------------------------------------------------------------------

#ifndef UnitKernelH
#define UnitKernelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PERFGRAP.h"
#include <Buttons.hpp>
//#include <Chart.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//#include <Series.hpp>
//#include <TeEngine.hpp>
//#include <TeeProcs.hpp>
#include <Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
        TSplitter *Splitter1;
        TPanel *Panel1;
        TPanel *Panel2;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TStringGrid *StringGrid1;
	TChart *Chart2;
	TLineSeries *Series4;
	TLineSeries *Series5;
	TLineSeries *Series6;
	TPanel *Panel3;
	TImage *Image1;
	TImage *Image2;
	TChart *Chart1;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TButton *Button1;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button2;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button12;
	TButton *Button13;
	TButton *Button11;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	TButton *Button19;
	TButton *Button20;
	TButton *Button21;
	TButton *Button22;
	TCheckBox *CheckBox8;
		void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	private:	// User declarations
public:
		//User declarations
		__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
