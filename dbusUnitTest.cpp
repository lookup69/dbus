#include <unistd.h>
#include <thread>
#include "dbusMsgHandler.h"
#include "message.h"
#include "interface.h"

using namespace lkup69;

class TestInterface : public Interface
{
public:
        TestInterface(const std::string &name) : Interface(name)
        {
                dbus_error_init(&m_err);

                registerMember("MyTest1", this, &TestInterface::methodTest);
                registerMember("signalTest1", this, &TestInterface::signalTest);
        }

        ~TestInterface()
        {
                 dbus_error_free(&m_err);
        }

        // callback member function
        DBusHandlerResult methodTest(DBusConnection *conn, DBusMessage *message)
        {
                const char *msg;
                const char *msg2 = "This is member function of Interface callback";
                int         val = 999;

                if (!dbus_message_get_args(message, &m_err,
                                           DBUS_TYPE_STRING, &msg,
                                           DBUS_TYPE_INVALID))
                        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
                
                Message replyMsg(conn);

                replyMsg.initReply(message);
                replyMsg.append(msg, val, msg2);
                replyMsg.reply();

                return DBUS_HANDLER_RESULT_HANDLED;
        }

        // callback member function
        DBusHandlerResult signalTest(DBusConnection *conn, DBusMessage *message)
        {
                printf("Signal with member function with the Interface class\n");
                return DBUS_HANDLER_RESULT_HANDLED;
        }

private:
        DBusError m_err;
};

// callback normal function
DBusHandlerResult methodTest(DBusConnection *conn, DBusMessage *message)
{
        DBusHandlerResult ret = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        DBusError         m_err;        
        const char        *msg;
        const char        *msg2 = "This is normal function callback";
        int               val = 777777;

        dbus_error_init(&m_err);

        if (dbus_message_get_args(message, &m_err,
                                   DBUS_TYPE_STRING, &msg,
                                   DBUS_TYPE_INVALID)) {
                Message replyMsg(conn);

                replyMsg.initReply(message);
                replyMsg.append(msg, val, msg2);
                replyMsg.reply();

                ret = DBUS_HANDLER_RESULT_HANDLED;
        } else {
                ret = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        }

        dbus_error_free(&m_err);

        return ret;
}

// callback normal function
DBusHandlerResult signalTest(DBusConnection *conn, DBusMessage *message)
{
        printf("Signal with normal function\n");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main()
{
        DBusMessageHandler dbus("org.example.TestServer");
        TestInterface      interface1("org.example.TestInterface.foo");
        TestInterface      interface2("org.example.TestInterface.bar");

        auto     loop = []() {
                while(1) {
                        sleep(1);
                }        
        };

        interface1.registerMember("MyTest2", methodTest);
        interface1.registerMember("signalTest2", signalTest);
        dbus.registerInterface("/path1", &interface1);

        interface2.registerMember("MyTest2", methodTest);
        interface2.registerMember("signalTest2", signalTest);
        dbus.registerInterface("/path1", &interface2);

        dbus.serverInit("/path1");
        dbus.eventDispatch();


        //ut.run();
        std::thread t(loop);
        t.join();
        
        return 0;
}
