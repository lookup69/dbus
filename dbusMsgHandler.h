#ifndef _DBUS_MSSAGE_HANDLER_H_
#define _DBUS_MSSAGE_HANDLER_H_

#include <dbus/dbus.h>
#include <string>
#include <vector>

#include "interface.h"

namespace lkup69 {
class DBusMessageHandler
{
public:
        DBusMessageHandler(const std::string &busName, DBusBusType busType = DBUS_BUS_SESSION);
        ~DBusMessageHandler();
        
        int serverInit(const std::string &objectPath);
        void registerInterface(const std::string &objectPath, Interface *interface);
        void eventDispatch(void);

private:
        typedef std::map<std::string, std::vector<Interface *>> interfaceMap_t;

        // https://dbus.freedesktop.org/doc/api/html/structDBusObjectPathVTable.html
        const DBusObjectPathVTable m_objectPathVTable = {
                unregister_,    // DBusObjectPathUnregisterFunction 	unregister_function
                handler_        // DBusObjectPathMessageFunction 	message_function
        };       

        bool                       m_bExit = false;
        DBusConnection             *m_conn = nullptr;
        DBusError                  m_dbusErr;
        interfaceMap_t             m_interfaceMap;

        // dbus handler function
        DBusHandlerResult messageHandler_(DBusConnection *conn, DBusMessage *message);

        virtual void objectPathUnregister_(DBusConnection *conn)
        {
        }

        static DBusHandlerResult handler_(DBusConnection *conn, DBusMessage *msg, void *cookie)
        {
             DBusMessageHandler *obj = static_cast<DBusMessageHandler *>(cookie);   

             return obj->messageHandler_(conn, msg);
        }

        static void unregister_(DBusConnection *conn, void *cookie)
        {
             DBusMessageHandler *obj = static_cast<DBusMessageHandler *>(cookie);      

             obj->objectPathUnregister_(conn);
        }
};
}

#endif
