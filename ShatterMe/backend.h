#ifndef BACKEND_H
#define BACKEND_H

#include <QDebug>
#include <QString>
#include <stdint.h>
#include <math.h>


#define SHATTER_DEBUG   true

void shatter_debug(QString message);
void shatter_debug(char *message);
void shatter_debug_hex(const char *message, char *data, int len);

bool bitreader(const char *data, int offset);
uint8_t MakeCRC(char *BitString);

QString getStrCommand(QString command);
void runCommand(QString command);

#endif // BACKEND_H
