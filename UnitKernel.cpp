//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "jpeg.hpp"
#include <algorithm>
#include "UnitKernel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "PERFGRAP"
#pragma resource "*.dfm"
TForm3* Form3;
int LUT[256];

//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject* Sender)
{
	for (int i = 0; i < StringGrid1->RowCount; i++)
		for (int j = 0; j < StringGrid1->ColCount; j++)
			StringGrid1->Cells[j][i] = 0;
	StringGrid1->Cells[StringGrid1->ColCount / 2][StringGrid1->ColCount / 2] =
		1;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::BitBtn3Click(TObject* Sender)
{
	OpenDialog1->Filter = "Bmp files(*.bmp)|*.BMP|JPEG images\(*.jpg)|*.jpg;";
	Image1->Proportional = true;
	Image2->Proportional = true;

	if (OpenDialog1->Execute()) {
		if (!FileExists(OpenDialog1->FileName))
			return; // ne asiguram ca s-a selectat un fisier
		AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
		AnsiString Ext = temp.LowerCase();
		if (Ext.AnsiPos("jpg") > 0) // este un fisier jpg
		{ //-- Decomprima jpeg-ul in imagine bitmap
			TJPEGImage* myjpeg = new TJPEGImage();
			myjpeg->LoadFromFile(OpenDialog1->FileName);
			// creaza o replica de tip bitmap
			myjpeg->DIBNeeded();
			Image1->Picture->Bitmap->Assign(myjpeg);
			delete myjpeg;
		} else if (Ext.AnsiPos("bmp") > 0)
			Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
	}
}

void __fastcall TForm3::Button1Click(TObject* Sender)
{
	RGBTRIPLE *pixels, *pixelsS, *pixelsD;
	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* dest = new Graphics::TBitmap();
	dest->Width = source->Width;
	dest->Height = source->Height;
	dest->PixelFormat = source->PixelFormat;

	int kernelSize = 3;
	const int ROWS = StringGrid1->RowCount;
	const int COLS = StringGrid1->ColCount;

	float kernel[ROWS][COLS];
	for(int i=0;i< ROWS;i++){
		for(int j=0;j<COLS;j++){
			kernel[i][j]=StrToFloat(StringGrid1->Cells[i][j]);

		}
	}
	//Introducere manuala a valorilor matrici kernel
	//double kernel[3][3] = { { 2, 0, 0 }, { 0, -1, 0 }, { 0, 0, -1 } };
	for(int y=0;y<dest->Height;y++){
		pixels = (RGBTRIPLE*)source->ScanLine[y];
		for(int x=0;x<dest->Width;x++){
			double totalR = 0,totalG = 0,totalB = 0;
			for(int i=0;i< kernelSize;i++){
				for(int j=0;j<kernelSize;j++){
					int srcX = x+i - kernelSize/2;
					int srcY = y+j - kernelSize/2;
					if(srcX>=0 && srcX < source->Width && srcY >=0 && srcY<source->Height){
						pixelsS = (RGBTRIPLE*)source->ScanLine[y];
						totalR+=kernel[i][j]*pixelsS[srcX].rgbtRed;
						totalG+=kernel[i][j]*pixelsS[srcX].rgbtGreen;
						totalB+=kernel[i][j]*pixelsS[srcX].rgbtBlue;
					}
				}
			}
			dest->Canvas->Pixels[x][y] = TColor(RGB(
			std::max(0,std::min(255,(int)totalR)),
			std::max(0,std::min(255,(int)totalG)),
			std::max(0,std::min(255,(int)totalB))));
		}
	}

	Image2->Picture->Bitmap = dest;
	delete source;
	delete dest;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::BitBtn4Click(TObject* Sender)
{
	AnsiString Ext;
	SaveDialog1->Filter =
		"Bmp files (*.bmp)|*.BMP| JPEG images (*.jpg;*.jpeg)|*.jpg;*";
	SaveDialog1->Title = "Save Image";
	SaveDialog1->DefaultExt = "jpg";
	SaveDialog1->FilterIndex = 1;
	if (SaveDialog1->Execute()) {
		AnsiString temp = ExtractFileExt(SaveDialog1->FileName);
		Ext = temp.LowerCase();
	}
	if (Ext.AnsiPos("jpg") > 0) {
		TJPEGImage* jp = new TJPEGImage();
		try {
			jp->Assign(Image2->Picture->Bitmap);
			jp->SaveToFile(SaveDialog1->FileName);
		} __finally
		{
			delete jp;
		}
	} else if (Ext.AnsiPos("bmp") > 0)
		Image2->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
}

void __fastcall TForm3::Button4Click(TObject *Sender)
{
	RGBTRIPLE *pixels, *pixelsS, *pixelsD;
	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* dest = new Graphics::TBitmap();
	dest->Width = source->Width;
	dest->Height = source->Height;
	dest->PixelFormat = source->PixelFormat;

	double kernel[3][3]={{1.0/9,1.0/9,1.0/9},
	{1.0/9,1.0/9,1.0/9},
	{1.0/9,1.0/9,1.0/9}};

	for(int y=0;y<dest->Height;y++){
		RGBTRIPLE* pixels = (RGBTRIPLE*)dest->ScanLine[y];
		for(int x=0;x<dest->Width;x++){
			double totalR=0,totalG=0,totalB=0;
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					int srcX = x + i - 1;
					int srcY = y + j - 1;
					if(srcX >= 0  && srcX < source->Width &&
					srcY >= 0  && srcY < source->Height){
						RGBTRIPLE* pixelsS = (RGBTRIPLE*)source->ScanLine[srcY];
						totalR += kernel[i][j] * pixelsS[srcX].rgbtRed;
						totalG += kernel[i][j] * pixelsS[srcX].rgbtGreen;
						totalB += kernel[i][j] * pixelsS[srcX].rgbtRed;
					}
				}
			}
			pixels[x].rgbtRed = std::max(0,std::min(255,(int)totalR));
			pixels[x].rgbtGreen = std::max(0,std::min(255,(int)totalG));
			pixels[x].rgbtBlue = std::max(0,std::min(255,(int)totalB));
		}
	}
	Image2->Picture->Bitmap = dest;
	delete source;
	delete dest;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button3Click(TObject *Sender)
{
	RGBTRIPLE *pixels, *pixelsS, *pixelsD;
	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* dest = new Graphics::TBitmap();
	dest->Width = source->Width;
	dest->Height = source->Height;
	dest->PixelFormat = source->PixelFormat;

	int kernelSize = 3;
	const int linie = StringGrid1->RowCount;
	const int col = StringGrid1->ColCount;
	double kernel[linie][col];
	for(int i=0;i<linie;i++){
		for(int j=0;j<col;j++){
			kernel[i][j] = StrToFloat(StringGrid1->Cells[i][j]);
		}
	}

	for (int y = 0; y < dest->Height; y++) {
		pixels = (RGBTRIPLE*)source->ScanLine[y];
		for (int x = 0; x < dest->Width; x++) {
			 double totalR = 0, totalG = 0, totalB = 0;
			 for (int i = 0; i < kernelSize; i++) {
				 for (int j = 0; j < kernelSize; j++) {
					 int srcX = x + i - kernelSize / 2;
					 int srcY = y + j - kernelSize / 2;
					 if (srcX >= 0 && srcX < source->Width &&
					srcY >= 0 && srcY < source->Height) {
						pixelsS = (RGBTRIPLE*)source->ScanLine[srcY];
						totalR +=kernel[i][j] * pixelsS[srcX].rgbtRed;
						totalG +=kernel[i][j] * pixelsS[srcX].rgbtGreen;
						totalB +=kernel[i][j] * pixelsS[srcX].rgbtBlue;
					 }
				}
			}
			 dest->Canvas->Pixels[x][y] = TColor(
			 RGB(std::max(0, std::min(255, (int)totalR + 128)),
			 std::max(0, std::min(255, (int)totalG + 128)),
			 std::max(0, std::min(255, (int)totalB + 128))));
		}
	}
	Image2->Picture->Bitmap = dest;
	delete source;
	delete dest;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button5Click(TObject *Sender)
{
	RGBTRIPLE *pixels, *pixelsS, *pixelsD;
	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* dest = new Graphics::TBitmap();
	dest->Width = source->Width;
	dest->Height = source->Height;
	dest->PixelFormat = source->PixelFormat;

	double wr[9] ={0},wg[9]={0},wb[9]={0};
	for(int y=1;y<dest->Height-1;y++)
	{
		for(int x=1;x<dest->Width-1;x++)
		{
			for(int i=0;i<9;i++)
			{
				pixels = (RGBTRIPLE*)source->ScanLine[y + (i/3) - 1];
				wr[i] = pixels[x + (i%3) - 1].rgbtRed;
				wg[i] = pixels[x + (i%3) - 1].rgbtGreen;
				wb[i] = pixels[x + (i%3) - 1].rgbtBlue;
			}
			std::sort(wr,wr+9);
			std::sort(wg,wg+9);
			std::sort(wb,wb+9);
			dest->Canvas->Pixels[x][y] = TColor(RGB(wr[4],wg[4],wb[4]));
        }
	}
	Image2->Picture->Bitmap = dest;
	delete source;
	delete dest;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button11Click(TObject *Sender)
{
	RGBTRIPLE *pixels, *pixelsS, *pixelsD;
	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* dest = new Graphics::TBitmap();
	dest->Width = source->Width;
	dest->Height = source->Height;
	dest->PixelFormat = source->PixelFormat;

	double vM[100]={0},iX[100]={0},iY[100]={0};
	double w[3][3]={{1.0,2.0,1.0},
	{2.0,3.0,2.0},
	{1.0,2.0,1.0}};

	for(int y=1;y<dest->Height-1;y++)
	{
		for(int x=1;x<dest->Width-1;x++)
		{
			int index=0;
			for(int i=-1;i<=1;i++)
			{
				for(int j=-1;j<=1;j++)
				{
					for(int cnt=0;cnt<w[i+1][j+1];cnt++)
					{
						RGBTRIPLE* ps = (RGBTRIPLE*)source->ScanLine[y+j];
						double m=ps[i+x].rgbtRed + ps[i+x].rgbtGreen + ps[i+x].rgbtBlue;
						iX[index] = x+i; iY[index]=y+j;
						vM[index++] = m;
                    }
                }
			}
			for(int i=0;i<index;i++)
			{
				for(int j=i+1;j<index;j++)
				{
					if(vM[j]<vM[i])
					{
						std::swap(vM[i],vM[j]);
						std::swap(iX[i],iX[j]);
						std::swap(iY[i],iY[j]);
                    }
                }
			}
			int inX = (int)iX[index / 2], inY = (int)iY[index / 2];
			dest->Canvas->Pixels[x][y] = source->Canvas->Pixels[inX][inY];
        }
	}
	Image2->Picture->Bitmap = dest;
	delete source;
	delete dest;
}
//---------------------------------------------------------------------------

