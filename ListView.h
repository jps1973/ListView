// ListView.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"

#define MAIN_WINDOW_CLASS_NAME													"Main ListView Window Class"
#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( NULL, IDI_APPLICATION )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_WINDOW + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )
#define MAIN_WINDOW_CLASS_ICON_SMALL											LoadIcon( NULL, IDI_APPLICATION )

#define MAIN_WINDOW_EXTENDED_STYLE												( WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES )
#define MAIN_WINDOW_TEXT														"ListView"
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW

#define MAIN_WINDOW_MINIMUM_WIDTH												320
#define MAIN_WINDOW_MINIMUM_HEIGHT												200

#define LIST_VIEW_WINDOW_EXTENDED_STYLE											LVS_EX_FULLROWSELECT
#define LIST_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT )
#define LIST_VIEW_WINDOW_TEXT													NULL

#define STATUS_BAR_WINDOW_EXTENDED_STYLE										0
#define STATUS_BAR_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP )
#define STATUS_BAR_WINDOW_TEXT													NULL

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define STRING_LENGTH															2048

#define ABOUT_MESSAGE_TEXT														"ListView\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"April 2023"

#define ABOUT_MESSAGE_CAPTION													"About"
#define ERROR_MESSAGE_CAPTION													"Error"
#define INFORMATION_MESSAGE_CAPTION												"Information"
#define WARNING_MESSAGE_CAPTION													"Warning"

#define UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE						"Unable to register main window class"
#define UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE								"Unable to create main window"

#define LIST_VIEW_WINDOW_DEFAULT_COLUMN_WIDTH									100

#define LIST_VIEW_WINDOW_COLUMN_TITLES											{ "First", "Second" }

typedef enum 
{
	LIST_VIEW_WINDOW_FIRST_COLUMN_ID = 0,
	LIST_VIEW_WINDOW_SECOND_COLUMN_ID,

	LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS

} ListViewWindowColumnIDs;

void ListViewWindowAddColumns();

int ListViewWindowAddItem( LPCTSTR lpszFirstItemText, LPCTSTR lpszSecondItemText );

int ListViewWindowAddItem( LPCTSTR lpszItemText );

void ListViewWindowAutoSizeColumns();

int ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText );

int ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText );
