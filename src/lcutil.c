/*
 * lcutil.c
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
#include "lcutil.h"
#include "ui_aboutdialog.h"
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <libnotify/notify.h>

static gchar exedir[PATH_MAX] = "./";
static gchar cachedir[PATH_MAX];
static gchar imgcachedir[PATH_MAX];

void lc_init(int argc, char *argv[])
{
    notify_init("lucy");
    /* get the running executable file's absolute path */
    ssize_t count = readlink("/proc/self/exe", exedir, PATH_MAX);
    if (count <= 0) {
        return;
    }
    exedir[count] = '\0';
    gchar *s = g_strrstr(exedir, "/");
    if (s) {
        *s = '\0';
    }

    g_snprintf(cachedir, sizeof(cachedir), "%s/%s", g_get_user_cache_dir(),
               PROGRAM_NAME);
    g_snprintf(imgcachedir, sizeof(imgcachedir), "%s/img", cachedir);

    g_mkdir_with_parents(imgcachedir, 0777);
}

gchar *lc_util_get_string_from_byte_array(GByteArray * array, gsize * size)
{
    if (array == NULL) {
        return NULL;
    }
    guint8 null = '\0';
    g_byte_array_append(array, &null, 1);
    GBytes *bytes = g_byte_array_free_to_bytes(array);
    gsize len;
    if (size == NULL) {
        size = &len;
    }
    return g_bytes_unref_to_data(bytes, size);
}

GBytes *lc_util_get_bytes_from_byte_array(GByteArray * array)
{
    if (array == NULL) {
        return NULL;
    }
    GBytes *bytes = g_byte_array_free_to_bytes(array);

    return bytes;
}

const gchar *lc_util_get_resource_by_name(const gchar * name)
{
    static gchar buf[1024];
    g_snprintf(buf, sizeof(buf), "%s/%s", PACKAGE_DATA_DIR, name);
    if (g_file_test(buf, G_FILE_TEST_EXISTS)) {
        return buf;
    }
    g_snprintf(buf, sizeof(buf), "%s/res/%s", exedir, name);
    if (g_file_test(buf, G_FILE_TEST_EXISTS)) {
        return buf;
    }
    g_warning("Resource '%s' not found", name);
    return NULL;
}

const gchar *lc_util_get_cache_path_by_name(const gchar * name)
{
    static gchar buf[PATH_MAX + 1];

    g_snprintf(buf, sizeof(buf), "%s/%s", cachedir, name);

    return buf;
}

const gchar *lc_util_get_image_cache_path_by_name(const gchar * name)
{
    static gchar buf[PATH_MAX + 1];

    g_snprintf(buf, sizeof(buf), "%s/%s", imgcachedir, name);

    return buf;
}

GdkPixbuf *lc_util_load_pixbuf_from_resouce(const gchar * name)
{
    const gchar *path = lc_util_get_resource_by_name(name);
    if (path == NULL) {
        return NULL;
    }
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
    return pixbuf;
}

GdkPixbuf *lc_util_load_pixbuf_from_resource_with_size(const gchar * name,
                                                       int width,
                                                       int height)
{
    GdkPixbuf *pixbuf = lc_util_load_pixbuf_from_resouce(name);
    if (pixbuf) {
        GdkPixbuf *sized = gdk_pixbuf_scale_simple(pixbuf, width, height,
                                                   GDK_INTERP_TILES);
        g_object_unref(pixbuf);
        return sized;
    }
    return NULL;
}

GdkPixbuf *lc_util_load_pixbuf_from_cache(const gchar * name)
{
    const gchar *path = lc_util_get_image_cache_path_by_name(name);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
    return pixbuf;
}

GdkPixbuf *lc_util_load_pixbuf_from_cache_with_size(const gchar * name,
                                                    int width, int height)
{
    GdkPixbuf *pixbuf = lc_util_load_pixbuf_from_cache(name);
    if (pixbuf) {
        GdkPixbuf *new = gdk_pixbuf_scale_simple(pixbuf, width, height,
                                                 GDK_INTERP_NEAREST);
        g_object_unref(pixbuf);
        return new;
    }
    return pixbuf;
}

