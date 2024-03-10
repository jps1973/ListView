// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
HWND g_hWndListView;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddItem( LPCTSTR lpszItemText )
{
	int nResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );
	lvItem.iSubItem		= LIST_VIEW_WINDOW_NAME_COLUMN_ID;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	nResult = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAddItem

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH,( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column
		else
		{
			// Unable to auto-size current column

			// Force exit from loop
			nWhichColumn = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column

	} // End of loop through columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = ::CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles[] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask	= ( LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM );
		lvColumn.cx		= LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )0, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Insert columns
		for( lvColumn.iSubItem = 0; lvColumn.iSubItem < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; lvColumn.iSubItem ++ )
		{
			// Update list view column structure for column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ lvColumn.iSubItem ];

			// Insert column
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )lvColumn.iSubItem, ( LPARAM )&lvColumn );

		}; // End of loop to insert columns

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function ListViewWindowCreate

int ListViewWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListView, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ListViewWindowGetCurrentSelection

int ListViewWindowGetItemText( int nWhichItem, LPTSTR lpszItemText )
{
	// Get item text
	return::SendMessage( g_hWndListView, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText );

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return ::GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list view window notification code
	switch( HIWORD( wParam ) )
	{
		case LBN_DBLCLK:
		{
			// A list view window double click notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListViewWindowGetCurrentSelection();

			// Get selected item text
			if( ListViewWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list view window double click notification code
		case LBN_SELCHANGE:
		{
			// A list view window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListViewWindowGetCurrentSelection();

			// Get selected item text
			if( ListViewWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call selection changed function
				( *lpSelectionChangedFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list view window selection change notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleCommandMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return ::MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return ::SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	::SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText )
{
	BOOL bResult;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Set item text
	bResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowSetItemText