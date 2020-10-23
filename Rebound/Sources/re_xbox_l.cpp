#include "re_xbox_l.h"
#include <string.h>

#define JOYSTICK_DELAY 100

ReXboxL::ReXboxL(QObject *item, QObject *parent) : QObject(parent)
{
    //init
    ui = item;
    charBuffer = '0';
    isBufferEmpty = true;
    commandMode = false;
    exec = new ReExec(ui);

    //read from stdin
    stdin_notify = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read, this);
    connect(stdin_notify, SIGNAL(activated(int)), this, SLOT(readyData()));
    stdin_notify->setEnabled(true);

    stdin_file = new QFile;
    stdin_file->open(stdin, QIODevice::ReadOnly);
}

void ReXboxL::readyData()
{
    QString line = stdin_file->readLine();
    QStringList space_separated;
    if( line.contains("type 1") || line.contains("type 3"))
    {
        space_separated = line.split(" ");

        if( space_separated.count()>10 )
        {
            QString key_code = space_separated[8];
            QString key_val = space_separated[10];

            //clean string
            key_code.chop(2);
            key_code.remove(0, 2);

            key_val.chop(1);

//                 qDebug() << key_code << key_val;

            int key_val_int = key_val.toInt();
            keyParser(key_code, key_val_int);
        }
    }
}

void ReXboxL::keyParser(QString key, int value)
{
    qDebug() << key << value;
    if( key==RE_KEY_HOMEPAGE )
    {
        buttonGuideChanged(value);
    }
    else if( key==RE_KEY_SOUTH )
    {
        buttonAChanged(value);
    }
    else if( key==RE_KEY_EAST )
    {
        buttonBChanged(value);
    }
    else if( key==RE_KEY_NORTH )
    {
        buttonXChanged(value);
    }
    else if( key==RE_KEY_WEST )
    {
        buttonYChanged(value);
    }
    else if( key==RE_KEY_START )
    {
        buttonStartChanged(value);
    }
    else if( key==RE_KEY_BACK )
    {
        buttonSelectChanged(value);
    }
    else if( key==RE_KEY_HAT0Y )
    {
        if( value==1 )
        {
            buttonDownChanged(1);
        }
        else if( value==-1 )
        {
            buttonUpChanged(1);
        }
        else if( value==0 )
        {
            buttonDownChanged(0);
            buttonUpChanged(0);
        }
    }
    else if( key==RE_KEY_HAT0X )
    {
        if( value==1 )
        {
            buttonRightChanged(1);
        }
        else if( value==-1 )
        {
            buttonLeftChanged(1);
        }
        else if( value==0 )
        {
            buttonRightChanged(0);
            buttonLeftChanged(0);
        }
    }
    else if( key==RE_KEY_TR )
    {
        buttonR1Changed(value);
    }
    else if( key==RE_KEY_TL )
    {
        buttonL1Changed(value);
    }
    else if( key==RE_KEY_GAS )
    {
        buttonR2Changed(value/1023.0);
    }
    else if( key==RE_KEY_BRAKE )
    {
        buttonL2Changed(value/1023.0);
    }
    else if( key==RE_KEY_THUMBR )
    {
        buttonR3Changed(value);
    }
    else if( key==RE_KEY_THUMBL )
    {
        buttonL3Changed(value);
    }
    else if( key==RE_KEY_X )
    {
        buttonAxisLxChanged(value/65530.0);
    }
    else if( key==RE_KEY_Y )
    {
        buttonAxisLyChanged(value/65530.0);
    }
    else if( key==RE_KEY_Z )
    {
        buttonAxisRxChanged(value/65530.0);
    }
    else if( key==RE_KEY_RZ )
    {
        buttonAxisRyChanged(value/65530.0);
    }
}

ReXboxL::~ReXboxL()
{
    qDebug() << "Closing Server";
}

void ReXboxL::buttonAChanged(bool value)
{
    qDebug() << "A Pressed";
    if ( value==1 )
    {
        emit buttonAPressed();
    }
}

