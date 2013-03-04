QConnect
==========

##Multiple terminal display

![QConnect Screenshot](./window.png "2013-03-02")

In order to build this project you need to install the QtSerialPort module.

### Installing QtSerialPort

Go to the [QtSerialPort page](http://qt-project.org/wiki/QtSerialPort) in the Qt Project Wiki for most up-to-date instructions. They didn't really work for me when I followed them to the letter, but the following steps should get you up and running. At least they worked on my Windows 8 machine, as of 2013-03-03:

1. Install Perl and make sure it's in your system PATH.
2. Download the QtSerialPort source:

    *git clone git://gitorious.org/qt/qtserialport.git*
3. Open up a Qt command prompt and go to the project folder.

    *C:\qt-qtserialport> qmake qtserialport.pro*
	
	*C:\qt-qtserialport> qmake*
	
	*C:\qt-qtserialport> mingw32-make*
	
	*C:\qt-qtserialport> mingw32-make install*
4. Open a project in Qt Creator and add this to your **.pro** file:
    QT += serialport
5. And these includes (only the first one is necessary):
    \#include <QtSerialPort/qserialport.h>
	\#include <QtSerialPort/qserialportinfo.h>
6. Use the **QSerialPort** class as your IO device. Look at my DataConnection class for more tips on usage.

