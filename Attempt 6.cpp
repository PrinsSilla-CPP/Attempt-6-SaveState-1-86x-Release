#include <Windows.h>
#include "resource.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <gl/GL.h>
#include <gl/GLU.h>
//misschien het word tijd om GLUT en GLEW te downloaden zodra je je eerste paar grafieken kan plotten jij aarschlog
#define MAX_LOADSTRING 100

// Welke variabelen ga je hebben jij plopkoek
HINSTANCE		handleNaarInstantieOpzet; 
HWND			handleNaarParentWindow;
HDC				HandlerDeviceContext;
HGLRC			handleNaarGLRenderContext;

WCHAR			TitelVanDePagina[MAX_LOADSTRING];
WCHAR			WindowKlasse[MAX_LOADSTRING];

//Vertel dat er 
ATOM			MyRegisterClass(HINSTANCE handleNaarInstantie); //ATOM -> Acces TO Memory, goed voor memory leaks dus lol
BOOL			InitInstance(HINSTANCE, int);

void SetupPixelFormat(HDC HandlerDeviceContext);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE handleNaarInstantie, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdshow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(handleNaarInstantie, IDS_APP_TITLE, TitelVanDePagina, MAX_LOADSTRING); //IDS_APP_TITLE
	LoadStringW(handleNaarInstantie, IDC_WINDOWSKLASSE, WindowKlasse, MAX_LOADSTRING); //IDC_ATTEMPT_SIX
	handleNaarInstantieOpzet = handleNaarInstantie;

	HACCEL hAccelTable = LoadAccelerators(handleNaarInstantie, MAKEINTRESOURCE(IDC_ATTEMPT_SIX));


	if (!MyRegisterClass(handleNaarInstantie)) //voer de myregisterclass uit! en checkt ook nog of ie werkt. Anders gebeurt het volgende
	{
		MessageBox(nullptr, L"Registreren van de window class is mislukt!", L"Fout", MB_OK | MB_ICONERROR); //dit is het volgende
		return FALSE;
	}

	if (!InitInstance(handleNaarInstantie,nCmdshow)) //voer de InitieerDeInstantie uit! en checkt ook nog of ie werkt. Anders gebeurt het volgende
	{
		MessageBox(nullptr, L"Geen initiatie!", L"Fout", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	
	MSG Bericht;

	while (GetMessage(&Bericht, nullptr, 0, 0))
	{ 
		if (!TranslateAccelerator(Bericht.hwnd, hAccelTable, &Bericht))  //handleNaarParentwindow HAccelTable
		{
			TranslateMessage(&Bericht);
			DispatchMessage(&Bericht);
		}
	}

	return (int)Bericht.wParam;
}

ATOM MyRegisterClass(HINSTANCE handleNaarInstantie)
{
	WNDCLASSEXW EigenschappenVanDeWindow; //maak de classe/struct aan
	
	EigenschappenVanDeWindow.cbSize = sizeof(WNDCLASSEX);

	EigenschappenVanDeWindow.style = CS_VREDRAW | CS_HREDRAW;
	EigenschappenVanDeWindow.lpfnWndProc = WndProc;
	EigenschappenVanDeWindow.hInstance = handleNaarInstantie;
	EigenschappenVanDeWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	EigenschappenVanDeWindow.lpszClassName = WindowKlasse;
	EigenschappenVanDeWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //achtergrond kleur
	EigenschappenVanDeWindow.cbClsExtra = 0;
	EigenschappenVanDeWindow.cbWndExtra = 0;
	EigenschappenVanDeWindow.hIcon = LoadIcon(handleNaarInstantie, MAKEINTRESOURCE(IDI_ATTEMPT_SIX)); // of eigen icoon 
	EigenschappenVanDeWindow.lpszMenuName = MAKEINTRESOURCE(IDC_ATTEMPT_SIX); // of MAKEINTRESOURCE je menu id
	EigenschappenVanDeWindow.hIconSm = LoadIcon(handleNaarInstantie, MAKEINTRESOURCE(IDI_ATTEMPT_SIX)); 

	//All deze eigenschappen zijn nodig anders doet de myregisterclass functie tistisch en dat wille we nie

	return RegisterClassExW(&EigenschappenVanDeWindow); //geeft al deze eigenschappen terug want anders zeggen we well wat maar word er niks mee gedaan
}

BOOL InitInstance(HINSTANCE handleNaarInstantieOpzet, int nCmdShow)
{	handleNaarParentWindow = CreateWindowW(	WindowKlasse,		TitelVanDePagina,	WS_OVERLAPPEDWINDOW,
											CW_USEDEFAULT,		0,					CW_USEDEFAULT,
											0,					nullptr,			nullptr, 
											handleNaarInstantieOpzet,				nullptr				);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!handleNaarParentWindow) 
	{
		return FALSE;
	}
		ShowWindow(handleNaarParentWindow, nCmdShow);
		UpdateWindow(handleNaarParentWindow);
		return TRUE;
	/////////////////////////////////////////////////////////////////////
}


