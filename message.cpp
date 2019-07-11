#include "message.h"

using namespace lkup69;

Message::Message(DBusConnection *conn) : m_conn(conn)
{
}

Message::~Message()
{
        if(m_msgPtr)
                dbus_message_unref(m_msgPtr);
}

int Message::initReply(DBusMessage *message) 
{
        m_msgPtr = dbus_message_new_method_return(message);

        if(m_msgPtr == nullptr)
                return -1;

        dbus_message_iter_init_append(m_msgPtr, &m_msgIter);

        return 0;
}

int Message::reply(void)
{
        if(m_conn == nullptr)
                return -1;

        if(!dbus_connection_send(m_conn, m_msgPtr, nullptr))
                return -1;

        dbus_connection_flush(m_conn);

        return 0;
}
