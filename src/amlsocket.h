/* amlsocket.h generated by valac 0.20.1, the Vala compiler, do not modify */


#ifndef __AMLSOCKET_H__
#define __AMLSOCKET_H__

#include <glib.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <string.h>

G_BEGIN_DECLS
#define TYPE_AML_SOCKET (aml_socket_get_type ())
#define AML_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_AML_SOCKET, AmlSocket))
#define AML_SOCKET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_AML_SOCKET, AmlSocketClass))
#define IS_AML_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_AML_SOCKET))
#define IS_AML_SOCKET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_AML_SOCKET))
#define AML_SOCKET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_AML_SOCKET, AmlSocketClass))
typedef struct _AmlSocket AmlSocket;
typedef struct _AmlSocketClass AmlSocketClass;
typedef struct _AmlSocketPrivate AmlSocketPrivate;

struct _AmlSocket {
    GSocket parent_instance;
    AmlSocketPrivate *priv;
};

struct _AmlSocketClass {
    GSocketClass parent_class;
};


GType aml_socket_get_type(void) G_GNUC_CONST;
AmlSocket *aml_socket_new(const gchar * addr, guint16 port);
AmlSocket *aml_socket_construct(GType object_type, const gchar * addr,
                                guint16 port);

/*
 * 发起链接
 */
void aml_socket_connect_async(AmlSocket * socket,
                              GAsyncReadyCallback callback,
                              gpointer user_data);
gboolean aml_socket_connect_finish(GAsyncResult * res);

/*
 * 发送/接收数据
 */
gssize aml_socket_send(AmlSocket * socket, const gchar * buffer,
                       gsize size);
gssize aml_socket_receive(AmlSocket * socket, gchar * buffer, gsize size);
/* 读取一行 */
gssize aml_socket_receive_line(AmlSocket * socket, gchar * buffer,
                               gsize size);

/*
 * 该函数会发送一个协议请求，
 * 然后等待响应，直到接受到了完整的响应（一个空行结束）才回调
 */
void aml_socket_send_command_async(AmlSocket * socket,
                                   const gchar * command,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data);
/*
 * 正常则返回响应的数据，否则返回NULL。返回的数据需要手动释放
 */
gchar *aml_socket_send_command_finish(GAsyncResult * result);

G_END_DECLS
#endif
