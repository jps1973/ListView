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
	LPTSTR lpszItemText1 = new char[ STRING_LENGTH ];
	LPTSTR lpszItemText2 = new char[ STRING_LENGTH ];

	// Clear list view item structure
	::ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure for first item
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
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
				LPTSTR lpszItemText = new char[ STRING_LENGTH ];

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
				LPTSTR lpszItemText = new char[ STRING_LENGTH ];

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
	ListViewWindowAdditem( "1234567890" );
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
