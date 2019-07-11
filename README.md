# dbus
DBus utility library


compile:

g++ -g -std=c++17 -Wall -o dbusUT dbusMsgHandler.cpp message.cpp interface.cpp dbusUnitTest.cpp   `pkg-config --cflags --libs /usr/lib/x86_64-linux-gnu/pkgconfig/dbus-1.pc` `pkg-config --cflags --libs dbus-glib-1` -pthread

Test:

Server:
    dbusUT
    
Client:
    Method:
        dbus-send --session --type=method_call --print-reply --dest=org.example.TestServer /path1 org.example.TestInterface.foo.MyTest1 string:"aaaaaaaaaaa"
        dbus-send --session --type=method_call --print-reply --dest=org.example.TestServer /path1 org.example.TestInterface.foo.MyTest2 string:"aaaaaaaaaaa"
        dbus-send --session --type=method_call --print-reply --dest=org.example.TestServer /path1 org.example.TestInterface.bar.MyTest1 string:"aaaaaaaaaaa"
        dbus-send --session --type=method_call --print-reply --dest=org.example.TestServer /path1 org.example.TestInterface.bar.MyTest2 string:"aaaaaaaaaaa"
        
    Signal:
        dbus-send --session --type=signal  --dest=org.example.TestServer /path1 org.example.TestInterface.foo.signalTest1
        dbus-send --session --type=signal  --dest=org.example.TestServer /path1 org.example.TestInterface.bar.signalTest1
