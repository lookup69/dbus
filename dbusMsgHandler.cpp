#include "dbusMsgHandler.h"
#include <thread>

using namespace lkup69;

DBusMessageHandler::DBusMessageHandler(const std::string &busName, DBusBusType busType)
{
        dbus_error_init(&m_dbusErr);

        m_conn = dbus_bus_get(busType, &m_dbusErr);
        if(m_conn) {
                int ret;

                ret = dbus_bus_request_name(m_conn, busName.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING , &m_dbusErr);
                if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
                        dbus_connection_close(m_conn);
                        m_conn = nullptr;

                        fprintf(stderr, "Failed to request name on bus: %s\n", m_dbusErr.message);
                }
        } else {
                fprintf(stderr, "Failed to get a session DBus connection: %s\n", m_dbusErr.message);
        }
}

DBusMessageHandler::~DBusMessageHandler()
{
        m_bExit = true;

        if(m_conn)
                dbus_connection_close(m_conn);
        dbus_error_free(&m_dbusErr);
}


int DBusMessageHandler::serverInit(const std::string &objectPath)
{
        if(m_conn == nullptr)
                return -1;

        if (!dbus_connection_register_object_path(m_conn, 
                                                  objectPath.c_str(), 
                                                  &m_objectPathVTable, 
                                                  static_cast<void *>(this))) {
                fprintf(stderr, "Failed to register a object path for 'TestObject'\n");
                
                return -1;
        }

        return 0;
}

void DBusMessageHandler::registerInterface(const std::string &objectPath, Interface *interface)
{
        auto entry = m_interfaceMap.find(objectPath);

        if(entry == m_interfaceMap.end()) 
                m_interfaceMap[objectPath] = std::vector<Interface *>{interface};
        else 
                entry->second.push_back(interface);
}

void DBusMessageHandler::eventDispatch(void)
{
        auto dispatchLoop = [&]() {
                while (!m_bExit) {
                        dbus_connection_read_write_dispatch (m_conn, -1);
                }
        };

        std::thread t(dispatchLoop);
        t.detach();
}

DBusHandlerResult DBusMessageHandler::messageHandler_(DBusConnection *conn, DBusMessage *message)
{
        DBusHandlerResult ret = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        std::string       method = dbus_message_get_member(message);
        std::string       objectPath = dbus_message_get_path(message);
        std::string       interface = dbus_message_get_interface(message);
        int               messageType = dbus_message_get_type(message);

        if((messageType == DBUS_MESSAGE_TYPE_METHOD_CALL) ||
           (messageType == DBUS_MESSAGE_TYPE_SIGNAL)) {
                auto object = m_interfaceMap.find(objectPath);

                if(object != m_interfaceMap.end()) {
                        //const auto &interfaceVec = object->second;
                        std::vector<Interface *> &interfaceVec = object->second;
                        for(Interface *item : interfaceVec) {
                                if(*item == interface) {
                                        ret = item->methodCall(conn, message, method);
                                }
                        }
                }
        } else {
                printf("DBUS_UNKNOW\n");
        }

        return ret;
}
