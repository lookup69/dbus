#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <dbus/dbus.h>

namespace lkup69 {
class Message
{
public:
        Message(DBusConnection *conn);
        ~Message();

        int initReply(DBusMessage *message);
        int reply(void);

        void packArg(bool val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_BOOLEAN, &val);
        }

        void packArg(char val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_BYTE, &val);
        }

        void packArg(short val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_INT16, &val);
        }

        void packArg(unsigned short val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_UINT16, &val);
        }

        void packArg(int val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_INT32, &val);
        }

        void packArg(unsigned int val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_UINT32, &val);
        }

        void packArg(long val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_INT64, &val);
        }

        void packArg(unsigned long val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_UINT64, &val);
        }

        void packArg(long long val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_INT64, &val);
        }

        void packArg(unsigned long long val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_INT64, &val);
        }

        void packArg(float val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_DOUBLE, &val);
        }

        void packArg(double val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_DOUBLE, &val);
        }

        void packArg(const char *val)
        {
                dbus_message_iter_append_basic(&m_msgIter, DBUS_TYPE_STRING, &val);
        }

        void packArgs(void)
        {
        }

        template<typename T, typename ...Args>
        void packArgs(T first, Args... args)
        {
                packArg(first);    // single

                packArgs(args...); // variadic
        }

        template<typename ...Args>
        int append(Args... args)
        {
                if(m_msgPtr == nullptr) 
                        return -1;

                packArgs(args...);

                return 0;
        }

private:
        Message(const Message &);
        Message &operator=(const Message &);

private:
        DBusConnection  *m_conn = nullptr;
        DBusMessage     *m_msgPtr = nullptr;
        DBusMessageIter m_msgIter;
};
}
#endif