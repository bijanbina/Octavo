#include "re_captain_l.h"
#include <unistd.h>
#include <fcntl.h>

ReCaptainL::ReCaptainL(ReState *st, QObject *parent): QObject(parent)
{
    state = st;

    struct uinput_setup usetup;

    uinput_f = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    /*
     * The ioctls below will enable the device that is about to be
     * created, to pass key events, in this case the space key.
     */
    ioctl(uinput_f, UI_SET_EVBIT, EV_KEY);

    // enable all the keys
    // no better solution found :(
    for( int keycode=KEY_ESC ; keycode<KEY_COMPOSE ; keycode++)
    {
        ioctl(uinput_f, UI_SET_KEYBIT, keycode);
    }

    ioctl(uinput_f, UI_SET_KEYBIT, KEY_SPACE);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234; /* sample vendor */
    usetup.id.product = 0x5678; /* sample product */
    strcpy(usetup.name, "Rebound Keyboard");

    ioctl(uinput_f, UI_DEV_SETUP, &usetup);
    ioctl(uinput_f, UI_DEV_CREATE);
}

ReCaptainL::~ReCaptainL()
{
    ioctl(uinput_f, UI_DEV_DESTROY);
    close(uinput_f);
}

void ReCaptainL::setKey(int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(uinput_f, &ie, sizeof(ie));
}

void ReCaptainL::sendKey(int key_val)
{
    pressKey(key_val);
    releaseKey(key_val);
}

void ReCaptainL::pressKey(int key_val)
{
    /* Key press, report the event, send key release, and report again */
    setKey(EV_KEY, key_val, 1);
    setKey(EV_SYN, SYN_REPORT, 0);
}

void ReCaptainL::releaseKey(int key_val)
{
    setKey(EV_KEY, key_val, 0);
    setKey(EV_SYN, SYN_REPORT, 0);
}

void ReCaptainL::pressModifier(CaptainCommand command)
{
    modifiers.append(command);
    pressKey(command.val1);
}

void ReCaptainL::releaseModifiers()
{
    for( int i=0 ; i<modifiers.count() ; i++ )
    {
        releaseKey(modifiers[i].val1);
    }
    modifiers.clear();
}

void ReCaptainL::execute(QVector<CaptainCommand> commands)
{
    for( int i=0 ; i<commands.length() ; i++ )
    {
        if( state->isSleep() )
        {
            if( commands[i].type!=RE_COMMAND_META )
            {
                continue;
            }
            else
            {
                if( isWakeUp(commands[i]) )
                {
                    state->wakeUp();
                }
            }
        }
        execCommand(commands[i]);
    }
}

void ReCaptainL::execCommand(CaptainCommand command)
{
    if( command.type==RE_COMMAND_NATO ||
        command.type==RE_COMMAND_DIGIT ||
        command.type==RE_COMMAND_KEY )
    {
        for( int j=0 ; j<command.val2 ; j++ )
        {
            sendKey(command.val1);
        }
        releaseModifiers();
    }
    else if( command.type==RE_COMMAND_MOD )
    {
        pressModifier(command);
    }
    else if( command.type==RE_COMMAND_META )
    {
        execMeta(command);
    }
}

bool ReCaptainL::isLastCmdReeatable(QVector<CaptainCommand> commands)
{
    if( commands.count()==0 )
    {
        return false;
    }
    int last_i = commands.count()-1; //last index
//    int key_code = commands[last_i].val1;
    int cmd_type = commands[last_i].type;

    if( cmd_type==RE_COMMAND_KEY )
    {
        return true;
    }

    return false;
}

bool ReCaptainL::isLastMeta(QVector<CaptainCommand> commands)
{
    if( commands.count()==0 )
    {
        return false;
    }

    int last_i = commands.count()-1; //last index
    int cmd_type = commands[last_i].type;

    if( cmd_type==RE_COMMAND_META )
    {
        return true;
    }

    return false;
}

bool ReCaptainL::isWakeUp(CaptainCommand command)
{
    if( command.type==RE_COMMAND_META )
    {
        return true;
    }

    return false;
}

void ReCaptainL::execMeta(CaptainCommand command)
{
    if( command.val2==0 )
    {
        return;
    }

    if( command.val1==RE_META_OPEN )
    {
        QString cmd = "./Scripts/open ";
        cmd += QString::number(command.val2)+ " &";
        system(cmd.toStdString().c_str());
    }
    else if( command.val1==RE_META_SYS )
    {

    }
    else if( command.val1==RE_META_WAKE )
    {

    }
    else if( command.val1==RE_META_START )
    {

    }
    else if( command.val1==RE_META_PAGE )
    {

    }
    else if( command.val1==RE_META_GO )
    {

    }
}

int ReCaptainL::keyCode2Digit(QString key_code)
{
    int code = key_code.toInt();

    if( code==11 )
    {
        return 0;
    }
    else
    {
        return code-1;
    }
}
