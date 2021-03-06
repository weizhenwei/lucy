/*
 * lccommander.h
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
#ifndef __LC_COMMANDER_H__
#define __LC_COMMANDER_H__
#include <gtk/gtk.h>

typedef enum {
    LC_COMMANDER_INIT_OK,
    LC_COMMANDER_INIT_FAILED_FORWARD,
    LC_COMMANDER_INIT_FAILED_SERVER,
    LC_COMMANDER_INIT_FAILED_INSTALL,
    LC_COMMANDER_INIT_FAILED_START,
    LC_COMMANDER_INIT_FAILED_VERSION,
} LcCommanderInitResult;
/*
 * initialize all adb settings and connect to Android
 */
typedef void (*LcCommanderInitCallback) (LcCommanderInitResult result,
                                         gpointer user_data);
void lc_commander_init_async(LcCommanderInitCallback callback,
                             gpointer data);


typedef void (*LcCommanderCommandCallback) (const gchar * cmd,
                                            GByteArray * result,
                                            gpointer data);
void lc_commander_send_command_async(const gchar * cmd,
                                     LcCommanderCommandCallback callback,
                                     gpointer data);


#endif
