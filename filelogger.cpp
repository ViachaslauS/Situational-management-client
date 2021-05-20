#include "filelogger.h"

FileLogger::FileLogger()
{
    file = new QFile("log.txt");
    if(file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        FileLogger::canLog = true;

    }
    else
        qDebug("Cant logging actions");
}

void FileLogger::log(QString message)
{
    if(file == 0)
    {
        FileLogger();
    }

    if(canLog)
    {
        QTime time;
        QDate date;
        QTextStream stream(file);
        stream << '['+ date.currentDate().toString("dd.MM.yyyy") + ' ' + time.currentTime().toString() +"]: " + message + '\n';
        qDebug() << message;
    }
}
