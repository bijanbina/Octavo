#include "re_buttons.h"
#include <QThread>

ReButtons::ReButtons(QObject *item, QObject *switcher, ReState *st, QObject *parent) : QObject(parent)
{
    ui = item;
    uiSwitcher = switcher;
    state = st;

#if __linux__
    //switching tab
    timer_tab = new QTimer;
//    timer_tab->setSingleShot(true);
    connect(timer_tab, SIGNAL(timeout()), this, SLOT(tab_timeout()));
#endif
}

#ifdef _WIN32
void ReButtons::buttonAPressed()
{
    if( state->ui_visible )
    {
        if ( state->getMode()==RE_MODE_APPLICATION )
        {
            state->toggleUi(ui);
            state->api->openApp(RE_WIN_TELEGRAM);
            qDebug() << "openApp";
        }
        else
        {
            state->propageteMode(RE_MODE_APPLICATION);
        }
    }
    else
    {
        executeAhk("button_a");
    }
}

void ReButtons::buttonBPressed()
{
    if( state->ui_visible )
    {
         if ( state->getMode()==RE_MODE_APPLICATION )
         {
             state->toggleUi(ui);

             if( state->vpn_connected )
             {
                 system("rasdial MK2 /DISCONNECT");
             }
             else
             {
                 system("rasdial MK2 km93057 48868");
             }
         }
         else if ( state->getMode()==RE_MODE_SPOTIFY )
         {
             executScript("b", RE_SCR_SPOTIFY);
         }
         else
         {
             state->propageteMode(RE_MODE_SPOTIFY);
         }
    }
    else
    {
        executeAhk("button_b");
    }
}

void ReButtons::buttonXPressed()
{
    if( state->ui_visible )
    {
        if ( state->getMode()==RE_MODE_APPLICATION )
        {
            state->toggleUi(ui);
            state->api->openApp(RE_WIN_FIREFOX);
            qDebug() << "openApp";
        }
        else if ( state->getMode()==RE_MODE_SPOTIFY )
        {
            executScript("x", RE_SCR_SPOTIFY);
        }
        else
        {
            state->toggleUi(ui);
            QThread::msleep(20);
            executScript("x", RE_SCR_UI);
        }
    }
    else
    {
        executeAhk("button_x");
    }
}

void ReButtons::buttonYPressed()
{
    if( state->ui_visible )
    {
        if ( state->getMode()==RE_MODE_APPLICATION )
        {
            state->toggleUi(ui);
            state->api->openApp(RE_WIN_SPOTIFY);
            qDebug() << "openApp";
        }
        else if ( state->getMode()==RE_MODE_SPOTIFY )
        {
            executScript("y", RE_SCR_SPOTIFY);
        }
        else
        {
            state->propageteMode(RE_MODE_SWITCH);
        }
    }
    else
    {
        executeAhk("button_y");
    }
}

void ReButtons::buttonStartPressed()
{
    if( state->ui_visible )
    {
        state->toggleUi(ui);
    }
    else
    {
        state->toggleUi(ui);
    }
}

void ReButtons::buttonGuidePressed()
{
    if( state->ui_visible )
    {
        state->toggleUi(ui);
        QThread::msleep(20);
//        executScript("select");
        state->hardware->disconnectXbox();
    }
    else
    {
        executeAhk("select_button");
    }
}

void ReButtons::buttonSelectPressed()
{
    if( state->ui_visible )
    {
        if ( state->getMode()==RE_MODE_SPOTIFY )
        {
            executScript("button_select", RE_SCR_SPOTIFY);
        }
        else if ( state->getMode()==RE_MODE_APPLICATION )
        {
            executeAhk("suspend");
        }
        else
        {
            state->toggleUi(ui);
            QThread::msleep(20);
            executScript("button_select", RE_SCR_UI);
        }
    }
    else
    {
        if( isItemVisible(uiSwitcher) )
        {
            QMetaObject::invokeMethod(uiSwitcher, "activeNextProcess");
        }
        else
        {
            state->showSwither(uiSwitcher);
        }
    }
}
#elif __linux__
int sh_alt_down = 0;
int sh_child_count = 0;

void ReButtons::tab_timeout()
{
    timer_tab->stop();
//    sendFakeEvent(0, XK_Alt_L); //A release
//    system("xdotool keyup 0xffea &");
    qDebug() <<  "Release Window";
}

void ReButtons::buttonAPressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
         system("./Scripts/focus_window spotify");
    }
    else
    {
        system("./Scripts/button_a &");
    }
}

void ReButtons::buttonBPressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
         system("firefox &");
    }
    else
    {
        system("./Scripts/button_b &");
    }
}

void ReButtons::buttonXPressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
         QThread::msleep(200);
         system("xdotool key Alt+F4 &");
    }
    else
    {
        system("./Scripts/button_x &");
    }
}

void ReButtons::buttonYPressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
         system("./Scripts/focus_window firefox");
    }
    else
    {
        system("./Scripts/button_y &");
    }
}

void ReButtons::buttonStartPressed()
{
    if( state->ui_visible )
    {
        state->toggleUi(ui);
        system("./Scripts/menu_button &");
    }
    else
    {
        state->toggleUi(ui);
    }
}

//in native mode add sleep
void ReButtons::buttonSelectPressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
         system("xdotool key XF86AudioMute &");
    }
    else
    {
        if( state->ui_visible )
        {
            QMetaObject::invokeMethod(uiSwitcher, "activeNextProcess");
        }
        else
        {
            state->showSwither(uiSwitcher);
        }
    }
}

void ReButtons::buttonGuidePressed()
{
    if( state->ui_visible )
    {
         state->toggleUi(ui);
    }
}

#endif
