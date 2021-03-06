/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2016-2016 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <string.h>
#include "layer0.h"
#include "filefmt.h"
#include "write.h"
#include "read.h"
#include "resmenu.h"
#include "reserr.h"
#include "wresrtns.h"

bool ResWriteMenuHeader( MenuHeader *currhead, WResFileID handle )
/****************************************************************/
{
    bool    error;

    error = ResWriteUint16( currhead->Version, handle );
    if( !error ) {
        error = ResWriteUint16( currhead->HeaderSize, handle );
    }
    return( error );
}

bool ResWriteMenuExHeader( MenuHeader *currhead, WResFileID handle, uint_8 *headerdata )
/**************************************************************************************/
{
    if( ResWriteMenuHeader( currhead, handle ) ) {
        return( true );
    } else {
        if( headerdata != NULL ) {
            if( WRESWRITE( handle, headerdata, currhead->HeaderSize ) != currhead->HeaderSize ) {
                WRES_ERROR( WRS_WRITE_FAILED );
                return( true );
            }
        }
        return( false );
    }
}

bool ResWriteMenuItemPopup( const MenuItemPopup *curritem, bool use_unicode, WResFileID handle )
/**********************************************************************************************/
{
    bool        error;

    if( curritem->ItemFlags & MENU_POPUP ) {
        error = ResWriteUint16( curritem->ItemFlags, handle );
        if( !error ) {
            error = ResWriteString( curritem->ItemText, use_unicode, handle );
        }
    } else {
        WRES_ERROR( WRS_BAD_PARAMETER );
        error = true;
    }

    return( error );
}

bool ResWriteMenuExItemPopup( const MenuItemPopup *curritem, const MenuExItemPopup *exdata,
                             bool use_unicode, WResFileID handle )
/*****************************************************************************/
{
    bool        error;

    if( curritem->ItemFlags & MENUEX_POPUP ) {
        error = ResWriteUint32( exdata->ItemType, handle );
        if( !error ) {
            error = ResWriteUint32( exdata->ItemState, handle );
        }
        if( !error ) {
            error = ResWriteUint32( exdata->ItemId, handle );
        }
        if( !error ) {
            error = ResWriteUint16( curritem->ItemFlags, handle );
        }
        if( !error ) {
            error = ResWriteString( curritem->ItemText, use_unicode, handle );
        }
        if( !error ) {
            error = ResWritePadDWord( handle );
        }
        if( !error ) {
            error = ResWriteUint32( exdata->HelpId, handle );
        }
    } else {
        WRES_ERROR( WRS_BAD_PARAMETER );
        error = true;
    }

    return( error );

}

bool ResWriteMenuItemNormal( const MenuItemNormal *curritem, bool use_unicode,
                                                    WResFileID handle )
/******************************************************************************/
{
    bool        error;

    if( curritem->ItemFlags & MENU_POPUP ) {
        WRES_ERROR( WRS_BAD_PARAMETER );
        error = true;
    } else {
        error = ResWriteUint16( curritem->ItemFlags, handle );
        if( !error ) {
            error = ResWriteUint16( (uint_16)curritem->ItemID, handle );
        }
        if( !error ) {
            error = ResWriteString( curritem->ItemText, use_unicode, handle );
        }
    }

    return( error );
}

bool ResWriteMenuExItemNormal( const MenuItemNormal *curritem, const MenuExItemNormal *exdata,
                              bool use_unicode, WResFileID handle )
/*******************************************************************************************/
{
    bool        error;

    if( curritem->ItemFlags & MENUEX_POPUP ) {
        WRES_ERROR( WRS_BAD_PARAMETER );
        error = true;
    } else {
        error = ResWriteUint32( exdata->ItemType, handle );
        if( !error ) {
            error = ResWriteUint32( exdata->ItemState, handle );
        }
        if( !error ) {
            error = ResWriteUint32( curritem->ItemID, handle );
        }
        if( !error ) {
            error = ResWriteUint16( curritem->ItemFlags, handle );
        }
        if( !error ) {
            error = ResWriteString( curritem->ItemText, use_unicode, handle );
        }
        if( !error ) {
            error = ResWritePadDWord( handle );
        }
    }

    return( error );
}

bool ResWriteMenuItem( const MenuItem *curritem, bool use_unicode,
                                             WResFileID handle )
/******************************************************************/
{
    bool    error;

    if( curritem->IsPopup ) {
        error = ResWriteMenuItemPopup( &(curritem->Item.Popup), use_unicode, handle );
    } else {
        error = ResWriteMenuItemNormal( &(curritem->Item.Normal), use_unicode, handle );
    }

    return( error );
}
