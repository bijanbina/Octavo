#ifndef RE_XBOX_WIN32_H
#define RE_XBOX_WIN32_H

#include "backend.h"
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <windows.h>
#include <iostream>
#include <xinput.h>

#define XINPUT_GAMEPAD_GUIDE 0x0400

struct ReXboxController
{
    unsigned long eventCount; //event counter, increases with every controller event,
                  //but for some reason not by one.
    unsigned short up:1, down:1, left:1, right:1, start:1, back:1, l3:1, r3:1,
                   lButton:1, rButton:1, guideButton:1, unknown:1, aButton:1,
                   bButton:1, xButton:1, yButton:1; // button state bitfield
    unsigned char lTrigger;  //Left Trigger
    unsigned char rTrigger;  //Right Trigger
    short lJoyY;  //Left Joystick Y
    short lJoyx;  //Left Joystick X
    short rJoyY;  //Right Joystick Y
    short rJoyX;  //Right Joystick X
};

class ReXboxWin32 : public QObject
{
    Q_OBJECT

public:
    explicit ReXboxWin32(QObject *parent = 0);
    ~ReXboxWin32();

signals:
    void buttonGuideChanged(bool);

public slots:
    void ReXbox_getGuideBtn();

private:
    QTimer *guideTimer; //XBOX Guide Button check
    int guide_last_val;

    typedef int(__stdcall * pICFUNC)(int, ReXboxController &);
    pICFUNC getControllerData;

    typedef DWORD (WINAPI *XInputGetStateExProc)(DWORD dwUserIndex, XINPUT_STATE *pState);
    XInputGetStateExProc XInputGetStateEx;

    typedef DWORD (WINAPI *XInputPowerOffProc)(DWORD dwUserIndex);
    XInputPowerOffProc XInputPowerOff;

};

#endif // RE_XBOX_WIN32_H
