#ifndef FILELOGGER_H
#define FILELOGGER_H
#include <QFile>
#include <QTime>
#include <QDate>
#include <QTextStream>
#include <QApplication>
#include <QErrorMessage>

/**
 * @brief Класс будет работать только с C++17
 *
 */
class FileLogger
{
public:
    static void log(QString message= 0);

protected:
    FileLogger();

private:
    inline static QFile* file = 0;
    inline static bool canLog = false;
};

#endif // FILELOGGER_H