GdkPixbuf *lc_util_load_icon(const gchar * package_name, int width,
                             int height)
{
    GdkPixbuf *pixbuf =
        lc_util_load_pixbuf_from_cache_with_size(package_name, width,
                                                 height);
    if (pixbuf == NULL) {
        pixbuf =
            lc_util_load_pixbuf_from_resource_with_size
            (APPLICATION_DEFAULT_ICON, width, height);
    }
    return pixbuf;
}

static void apply_css_provider_forall(GtkWidget * widget, gpointer data)
{
    GtkCssProvider *provider = (GtkCssProvider *) data;
    if (GTK_IS_CONTAINER(widget)) {
        gtk_container_forall(GTK_CONTAINER(widget),
                             (GtkCallback) apply_css_provider_forall,
                             data);
    }
    GtkStyleContext *style =
        gtk_widget_get_style_context(GTK_WIDGET(widget));
    gtk_style_context_add_provider(style, GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void lc_util_load_css(GtkWidget * widget, const gchar * name)
{
    const gchar *path = lc_util_get_resource_by_name(name);
    GtkCssProvider *provider = gtk_css_provider_new();
    if (!gtk_css_provider_load_from_path(provider, path, NULL)) {
        g_warning("Failed to load CSS file: %s", path);
    } else {
        apply_css_provider_forall(widget, provider);
    }
    g_object_unref(provider);
}

GList *lc_util_clean_list_by(GList * list, GListElementNotify judge,
                             gpointer user_data, GDestroyNotify destroy)
{
    GList *lp = list;
    while (lp) {
        GList *next = g_list_next(lp);
        if (judge(lp->data, user_data)) {
            if (lp == list) {   /* first element */
                list = next;
                if (list) {
                    list->prev = NULL;
                }
                destroy(lp->data);
                g_list_free_1(lp);
            } else {
                lp->prev->next = next;
                next->prev = lp->prev;
                destroy(lp->data);
                g_list_free_1(lp);
            }
        }
        lp = next;
    }
    return list;
}

gssize lc_util_size_from_hex(const gchar buf[4])
{
    int i, j;
    int size = 0;
    for (i = 0; i < 4; i++) {
        int power = 3 - i;
        int base = 1;
        for (j = 0; j < power; j++) {
            base = base * 16;
        }
        int times = 0;
        if (buf[i] >= '0' && buf[i] <= '9') {
            times = buf[i] - '0';
        } else if (buf[i] >= 'a' && buf[i] <= 'f') {
            times = buf[i] - 'a' + 10;
        } else if (buf[i] >= 'A' && buf[i] <= 'F') {
            times = buf[i] - 'A' + 10;
        } else {
            return -1;
        }
        size += times * base;
    }
    return size;
}

#include <time.h>
const gchar *lc_util_date_time_format(guint64 time, const gchar * f)
{
    time_t timep = (time_t) time;
    struct tm *tp = localtime(&timep);
    static gchar buf[128];
    strftime(buf, sizeof(buf), f, tp);
    return buf;
}

/*
 * 获取系统的默认字体
 * 根据参数type的类型返回不同类型的字体，
 * 如果type是无效的值，则返回一般字体UTIL_SYSTEM_FONT_TYPE_NORMAL
 * 如果size为NULL，则忽略；如果不为NULL则返回字体大小
 */
#define SCHEMA_ID "org.gnome.desktop.interface"
gchar *lc_util_get_system_font(UtilSystemFontType type)
{
    GSettings *settings = g_settings_new(SCHEMA_ID);

    const gchar *font_type;
    switch (type) {
    case UTIL_SYSTEM_FONT_TYPE_MONO:
        font_type = "monospace-font-name";
        break;
    case UTIL_SYSTEM_FONT_TYPE_TITLEBAR:
        font_type = "titlebar-font";
        break;
    case UTIL_SYSTEM_FONT_TYPE_DOCUMENT:
        font_type = "document-font-name";
    default:
        font_type = "font-name";
        break;
    }

    gchar *font_name = g_settings_get_string(settings, font_type);
    g_object_unref(settings);

    return font_name;
}
