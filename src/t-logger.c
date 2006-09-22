/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#include <config.h>
#include <glib.h>
#include <t-logger.h>
#include <structures.h>


static GHashTable *trigtooth_hash = NULL;

#define CTR 187
#define UNITS 188

/*!
 \brief 
 */

EXPORT void setup_logger_display(GtkWidget * src_widget)
{
	struct TTMon_Data *ttm_data = g_new0(struct TTMon_Data, 1);

	ttm_data->page = (gint)g_object_get_data(G_OBJECT(src_widget),"page");
	ttm_data->pixmap = NULL;
	ttm_data->darea = src_widget;
	ttm_data->min_time = 65535;
	ttm_data->max_time = 0;
	ttm_data->est_teeth = 0;
	ttm_data->wrap_pt = 0;
	ttm_data->current = g_array_sized_new(FALSE,TRUE,sizeof (gushort),93);
	ttm_data->last = g_array_sized_new(FALSE,TRUE,sizeof (gushort),93);

	g_object_set_data(G_OBJECT(src_widget),"ttmon_data",(gpointer)ttm_data);
	if (!trigtooth_hash)
		trigtooth_hash = g_hash_table_new(g_direct_hash,g_direct_equal);
	g_hash_table_replace(trigtooth_hash,GINT_TO_POINTER(ttm_data->page),ttm_data);
	return;
}

EXPORT gboolean logger_display_config_event(GtkWidget * widget, GdkEventConfigure *event , gpointer data)
{
	struct TTMon_Data *ttm_data = NULL;
	gint w = 0;
	gint h = 0;

	ttm_data = (struct TTMon_Data *)g_object_get_data(G_OBJECT(widget),"ttmon_data");
	if(widget->window)
	{
		if (ttm_data->pixmap)
			g_object_unref(ttm_data->pixmap);
		w=widget->allocation.width;
		h=widget->allocation.height;
		ttm_data->pixmap=gdk_pixmap_new(widget->window,
				w,h,
				gtk_widget_get_visual(widget)->depth);
		gdk_draw_rectangle(ttm_data->pixmap,
				widget->style->white_gc,
				TRUE, 0,0,
				w,h);
		gdk_window_set_back_pixmap(widget->window,ttm_data->pixmap,0);

	}
	update_trigtooth_display(ttm_data->page);
	return TRUE;
}

EXPORT gboolean logger_display_expose_event(GtkWidget * widget, GdkEventExpose *event , gpointer data)
{
	struct TTMon_Data *ttm_data = NULL;
	ttm_data = (struct TTMon_Data *)g_object_get_data(G_OBJECT(widget),"ttmon_data");
	gdk_draw_drawable(widget->window,
			widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
			ttm_data->pixmap,
			event->area.x, event->area.y,
			event->area.x, event->area.y,
			event->area.width, event->area.height);

	return TRUE;
}


void update_trigtooth_display(gint page)
{
	extern gint ** ms_data;
	gint i = 0;
	gint min = -1;
	gint max = -1;
	gushort total = 0;
	struct TTMon_Data *ttm_data = NULL;
	
	ttm_data = g_hash_table_lookup(trigtooth_hash,GINT_TO_POINTER(page));
	if (!ttm_data)
		printf("could NOT find ttm_Data in hashtable,  BIG ASS PROBLEM!\n");
	gint position = ms_data[page][CTR];
	gint count = 0;

	printf("Counter position on page %i is %i\n",page,position);
	if (position > 0)
		printf("data block from position %i to 185, then wrapping to 0 to %i\n",position,position-1);
	else
		printf("data block from position 0 to 185 (93 words)\n");

	count=0;
	for (i=position;i<185;i+=2)
	{
		total = (ms_data[page][i]*256)+ms_data[page][i+1];
		printf("%i ",total);
		g_array_insert_val(ttm_data->current,count,total);
		if (((count+1)%12)==0)
			printf("\n");
		count++;
	}
	if (position != 0)
	{
		for (i=0;i<position;i+=2)
		{
			total = (ms_data[page][i]*256)+ms_data[page][i+1];
			printf("%i ",total);
			g_array_insert_val(ttm_data->current,count,total);
			if (((count+1)%12)==0)
				printf("\n");
			count++;
		}
	}
	printf("\n");

	if (ms_data[page][UNITS] == 1)
	{
		printf("0.1 ms units\n");
	}
	else
	{
		printf("1uS units\n");
	}

	min = 65535;
//	min_index = -1;
//	max_index = -1;
	max = 0;
	for (i=0;i<93;i++)
	{
		if (g_array_index(ttm_data->current,gushort, i) < min)
			min = g_array_index(ttm_data->current,gushort, i);
		if (g_array_index(ttm_data->current,gushort, i) > max)
			max = g_array_index(ttm_data->current,gushort, i);
	}
	ttm_data->min_time = min;
	ttm_data->max_time = max;
	printf ("Minimum tooth time: %i, max tooth time %i\n",min,max);

}