void ReXboxL::buttonBChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonBPressed();
    }
}

void ReXboxL::buttonXChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonXPressed();
    }
}

void ReXboxL::buttonYChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonYPressed();
    }
}

void ReXboxL::buttonL1Changed(bool value)
{
    if ( value==1 )
    {
        emit buttonL1Pressed();
    }
}

void ReXboxL::buttonL2Changed(double value)
{
    if ( value==1 )
    {
        emit buttonL2Pressed();
    }
}

void ReXboxL::buttonL3Changed(bool value)
{
    qDebug() << "l3 pressed";
    if ( value==1 )
    {
        emit buttonL3Pressed();
    }
}

void ReXboxL::buttonR1Changed(bool value)
{
    if ( value==1 )
    {
        emit buttonR1Pressed();
    }
}

void ReXboxL::buttonR2Changed(double value)
{
    qDebug() << "r2 pressed";
    if ( value==1 )
    {
        emit buttonR2Pressed();
    }
}

void ReXboxL::buttonR3Changed(bool value)
{
    qDebug() << "r3 pressed";
    if ( value==1 )
    {
        emit buttonR3Pressed();
    }
}

void ReXboxL::buttonAxisLxChanged(double value)
{
    if( last_la_x==0 )
    {
        if( value<0.02 )
        {
            last_la_x = 1;
            emit buttonLAxisRight();
        }
        else if( value>0.98  )
        {
            last_la_x = 1;
            emit buttonLAxisLeft();
        }
    }
    else if( value<0.5 && value>0.25 )
    {
        last_la_x = 0;
    }
}

void ReXboxL::buttonAxisLyChanged(double value)
{
    if( last_la_y==0 )
    {
        if( value<0.02 ) //up
        {
            last_la_y = 1;
            emit buttonLAxisUp();
        }
        else if( value>0.98 ) //down
        {
            last_la_y = 1;
            emit buttonLAxisDown();
        }
    }
    else if( value<0.75 && value>0.25 )
    {
        last_la_y = 0;
    }
}

void ReXboxL::buttonAxisRxChanged(double value)
{
    if( last_la_x==0 )
    {
        if( value<0.02 )
        {
            last_la_x = 1;
            emit buttonRAxisRight();
        }
        else if( value>0.98 )
        {
            last_la_x = 1;
            emit buttonRAxisLeft();
        }
    }
    else if( value<0.75 && value>0.25 )
    {
        last_la_x = 0;
    }
}

void ReXboxL::buttonAxisRyChanged(double value)
{
//    qDebug() << value;
    if( last_ra_y==0 )
    {
        if( value<0.02 ) //up
        {
            last_ra_y = 1;
            emit buttonRAxisUp();
        }
        else if( value>0.98 ) //down
        {
            last_ra_y = 1;
            emit buttonRAxisDown();
        }
    }
    else if( value<0.75 && value>0.25 )
    {
        last_ra_y = 0;
    }
}

void ReXboxL::buttonStartChanged(bool value)
{
    if ( value==0 )
    {
        emit buttonStartPressed();
    }
}

void ReXboxL::buttonSelectChanged(bool value)
{
    if ( value==0 )
    {
        emit buttonSelectChanged(1);
    }
}

void ReXboxL::buttonCenterChanged(bool value)
{
    if ( value==0 )
    {
        qDebug() << "Center pressed";
        emit buttonCenterPressed();
    }
}

void ReXboxL::buttonGuideChanged(bool value)
{
    qDebug() << "Guide pressed";
    if ( value==1 )
    {
        emit buttonGuidePressed();
    }
}

void ReXboxL::buttonLeftChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonLeftPressed();
    }
}

void ReXboxL::buttonRightChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonRightPressed();
    }
}

void ReXboxL::buttonUpChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonUpPressed();
    }
}

void ReXboxL::buttonDownChanged(bool value)
{
    if ( value==1 )
    {
        emit buttonDownPressed();
    }
}