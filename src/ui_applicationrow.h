/*
 * ui_applicationrow.h
 *
 * Copyright (C) 2014 - Wiky L
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __UI_APPLICATIONROW_H__
#define __UI_APPLICATIONROW_H__

#include <gtk/gtk.h>
#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_UI_APPLICATION_ROW (ui_application_row_get_type ())
#define UI_APPLICATION_ROW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_UI_APPLICATION_ROW, UIApplicationRow))
#define UI_APPLICATION_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_UI_APPLICATION_ROW, UIApplicationRowClass))
#define IS_UI_APPLICATION_ROW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_UI_APPLICATION_ROW))
#define IS_UI_APPLICATION_ROW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_UI_APPLICATION_ROW))
#define UI_APPLICATION_ROW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_UI_APPLICATION_ROW, UIApplicationRowClass))
typedef struct _UIApplicationRow UIApplicationRow;
typedef struct _UIApplicationRowClass UIApplicationRowClass;
typedef struct _UIApplicationRowPrivate UIApplicationRowPrivate;

struct _UIApplicationRow {
    GtkEventBox parent_instance;
    UIApplicationRowPrivate *priv;
};

struct _UIApplicationRowClass {
    GtkEventBoxClass parent_class;
};


GType ui_application_row_get_type(void) G_GNUC_CONST;
UIApplicationRow *ui_application_row_new(void);
UIApplicationRow *ui_application_row_construct(GType object_type);

void ui_application_row_set_icon(UIApplicationRow * self,
                                 GdkPixbuf * pixbuf);
UIApplicationRow *ui_application_row_new_full(GdkPixbuf * pixbuf,
                                              const gchar * name,
                                              const gchar * version,
                                              LcProtocolApplicationType
                                              type);
UIApplicationRow *ui_application_row_new_with_data(const
                                                   LcProtocolApplication *
                                                   info);
void ui_application_row_update_data(UIApplicationRow * self,
                                    const LcProtocolApplication * info);

void ui_application_row_set_data(UIApplicationRow * self, gpointer data,
                                 GDestroyNotify destroy);
gpointer ui_application_row_get_data(UIApplicationRow * self);

void ui_application_row_highlight(UIApplicationRow * self);
void ui_application_row_unhighlight(UIApplicationRow * self);

GtkButton *ui_application_row_get_uninstall_button(UIApplicationRow *
                                                   self);

G_END_DECLS
#endif
