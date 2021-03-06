/* Nabi - X Input Method server for hangul
 * Copyright (C) 2003-2008 Choe Hwanjin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */


#ifndef _NABIIC_H_
#define _NABIIC_H_

#include <X11/Xlib.h>
#include <glib.h>
#include <gtk/gtk.h>

#include <hangul.h>

#include "../IMdkit/IMdkit.h"
#include "../IMdkit/Xi18n.h"

#include "candidate.h"
#include "ustring.h"

typedef struct _PreeditAttributes PreeditAttributes;
typedef struct _StatusAttributes StatusAttributes;
typedef struct _NabiIC         NabiIC;
typedef struct _NabiConnection NabiConnection;
typedef struct _NabiToplevel   NabiToplevel;

typedef enum {
    NABI_INPUT_MODE_DIRECT,
    NABI_INPUT_MODE_COMPOSE
} NabiInputMode;

struct _NabiConnection {
    CARD16         id;
    NabiInputMode  mode;
    GIConv         cd;
    CARD16         next_new_ic_id;
    GSList*        ic_list;
};

struct _NabiToplevel {
    Window        id;
    NabiInputMode mode;
    unsigned int  ref;
};

struct _PreeditAttributes {
    UString*        str;
    GdkWindow*      window;         /* where to draw the preedit string */
    int             width;          /* preedit area width */
    int             height;         /* preedit area height */
    XPoint          spot;           /* window position */

    XRectangle      area;           /* area */
    XRectangle      area_needed;    /* area needed */

    Colormap        cmap;           /* colormap */
    GdkGC*          normal_gc;      /* gc */
    GdkGC*          hilight_gc;     /* gc */
    unsigned long   foreground;     /* foreground */
    unsigned long   background;     /* background */

    char            *base_font;     /* base font of fontset */
    XFontSet        font_set;       /* font set */
    int             ascent;         /* font property */
    int             descent;        /* font property */

    Pixmap          bg_pixmap;      /* background pixmap */
    CARD32          line_space;     /* line spacing */
    Cursor          cursor;         /* cursor */

    XIMPreeditState state;          /* preedit state */
    Bool            start;          /* preedit start */
    int		    prev_length;    /* previous preedit string length */

    gboolean        has_start_cb;   /* whether XNPreeditStartCallback
				     * registered */
    gboolean        has_draw_cb;    /* whether XNPreeditDrawCallback 
				     * registered */
    gboolean        has_done_cb;    /* whether XNPreeditDoneCallback 
				     * registered */
};

struct _StatusAttributes {
    XRectangle      area;           /* area */
    XRectangle      area_needed;    /* area needed */
    Colormap        cmap;           /* colormap */
    unsigned long   foreground;     /* foreground */
    unsigned long   background;     /* background */
    Pixmap          bg_pixmap;      /* background pixmap */
    char            *base_font;     /* base font of fontset */
    CARD32          line_space;     /* line spacing */
    Cursor          cursor;         /* cursor */
};

struct _NabiIC {
    CARD16              id;               /* ic id */
    INT32               input_style;      /* input style */
    Window              client_window;    /* client window */
    Window              focus_window;     /* focus window */
    char*               resource_name;    /* resource name */
    char*               resource_class;   /* resource class */
    StatusAttributes    status;           /* status attributes */
    PreeditAttributes   preedit;          /* preedit attributes */

    NabiConnection*     connection;
    NabiToplevel*       toplevel;

    /* hangul data */
    NabiInputMode       mode;
    HangulInputContext* hic;

    /* hanja or symbol select window */
    NabiCandidate*	candidate;

    gboolean            composing_started;
    UString*            client_text;
    gboolean            wait_for_client_text; /* whether this ic requested
					       * client text */
    gboolean            has_str_conv_cb;  /* whether XNStringConversionCallback
					   * registered */
};

NabiConnection* nabi_connection_create(CARD16 id, const char* encoding);
void         nabi_connection_destroy(NabiConnection* conn);
NabiIC*      nabi_connection_create_ic(NabiConnection* conn,
				       IMChangeICStruct* data);
void         nabi_connection_destroy_ic(NabiConnection* conn, NabiIC* ic);
NabiIC*      nabi_connection_get_ic(NabiConnection* conn, CARD16 id);

NabiToplevel* nabi_toplevel_new(Window id);
void          nabi_toplevel_ref(NabiToplevel* toplevel);
void          nabi_toplevel_unref(NabiToplevel* toplevel);

NabiIC* nabi_ic_create(NabiConnection* conn, IMChangeICStruct *data);
void    nabi_ic_destroy(NabiIC *ic);
void    nabi_ic_real_destroy(NabiIC *ic);

void    nabi_ic_set_values(NabiIC *ic, IMChangeICStruct *data);
void    nabi_ic_get_values(NabiIC *ic, IMChangeICStruct *data);

Bool    nabi_ic_is_empty(NabiIC *ic);
CARD16  nabi_ic_get_id(NabiIC* ic);

void    nabi_ic_set_focus(NabiIC *ic);

void    nabi_ic_set_mode(NabiIC *ic, NabiInputMode mode);
void    nabi_ic_start_composing(NabiIC *ic);
void    nabi_ic_end_composing(NabiIC *ic);

void    nabi_ic_preedit_start(NabiIC *ic);
void    nabi_ic_preedit_done(NabiIC *ic);
void    nabi_ic_preedit_update(NabiIC *ic);
void    nabi_ic_preedit_clear(NabiIC *ic);

void    nabi_ic_status_start(NabiIC *ic);
void    nabi_ic_status_done(NabiIC *ic);
void    nabi_ic_status_update(NabiIC *ic);

Bool    nabi_ic_commit(NabiIC *ic);

Bool    nabi_ic_process_keyevent(NabiIC* ic, KeySym keysym, unsigned int state);
void    nabi_ic_flush(NabiIC *ic);
void    nabi_ic_reset(NabiIC *ic, IMResetICStruct *data);

Bool    nabi_ic_popup_candidate_window(NabiIC *ic, const char* key);
void    nabi_ic_insert_candidate(NabiIC *ic, const Hanja* hanja);

void    nabi_ic_process_string_conversion_reply(NabiIC* ic, const char* text);

#endif /* _NABIIC_H_ */
/* vim: set ts=8 sw=4 sts=4 : */
