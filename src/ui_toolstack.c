/*
 * ui_toolstack.c
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



#include <glib.h>
#include <glib-object.h>
#include "ui_toolstack.h"
#include <gtk/gtk.h>

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UIToolStackPrivate {
    GtkToolbar *toolbar;
    GtkStack *stack;
    GtkRadioToolButton *current;
    guint stack_index;
};


#define G_OBJECT_KEY_STACK_NAME "stack-name"
#define G_OBJECT_KEY_CALLBACK	"toggled-callback"
#define G_OBJECT_KEY_USER_DATA	"toggled-callback-data"

static gpointer ui_tool_stack_parent_class = NULL;

#define UI_TOOL_STACK_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_UI_TOOL_STACK, UIToolStackPrivate))
enum {
    UI_TOOL_STACK_DUMMY_PROPERTY
};
static void ui_tool_stack_finalize(GObject * obj);


UIToolStack *ui_tool_stack_construct(GType object_type)
{
    UIToolStack *self = NULL;
    self = (UIToolStack *) g_object_new(object_type, NULL);
    self->priv->toolbar = (GtkToolbar *) gtk_toolbar_new();
    gtk_toolbar_set_style(self->priv->toolbar, GTK_TOOLBAR_BOTH);
    g_object_ref_sink(self->priv->toolbar);
    self->priv->stack = (GtkStack *) gtk_stack_new();
    gtk_stack_set_transition_type(self->priv->stack,
                                  GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    g_object_ref_sink(self->priv->stack);

    gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->priv->toolbar), 0, 0,
                    1, 1);
    gtk_grid_attach(GTK_GRID(self), GTK_WIDGET(self->priv->stack), 0, 1, 1,
                    1);
    gtk_widget_set_vexpand(GTK_WIDGET(self->priv->stack), TRUE);

    self->priv->stack_index = 0;
    self->priv->current = NULL;
    return self;
}


UIToolStack *ui_tool_stack_new(void)
{
    return ui_tool_stack_construct(TYPE_UI_TOOL_STACK);
}


static void ui_tool_stack_class_init(UIToolStackClass * klass)
{
    ui_tool_stack_parent_class = g_type_class_peek_parent(klass);
    g_type_class_add_private(klass, sizeof(UIToolStackPrivate));
    G_OBJECT_CLASS(klass)->finalize = ui_tool_stack_finalize;
}


static void ui_tool_stack_instance_init(UIToolStack * self)
{
    self->priv = UI_TOOL_STACK_GET_PRIVATE(self);

    gtk_grid_set_column_homogeneous(GTK_GRID(self), TRUE);
}


static void ui_tool_stack_finalize(GObject * obj)
{
    UIToolStack *self;
    self =
        G_TYPE_CHECK_INSTANCE_CAST(obj, TYPE_UI_TOOL_STACK, UIToolStack);
    _g_object_unref0(self->priv->toolbar);
    _g_object_unref0(self->priv->stack);
    G_OBJECT_CLASS(ui_tool_stack_parent_class)->finalize(obj);
}


GType ui_tool_stack_get_type(void)
{
    static volatile gsize ui_tool_stack_type_id__volatile = 0;
    if (g_once_init_enter(&ui_tool_stack_type_id__volatile)) {
        static const GTypeInfo g_define_type_info =
            { sizeof(UIToolStackClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) ui_tool_stack_class_init,
            (GClassFinalizeFunc) NULL,
            NULL, sizeof(UIToolStack), 0,
            (GInstanceInitFunc) ui_tool_stack_instance_init, NULL
        };
        GType lc_tool_stack_type_id;
        lc_tool_stack_type_id =
            g_type_register_static(GTK_TYPE_GRID, "UIToolStack",
                                   &g_define_type_info, 0);
        g_once_init_leave(&ui_tool_stack_type_id__volatile,
                          lc_tool_stack_type_id);
    }
    return ui_tool_stack_type_id__volatile;
}

static void on_radio_tool_button_toggled(GtkRadioToolButton * radio,
                                         gpointer user_data)
{
    UIToolStack *self = UI_TOOL_STACK(user_data);
    gboolean toggled =
        gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(radio));

    UIToolStackToggledNotify callback =
        (UIToolStackToggledNotify) g_object_get_data(G_OBJECT(radio),
                                                     G_OBJECT_KEY_CALLBACK);
    gpointer data =
        g_object_get_data(G_OBJECT(radio), G_OBJECT_KEY_USER_DATA);
    if (toggled) {
        self->priv->current = radio;

        const gchar *stack_name =
            (gchar *) g_object_get_data(G_OBJECT(radio),
                                        G_OBJECT_KEY_STACK_NAME);
        gtk_stack_set_visible_child_name(self->priv->stack, stack_name);
    }
    if (callback) {
        callback(toggled, data);
    }
}

void ui_tool_stack_append(UIToolStack * self,
                          GtkWidget * title_widget,
                          const gchar * title_label,
                          GtkWidget * content,
                          UIToolStackToggledNotify callback,
                          gpointer user_data)
{
    GtkToolbar *toolbar = self->priv->toolbar;
    GtkStack *stack = self->priv->stack;
    GtkRadioToolButton *radio =
        (GtkRadioToolButton *) gtk_toolbar_get_nth_item(toolbar, 0);

    GtkToolItem *button = gtk_radio_tool_button_new_from_widget(radio);
    gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(button), title_widget);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(button), title_label);
    gtk_toolbar_insert(toolbar, button, -1);

    gchar name[8];
    g_snprintf(name, sizeof(name), "%u", self->priv->stack_index++);
    gtk_stack_add_named(stack, content, name);

    g_object_set_data_full(G_OBJECT(button), G_OBJECT_KEY_STACK_NAME,
                           g_strdup(name), (GDestroyNotify) g_free);
    g_object_set_data(G_OBJECT(button), G_OBJECT_KEY_CALLBACK, callback);
    g_object_set_data(G_OBJECT(button), G_OBJECT_KEY_USER_DATA, user_data);

    g_signal_connect(G_OBJECT(button), "toggled",
                     G_CALLBACK(on_radio_tool_button_toggled), self);
}

const gchar *ui_tool_stack_get_current_title(UIToolStack * self)
{
    if (self->priv->current) {
        return
            gtk_tool_button_get_label(GTK_TOOL_BUTTON
                                      (self->priv->current));
    }
    return NULL;
}


void ui_tool_stack_set_transition_duration(UIToolStack * self,
                                           guint duration)
{
    gtk_stack_set_transition_duration(self->priv->stack, duration);
}

void ui_tool_stack_set_transition_type(UIToolStack * self,
                                       GtkStackTransitionType type)
{
    gtk_stack_set_transition_type(self->priv->stack, type);
}
