#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <dbus/dbus.h>

#include <string>
#include <map>
#include <functional>

namespace lkup69 {
class Interface
{
public:
        Interface(const std::string &name);
        virtual ~Interface();

        const std::string &getInterfaceName(void)
        {
                return m_name;
        }

        template<typename Class>
        void registerMember(const std::string &name, Class *o, 
                            DBusHandlerResult (Class::*pMemfunc)(DBusConnection *conn, DBusMessage *message))
        {
                m_method[name] = std::bind(pMemfunc, o, std::placeholders::_1, std::placeholders::_2);
        }

        void registerMember(const std::string &name, 
                            DBusHandlerResult (*pFunc)(DBusConnection *conn, DBusMessage *message));


        DBusHandlerResult methodCall(DBusConnection *conn, DBusMessage *message, const std::string &method);

        bool operator==(const std::string &interface);

private:
        Interface(const Interface &);
        Interface &operator=(const Interface &);

private:
        typedef std::map<std::string, std::function<DBusHandlerResult(DBusConnection *conn, DBusMessage *message)>> callbackMap_t;

        std::string    m_name;
        callbackMap_t  m_method;
};
}

#endif