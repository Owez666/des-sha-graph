#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

// Singleton для работы с SQLite
class DatabaseManager {
public:
    static DatabaseManager* getInstance(); // получить объект
    QSqlDatabase getDatabase(); // вернуть подключение

private:
    static DatabaseManager* instance;
    QSqlDatabase db;

    DatabaseManager(); // приватный конструктор
};

#endif // DATABASEMANAGER_H
