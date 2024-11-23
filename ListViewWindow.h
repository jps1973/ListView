// ListViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_VIEW_WINDOW_CLASS_NAME												WC_LISTVIEW

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define LIST_VIEW_WINDOW_INITIAL_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%d items"

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "Name", "Details" }

typedef enum
{
	LIST_VIEW_WINDOW_NAME_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_DETAILS_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} LIST_VIEW_WINDOW_COLUMNS;

BOOL IsListViewWindow( HWND hWnd );

int ListViewWindowAdditem( LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );

void ListViewWindowAutoSizeAllColumns();

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListViewWindowGetItemCount();

int ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );

BOOL ListViewWindowGetRect( LPRECT lpRect );

BOOL ListViewWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListViewWindowPopulate();

HWND ListViewWindowSetFocus();

void ListViewWindowSetFont( HFONT hFont );

int ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumItemTextLength = STRING_LENGTH );
