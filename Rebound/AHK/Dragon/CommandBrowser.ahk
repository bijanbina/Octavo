#Include Functions.ahk

CoordMode, Mouse, Relative
arg1=%1%

if ( arg1="export" )
{
    EnvGet, ReboundPath, RE_PATH
    Run, %ReboundPath%\..\..\AccJoon\release\AccJoon.exe "L" "4.33.4.1", ,Min
    Sleep, 200 ;wait for run to finish
    Click, 400, 200

    Send, {Home}
    Sleep, 200
    Send, {Shift down}{End}{Shift up}
    Sleep, 200
    Send, {Ctrl down}{x}{Ctrl up}
    Sleep, 200
    Send, %ReboundPath%\AHK\Dragon\Commands.dat
    Sleep, 1000
    Send, {Enter}
    Sleep, 500
    Send, {Enter}
}
else if ( arg1="editor" )
{
    Send, {Enter}
    Sleep, 500
    Send, {Alt down}{m}{Alt up}
    Sleep, 500
    MouseMove, 130, 125
    Sleep, 500
    Click, 2
}
else
{
    count := re_getNum(arg1)

    if( count>0 )
    {
        position_y := count*50 + 200
        ; MsgBox, This command is not r hello %position_y%
        Click, 400, %position_y%
    }
    else
    {
        Click, 400, 200
    }
}