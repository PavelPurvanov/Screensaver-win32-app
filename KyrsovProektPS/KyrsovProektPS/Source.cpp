#include <windows.h>
#include <scrnsave.h>
#include "resource.h"


#pragma comment(lib, "scrnsave.lib")
#pragma comment(lib, "ComCtl32.lib")

static void GetKeySettings();

int background = 1;


#define TIMER 1 


LRESULT WINAPI ScreenSaverProc(HWND hDlg, UINT message, 
                               WPARAM wParam, LPARAM lParam)
{

  
  HFONT hf;

  static HDC hDC;
  static RECT rect;

  PAINTSTRUCT ps;

  int Width, Height;

	 switch ( message ) {

  case WM_CREATE: 

    
    GetClientRect( hDlg, &rect );
    Width = rect.right;         
    Height = rect.bottom;

    GetKeySettings();
 
	
    SetTimer( hDlg, TIMER, 10, NULL ); 
    return 0;

 case WM_PAINT:{
 
			hDC = BeginPaint(hDlg, &ps);
	
			if(background == 1){
			
			HBRUSH hBrush1=CreateSolidBrush(RGB(0,0,0));
			SelectObject(hDC, hBrush1);
			Rectangle(hDC, 0, 0, 2000, 2000);
			   }
			
			else if(background == 2){
			
			
			HBRUSH hBrush1=CreateSolidBrush(RGB(0xFF,0xFF,0x00));
			SelectObject(hDC, hBrush1);
			Rectangle(hDC, 0, 0, 2000, 2000);	
			}
			
			hf = CreateFont(200, 60, 1, 1, 1, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");
		    SelectObject(hDC, hf);
		    SetBkMode(hDC, TRANSPARENT);
		    SetBkColor(hDC,RGB(0,255,0,0));
			SetTextColor(hDC,RGB(255,0,0));
			DrawText(hDC, "Technical University Varna", -1, &rect, DT_CENTER );

			
			EndPaint(hDlg, &ps);
			break;}


  case WM_DESTROY:
    KillTimer( hDlg, TIMER );
    
    return 0;
  }


  return DefScreenSaverProc( 
    hDlg, message, wParam, lParam );
}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}



BOOL WINAPI
ScreenSaverConfigureDialog(HWND hDlg, UINT message, 
                           WPARAM wParam, LPARAM lParam)
{
UNREFERENCED_PARAMETER(lParam);
  switch ( message ) 
  {

        case WM_INITDIALOG:
			
            GetKeySettings();

			CheckDlgButton(hDlg,IDC_RADIO1, MFS_CHECKED);
					
			return TRUE;

       case WM_COMMAND:
           switch( LOWORD( wParam ) ) 
                { 


                case IDOK:
			{
					
					HKEY keyh;
					DWORD dispos;
					TCHAR keyname[80];

					if (IsDlgButtonChecked(hDlg,IDC_RADIO1))	{	
					background = 1; }
						
					if (IsDlgButtonChecked(hDlg,IDC_RADIO2))	{	
					background = 2; }
					
						
						strcpy(keyname, "KyrsovProektPS");
					
						
						if (RegCreateKeyEx(HKEY_CURRENT_USER,
									keyname,					
									0,							
									"",							
									REG_OPTION_NON_VOLATILE,	
									KEY_ALL_ACCESS,						
									NULL,						
									&keyh,						
									&dispos) == ERROR_SUCCESS) { 
						
						
						RegSetValueEx(keyh, 0, 0, REG_DWORD, 
									 (BYTE *) &background, sizeof(background));
								
								RegCloseKey(keyh);

						}
				}
						EndDialog( hDlg, LOWORD( wParam ) == IDOK ); 
					  return TRUE; 

                case IDCANCEL: 
                        EndDialog( hDlg, LOWORD( wParam ) == IDOK ); 
                        return TRUE; 
                }

  }     

  return FALSE; 
}


static void GetKeySettings()
{
	
	HKEY keyh;
	TCHAR keyname[80];
	
	
	strcpy(keyname, "KyrsovProektPS");
	
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
			keyname,				
			0,						
			KEY_QUERY_VALUE,		
			&keyh) == ERROR_SUCCESS) {
		DWORD vsize = sizeof(background);
		DWORD vtype =  REG_DWORD;

		
		RegQueryValueEx(keyh, 0, NULL, &vtype, 
			(BYTE *) &background, &vsize);
		vsize = sizeof(background);

		RegCloseKey(keyh);
	} 
}

