#include "interface.h"

using namespace lkup69;
using namespace std;

Interface::Interface(const std::string &name) : m_name(name)
{
}

Interface::~Interface()
{
}

void Interface::registerMember(const string &name, 
                               DBusHandlerResult (*pFunc)(DBusConnection *conn, DBusMessage *message))
{
        m_method[name] = pFunc;
}

DBusHandlerResult Interface::methodCall(DBusConnection *conn, DBusMessage *message, const string &method)
{
        auto memfunc = m_method.find(method);

        if(memfunc != m_method.end()) 
                return memfunc->second(conn, message);

        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

bool Interface::operator==(const string &interface)
{
        if(m_name == interface)
                return true;

        return false;
}

