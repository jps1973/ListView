// ListView.cpp

#include "ListView.h"

// Global variables
HWND g_hWndListView;
HWND g_hWndStatusBar;

int ListViewWindowAddColumns()
{
	int nResult = 0;

	int nWhichColumn;
	LVCOLUMN lvColumn;

	LPCTSTR lpszColumnTitles[] = LIST_VIEW_WINDOW_COLUMN_TITLES;

	// Clear list view column structure
	ZeroMemory( &lvColumn, sizeof( lvColumn ) );

	// Initialise list view column structure
	lvColumn.mask	= ( LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM );
	lvColumn.cx		= LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

	// Loop through columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Update list view column structure for column
		lvColumn.pszText = ( LPTSTR )( lpszColumnTitles[ nWhichColumn ] );

		// Insert column
		SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

	}; // End of loop through columns

	return nResult;

} // End of function ListViewWindowAddColumns

LRESULT CALLBACK MainWndProc( HWND hWndMain, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Create list view window
			g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, WC_LISTVIEW, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndMain, ( HMENU )NULL, hInstance, NULL );

			// Ensure that list view window was created
			if( g_hWndListView )
			{
				// Successfully created list view window

				// Set extended list view window style
				SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )NULL, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

				// Set list view window font
				SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

				// Add columns to list view window
				ListViewWindowAddColumns();

				// Create status bar window
				g_hWndStatusBar = CreateWindowEx( STATUS_BAR_WINDOW_EXTENDED_STYLE, STATUSCLASSNAME, STATUS_BAR_WINDOW_TEXT, STATUS_BAR_WINDOW_STYLE, 0, 0, 0, 0, hWndMain, ( HMENU )NULL, hInstance, NULL );

				// Ensure that status bar window was created
				if( g_hWndStatusBar )
				{
					// Successfully created status bar window

					// Set status bar window font
					SendMessage( g_hWndStatusBar, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

				} // End of successfully created status bar window

			} // End of successfully created list view window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nListViewWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			SendMessage( g_hWndStatusBar, WM_SIZE, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get status window size
			GetWindowRect( g_hWndStatusBar, &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nListViewWindowHeight	= ( nClientHeight - nStatusWindowHeight );

			// Move list view window
			MoveWindow( g_hWndListView, 0, 0, nClientWidth, nListViewWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on list view window
			SetFocus( g_hWndListView );

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			UINT uFileCount;
			HDROP hDrop;
			UINT uWhichFile;
			UINT uFileSize;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

			// Get drop handle
			hDrop = ( HDROP )wParam;

			// Count dropped files
			uFileCount = DragQueryFile( hDrop, ( UINT )-1, NULL, 0 );

			// Loop through dropped files
			for( uWhichFile = 0; uWhichFile < uFileCount; uWhichFile ++ )
			{
				// Get size of dropped file
				uFileSize = DragQueryFile( hDrop, uWhichFile, NULL, 0 );

				// Ensure that file size is valid
				if( uFileSize != 0 )
				{
					// File size is valid

					// Get file path
					if( DragQueryFile( hDrop, uWhichFile, lpszFilePath, ( uFileSize + sizeof( char ) ) ) )
					{
						// Successfully got file path

						// Add file path to list view window
						

					} // End of successfully got file path

				} // End of file size is valid

			}; // End of loop through dropped files

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					MessageBox( hWndMain, ABOUT_MESSAGE_TEXT, ABOUT_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					MessageBox( hWndMain, ABOUT_MESSAGE_TEXT, ABOUT_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message

			// Call default handler
			lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenu;

			// Load context menu
			hMenu = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenu, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMsg, wParam, lParam);

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpfnWndProc		= MainWndProc;
	wcMain.hInstance		= hInstance;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;
			LPWSTR *lpszArgumentList;
			int nArgumentCount;

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Get argument list
			lpszArgumentList = CommandLineToArgvW( GetCommandLineW(), &nArgumentCount );

			// Ensure that argument list was got
			if( lpszArgumentList )
			{
				// Successfully got argument list
				int nWhichArgument;
				int nSizeNeeded;
				int nWideArgumentLength;

				// Allocate string memory
				LPTSTR lpszArgument = new char[ STRING_LENGTH ];

				// Loop through arguments
				for( nWhichArgument = 1; nWhichArgument < nArgumentCount; nWhichArgument ++ )
				{
					// Get wide argument length
					nWideArgumentLength = lstrlenW( lpszArgumentList[ nWhichArgument ] );

					// Get size required for argument
					nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, NULL, 0, NULL, NULL );

					// Convert argument to ansi
					WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

					// Terminate argument
					lpszArgument[ nSizeNeeded ] = ( char )NULL;

					// Add argument to list view window
					

				}; // End of loop through arguments

				// Free string memory
				delete [] lpszArgument;

			} // End of successfully got argument list

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of main message loop

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to register main window class

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return msg.wParam;

} // End of function WinMain