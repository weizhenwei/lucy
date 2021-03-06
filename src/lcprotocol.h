/*
 * lcprotocol.h
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

/*
 * the communication protocol between LUCY and LILY
 */

#ifndef __LC_PROTOCOL_H__
#define __LC_PROTOCOL_H__
#include <glib-2.0/glib.h>

/* all commands */
#define LC_PROTOCOL_APPLICATIONS "applications\n"
#define LC_PROTOCOL_VERSION      "version\n"
#define LC_PROTOCOL_PHONE        "phone\n"
#define LC_PROTOCOL_ICON         "icon:%s\n"
#define LC_PROTOCOL_SMS  		   "sms\n"

#define LC_PROTOCOL_HDR_LEN     (4)


typedef enum {
    LC_PROTOCOL_RESULT_OKAY,
    LC_PROTOCOL_RESULT_FAIL,
} LcProtocolResult;

LcProtocolResult lc_protocol_get_result_from_string(const gchar * str);
LcProtocolResult lc_protocol_get_result_from_bytes(GBytes * bytes);


/******************************APPLICATIONS******************************/
typedef enum {
    LC_PROTOCOL_APPLICATION_TYPE_ALL,
    LC_PROTOCOL_APPLICATION_TYPE_SYSTEM,
    LC_PROTOCOL_APPLICATION_TYPE_THIRD,
    LC_PROTOCOL_APPLICATION_TYPE_UNKNOWN,
} LcProtocolApplicationType;

typedef struct {
    gchar *package_name;
    gchar *app_name;
    gchar *version;
    gchar *installed_time;
    gchar *location;
    gchar *description;
    LcProtocolApplicationType type;
} LcProtocolApplication;

#define LC_PROTOCOL_APPLICATION_SIZE    (7)

LcProtocolApplication *lc_protocol_application_new(const gchar *
                                                   packageName,
                                                   const gchar * appName,
                                                   const gchar * version,
                                                   const gchar *
                                                   installedTime,
                                                   const gchar *
                                                   installedLocation,
                                                   const gchar * type,
                                                   const gchar *
                                                   description);
void lc_protocol_application_free(LcProtocolApplication * p);
LcProtocolApplication *lc_protocol_application_copy(const
                                                    LcProtocolApplication *
                                                    info);


/*
 * parse a string that contains infomation of an application.
 * data should be one line of response that's received from lily.
 */
LcProtocolApplication *lc_protocol_get_application(const gchar * data);
/*
 * parse the whole response of command 'applications'.
 * return a list that contains all applications infomation.
 *
 * the return list should be freed by lc_protocol_free_application_list()
 */
GList *lc_protocol_create_application_list(const gchar * data);
/*
 * free a list of LcProtocolApplication
 */
void lc_protocol_free_application_list(GList * list);

LcProtocolApplication *lc_protocol_application_find(GList * list,
                                                    const gchar * package);


/*******************************VERSION***********************************/
typedef struct {
    gchar *version;
} LcProtocolVersion;

LcProtocolVersion *lc_protocol_version_new(const gchar * v);
void lc_protocol_version_free(LcProtocolVersion * v);

/*
 * parse the response of command 'version\n'
 */
LcProtocolVersion *lc_protocol_create_version(const gchar * data);

LcProtocolVersion *lc_protocol_create_version_from_byte_array(GByteArray *
                                                              array);

/*******************************PHONE************************************/

typedef struct {
    gchar *model;
    gchar *brand;
    gchar *number;              /* phone number */
    gchar *availableMemory;
    gchar *totalMemory;
    gchar *availableSdCard;
    gchar *totalSdCard;
    gchar *availabelInternal;
    gchar *totalInternal;
} LcProtocolPhone;

#define LC_PROTOCOL_PHONE_SIZE  (9)

LcProtocolPhone *lc_protocol_phone_new(const gchar * model,
                                       const gchar * brand,
                                       const gchar * number,
                                       const gchar * availableMemory,
                                       const gchar * totalMemory,
                                       const gchar * availabelSdCard,
                                       const gchar * totalSdCard,
                                       const gchar * availabelInternal,
                                       const gchar * totalInternal);

void lc_protocol_phone_free(LcProtocolPhone * phone);

/*
 * parse the response of command 'phone\n'
 */
LcProtocolPhone *lc_protocol_create_phone(const gchar * data);

/**********************************ICON*********************************/

const gchar *lc_protocol_icon_command(const gchar * package);


/**********************************SMS*********************************/
typedef enum {
    LC_PROTOCOL_SMS_TYPE_INBOX = 1,
    LC_PROTOCOL_SMS_TYPE_SENT,
} LcProtocolSMSType;

typedef struct {
    gint thread_id;
    LcProtocolSMSType type;
    gchar *body;                /* the body of SMS */
    gchar *address;             /* the author of SMS (phone number) */
    guint64 date;               /* date in second */
    gint person;                /* person in contacts */
} LcProtocolSMS;

LcProtocolSMS *lc_protocol_sms_new(gint thread_id,
                                   LcProtocolSMSType type,
                                   const gchar * body,
                                   const gchar * address,
                                   guint64 date, gint person);
LcProtocolSMS *lc_protocol_sms_new_take(gint thread_id,
                                        LcProtocolSMSType type,
                                        gchar * body, gchar * address,
                                        guint64 date, gint person);
LcProtocolSMS *lc_protocol_sms_copy(LcProtocolSMS * self);

GList *lc_protocol_create_sms_list(const gchar * data);
void lc_protocol_free_sms_list(GList * list);

void lc_protocol_sms_free(LcProtocolSMS * sms);

#endif
