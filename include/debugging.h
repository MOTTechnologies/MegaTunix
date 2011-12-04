/*
 * Copyright (C) 2002-2011 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute, etc. this as long as all the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

/*!
  \file include/debugging.h
  \ingroup Headers
  \brief Handler for debug management. This SHOULD BE REFACTORED to use macros
  \author David Andruczyk
  */

#ifndef __DEBUG_GUI_H__
#define __DEBUG_GUI_H__

#include <gtk/gtk.h>

/* Handy wrapper to inject filename/function name nad line number into debugging
   */
#ifdef IN_PLUGIN
#define DEBUG_FUNC(level, ...) new_dbg_func_f(level,__FILE__,__FUNCTION__,__LINE__, __VA_ARGS__)
#define QUIET_DEBUG_FUNC(level, ...) new_dbg_func_f(level,NULL,NULL,NULL, __VA_ARGS__)
#else
#define DEBUG_FUNC(level, ...) new_dbg_func(level,__FILE__,__FUNCTION__,__LINE__, __VA_ARGS__)
#define QUIET_DEBUG_FUNC(level, ...) new_dbg_func(level,NULL,NULL,NULL, __VA_ARGS__)
#endif

/* Debugging Enumerations */
typedef enum
{
	NO_DEBUG 	= 0,
	INTERROGATOR 	= 1<<0,
	OPENGL		= 1<<1,
	CONVERSIONS	= 1<<2,
	SERIAL_RD	= 1<<3,
	SERIAL_WR	= 1<<4,
	IO_MSG		= 1<<5,
	IO_PROCESS	= 1<<6,
	THREADS		= 1<<7,
	REQ_FUEL	= 1<<8,
	TABLOADER	= 1<<9,
	KEYPARSER	= 1<<10,
	RTMLOADER	= 1<<11,
	COMPLEX_EXPR	= 1<<12,
	MTXSOCKET	= 1<<13,
	PLUGINS		= 1<<14,
	PACKETS		= 1<<15,
	CRITICAL	= 1<<30
}Dbg_Class;

typedef enum guint
{
	INTERROGATOR_SHIFT	= 0,
	OPENGL_SHIFT		= 1,
	CONVERSIONS_SHIFT	= 2,
	SERIAL_RD_SHIFT		= 3,
	SERIAL_WR_SHIFT		= 4,
	IO_MSG_SHIFT		= 5,
	IO_PROCESS_SHIFT	= 6,
	THREADS_SHIFT		= 7,
	REQ_FUEL_SHIFT		= 8,
	TABLOADER_SHIFT		= 9,
	KEYPARSER_SHIFT		= 10,
	RTMLOADER_SHIFT		= 11,
	COMPLEX_EXPR_SHIFT	= 12,
	MTXSOCKET_SHIFT		= 13,
	PLUGINS_SHIFT		= 14,
	PACKETS_SHIFT		= 15,
	CRITICAL_SHIFT		= 30
}Dbg_Shift;

typedef struct _DebugLevel DebugLevel;

/*!
 \brief _DebugLevel stores the debugging name, handler, class (bitmask) and 
 shift (forgot why this is here) and a enable/disable flag. Used to make the
 debugging core a little more configurable
 */
struct _DebugLevel
{
	gchar * name;		/*!< Debugging name */
	gint	handler;	/*!< Signal handler name */
	Dbg_Class dclass;	/*!< Bit mask for this level (0-31) */
	Dbg_Shift dshift;	/*!< Bit shift amount */
	gboolean enabled;	/*!< Enabled or not? */
};


/* Prototypes */
void close_debug(void);
void open_debug(void);
void dbg_func(Dbg_Class, gchar *);
void new_dbg_func(Dbg_Class, const gchar *, const gchar *, gint, const gchar *, ...);
void populate_debugging(GtkWidget *);
/* Prototypes */

#endif