LRESULT CALLBACK WndProc(HWND handleNaarParentWindow, UINT message, WPARAM wParameter, LPARAM lParameter)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	switch (message)
	{
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParameter);
				// Parse the menu selections:
				switch (wmId)
					{
						case IDM_EXIT:  //dit is de beschrijving van IDM_EXIT
							DestroyWindow(handleNaarParentWindow);
						break;
						default:
							return DefWindowProc(handleNaarParentWindow, message, wParameter, lParameter);
					}


			}

			break;
		case WM_CREATE:
			{
			HandlerDeviceContext = GetDC(handleNaarParentWindow); //krijg de ka-context van je window
					//volpoepen met graphix hierzo
			SetupPixelFormat(HandlerDeviceContext);

			//klaar met de doedeldiarree
			handleNaarGLRenderContext = wglCreateContext(HandlerDeviceContext);

			wglMakeCurrent(HandlerDeviceContext, handleNaarGLRenderContext);

			glEnable(GL_DEPTH_TEST); //zet diepte aan worstenbroodje
		
			return 0;
			}

		case WM_PAINT:
			{
			PAINTSTRUCT PaintStruct;
			HDC HandlerDeviceContext = BeginPaint(handleNaarParentWindow, &PaintStruct);

				// Stel de viewport en perspectief
					glViewport(0, 0, 800, 600);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);

				// Zet camera en model
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					gluLookAt(	0, 0, 5,   // camera positie
								0, 0, 0,   // kijkpunt
								0, 1, 0);  // up vector wat dit ook is dus zoek het op kneusje (je bent geen kneus btw ily <3)

				// Scherm schoonmaken
					glClearColor(1.0f,1.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//zet dat godverdomme assenstelsel neer en kijk of dit in een void functie past returnt vgm toch niks.

					// te beginnen bij punt-O(0,0)
					glBegin(GL_POINTS); //begin het genereren van een punt - had ik deze functie maar in gesprekken.
					glPointSize(25.0f); //'maak er geen groot punt van joh'- uh zeker wel anders kan je t niet zien
					glColor3f(0.0f, 0.0f, 0.0f); //maak punt zwoaart (We discrimineren niet op kleur hoor!)
					glVertex3f(0.0f, 0.0f, 0.0f); // zet she/them op coordinaat x=0, y=0, z=0
					glEnd(); //FATALITY


					//Op naar de assen
					glBegin(GL_LINES);

					glLineWidth(5.0f); //we willen dikke pauperassen a glibba
					glColor3f(1.0f, 0.0f, 0.0f); //rood kleur
					glVertex3f(0.0f, 0.0f, 0.0f); //Startpunt van de x-as, de volgende glvertex ding is t eindpunt
					glVertex3f(1.0f,0.0f,0.0f);// de eerste as is de X-as en deze is rood
					glColor3f(0.0f, 1.0f, 0.0f); // groen kleur
					glVertex3f(0.0f, 0.0f, 0.0f);//Startpunt van de y-as, de volgende glvertex ding is t eindpunt
					glVertex3f(0.0f, 1.0f, 0.0f);// de tweede as is de Y-as en deze is groeun
					glColor3f(0.0f, 0.0f, 1.0f); // blauwie kleur
					glVertex3f(0.0f, 0.0f, 0.0f);//Startpunt van de z-as, de volgende glvertex ding is t eindpunt
					glVertex3f(0.0f, 0.0f, 1.0f);// de derde as is de Z-as en deze is blauwie!


					glEnd();
					//glVertex3f(10.0f, 0.0f, 0.0f);// de vierde as is je oma's as en deze is dood (schedel emoji)
					// 
					// 
				// Begin render � voorbeeld: een driehoek
					glBegin(GL_TRIANGLES);
					//glColor3f(1.0f, 0.0f, 0.0f); // die kleurtjes zijn wel leuk maar ze mogen nu eerst mn pik likken

					//GLfloat vertices[] = {
					//	-0.5f,	-0.5f,	0.0f,
					//	0.5f,	-0.5f,	0.0f,
					//	0.0f,	 0.5f,	0.0f
					//	};
					//glVertex3f(-1.0f, -1.0f, 0.0f);
					
					//glColor3f(0.0f, 0.0f, 0.0f);


					//glVertex3f(1.0f, -1.0f, 0.0f);

					//glColor3f(0.0f, 0.0f, 1.0f);

					//glVertex3f(0.0f, 1.0f, 0.0f);
						// Eerste driehoek
					//glColor3f(waarden);
					//glVertex3f(-1.0f, -1.0f, 0.0f); vertex 1
					//glColor3f(waarden);
					//glVertex3f(0.0f, -1.0f, 0.0f); vertex 2
					//glColor3f(waarden); 
					//glVertex3f(-0.5f, 0.0f, 0.0f);vertex 3

					// Tweede driehoek
					glColor3f(1.0f, 1.0f, 0.0f);
					glVertex3f(0.0f, -1.0f, 0.0f);
					glColor3f(1.0f, 0.0f, 1.0f);
					glVertex3f(1.0f, -1.0f, 0.0f);
					glColor3f(0.0f, 1.0f, 1.0f);
					glVertex3f(0.5f, 0.0f, 0.0f);


					glEnd();

					//GLfloat verts[] =


				// Wissel buffers
					SwapBuffers(HandlerDeviceContext);
					EndPaint(handleNaarParentWindow, &PaintStruct);
				return 0;
			}

		case WM_DESTROY:
				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(handleNaarGLRenderContext);
				ReleaseDC(handleNaarParentWindow, HandlerDeviceContext);
				PostQuitMessage(0);
			break;
			default:
				return DefWindowProc(handleNaarParentWindow, message, wParameter, lParameter);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;

}


void SetupPixelFormat(HDC HandlerDeviceContext) {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,    // color depth
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,    // depth buffer
		8,     // stencil buffer
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(HandlerDeviceContext, &pfd);
	SetPixelFormat(HandlerDeviceContext, pixelFormat, &pfd);
}
