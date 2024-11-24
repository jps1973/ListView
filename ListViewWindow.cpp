// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAdditem( LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength )
{
	int nResult;

	LVITEM lvItem;
	int nItemCount;

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumItemTextLength;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= 0;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAdditem

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	int nResult = 0;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszItemText1 = new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszItemText2 = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	::ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure for first item
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= ( STRING_LENGTH + sizeof( char ) );
	lvItem.iSubItem		= lParamSort;
	lvItem.iItem		= lParam1;
	lvItem.pszText		= ( LPTSTR )lpszItemText1;

	// Get first item text
	::SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )( lvItem.iItem ), ( LPARAM )&lvItem );

	// Update list view item structure for second item
	lvItem.iItem		= lParam2;
	lvItem.pszText		= ( LPTSTR )lpszItemText2;

	// Get first item text
	::SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )( lvItem.iItem ), ( LPARAM )&lvItem );

	// Compare item texts
	nResult = lstrcmpi( lpszItemText1, lpszItemText2 );

	// Free string memory
	delete [] lpszItemText1;
	delete [] lpszItemText2;

	return nResult;

} // End of function ListViewWindowCompare

void ListViewWindowAutoSizeAllColumns()
{
	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER );

	}; // End of loop through all columns

} // End of function ListViewWindowAutoSizeAllColumns

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		int nWhichColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initalise list view column structure
		lvColumn.mask		= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx			= LIST_VIEW_WINDOW_INITIAL_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Loop through all columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure for current column
			lvColumn.pszText = ( LPTSTR )( lpszColumnTitles[ nWhichColumn ] );

			// Add current column to list view window
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop through all columns

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window

	return bResult;

} // End of function ListViewWindowCreate

int ListViewWindowGetItemCount()
{
	return SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ListViewWindowGetItemCount

int ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumItemTextLength )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumItemTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Set item text
	nResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )nWhichItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ), void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMLISTVIEW lpNmListView;

	// Get list view notify message information
	lpNmListView = ( LPNMLISTVIEW )lParam;

	// Select list view window notification code
	switch( lpNmListView->hdr.code )
	{
		case LVN_COLUMNCLICK:
		{
			// A column click notify message

			// Sort list view window
			SendMessage( g_hWndListView, LVM_SORTITEMSEX, ( WPARAM )( LPARAM )( lpNmListView->iSubItem ), ( LPARAM )&ListViewWindowCompare );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view window item changed notification code

			// See if selection has changed
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Selection has changed

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item text
				if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemText ) )
				{
					// Successfully got item text

					// Call selection changed function
					( *lpSelectionChangedFunction )( lpszItemText );

					// Update return value
					bResult = TRUE;

				} // End of successfully got item text

				// Free string memory
				delete [] lpszItemText;

			} // End of selection has changed

			// Break out of switch
			break;

		} // End of a list view window item changed notification code
		case NM_DBLCLK:
		{
			// A double click notification code

			// Ensure that an item is selected
			if( lpNmListView->iItem >= 0 )
			{
				// An item is selected

				// Allocate string memory
				LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get item text
				if( ListViewWindowGetItemText( lpNmListView->iItem, lpNmListView->iSubItem, lpszItemText ) )
				{
					// Successfully got item text

					// Call double click function
					( *lpDoubleClickFunction )( lpszItemText );

					// Update return value
					bResult = TRUE;

				} // End of successfully got item text

				// Free string memory
				delete [] lpszItemText;

			} // End of an item is selected

			// Break out of switch
			break;

		} // End of a double click notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

int ListViewWindowPopulate()
{
	int nResult = 0;

	// Add items to list view window
	ListViewWindowAdditem( "123,456,789,0" );
	ListViewWindowAdditem( "qwertyuiop" );
	ListViewWindowAdditem( "asdfghjkl" );
	ListViewWindowAdditem( "zxcvbnm" );

	// Set item texts
	ListViewWindowSetItemText( 0, 1, "10" );
	ListViewWindowSetItemText( 1, 1, "11" );
	ListViewWindowSetItemText( 2, 1, "12" );
	ListViewWindowSetItemText( 3, 1, "13" );

	// Update return value
	nResult = ListViewWindowGetItemCount();

	// Auto-size all list view window columns
	ListViewWindowAutoSizeAllColumns();

	return nResult;

} // End of function ListViewWindowPopulate

BOOL ListViewWindowSave( LPCTSTR lpszFileName, LPCTSTR lpszItemSeparator )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nWhichItem;
		int nWhichSubItem;
		int nItemCount;
		DWORD dwItemTextLength;
		DWORD dwItemSeparatorLength;
		DWORD dwNewLineTextLength;

		// Allocate string memory
		LPTSTR lpszItemText		= new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszTemporary	= new char[ STRING_LENGTH + sizeof( char ) ];

		// Store text lengths
		dwItemSeparatorLength	= lstrlen( lpszItemSeparator );
		dwNewLineTextLength		= lstrlen( NEW_LINE_TEXT );

		// Count items on list view window
		nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Update return value (assume success)
		bResult = TRUE;

		// Loop through items on list view window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Loop through sub-items on list view window
			for( nWhichSubItem = 0; nWhichSubItem < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichSubItem ++ )
			{
				// Get item text
				if( ListViewWindowGetItemText( nWhichItem, nWhichSubItem, lpszItemText ) )
				{
					// Successfully got item text

					// See if item text contains item separator
					if( strstr( lpszItemText, lpszItemSeparator ) )
					{
						// Item text contains item separator

						// Copy item text into temporary string
						lstrcpy( lpszTemporary, lpszItemText );

						// Put item text into inverted commas
						wsprintf( lpszItemText, LIST_VIEW_WINDOW_ITEM_TEXT_IN_INVERTED_COMMAS_FORMAT_STRING, lpszTemporary );

					} // End of item text contains item separator

					// Get item text length
					dwItemTextLength = lstrlen( lpszItemText );

					// Write item text to file
					if( WriteFile( hFile, lpszItemText, dwItemTextLength, NULL, NULL ) )
					{
						// Successfully wrote item text to file

						// See if this is the last sub-item
						if( nWhichSubItem < ( LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS - 1 ) )
						{
							// This is not the last sub-item

							// Write item separator to file
							WriteFile( hFile, lpszItemSeparator, dwItemSeparatorLength, NULL, NULL );

						} // End of this is not the last sub-item

					} // End of successfully wrote item text to file
					else
					{
						// Unable to write item text to file

						// Update return value
						bResult = FALSE;

						// Force exit from loops
						nWhichItem = nItemCount;
						nWhichSubItem = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

					} // End of unable to write item text to file

				} // End of successfully got item text

			}; // End of loop through sub-items on list view window

			// See if this is the last item
			if( nWhichItem < ( nItemCount - 1 ) )
			{
				// This is not the last item

				// Write new line to file
				WriteFile( hFile, NEW_LINE_TEXT, dwNewLineTextLength, NULL, NULL );

			} // End of this is not the last item

		}; // End of loop through items on list view window

		// Free string memory
		delete [] lpszItemText;
		delete [] lpszTemporary;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return bResult;

} // End of function ListViewWindowSave

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont

int ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumItemTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Set item text
	nResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )nWhichItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowSetItemText
