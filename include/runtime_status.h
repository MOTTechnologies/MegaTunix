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
  \file include/runtime_status.h
  \ingroup Headers
  \brief Header for the runtime status related code
  \author David Andruczyk
  */

#ifndef __RUNTIME_STATUS_H__
#define __RUNTIME_STATUS_H__

#include <enums.h>
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef enum
{
	COL_STATUS_WIDGET,
	COL_STATUS_LAST,
	STATUS_NUM_COLS
}StatusCols;

/* Prototypes */
void reset_runtime_status(void);
void rt_update_status(gpointer, gpointer);
void load_status_pf(void );
gboolean load_status_xml_elements(xmlNode *, GtkWidget *);
void load_status(xmlNode *, GtkWidget *);
void setup_status_treeview(GtkWidget *);
gboolean status_foreach(GtkTreeModel *, GtkTreePath *, GtkTreeIter *, gpointer);
gboolean update_runtime_status_pf(void);

/* Prototypes */

#endif
