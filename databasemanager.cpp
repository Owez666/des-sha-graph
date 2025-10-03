#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("project_data.db"); // файл создастся в папке проекта

    if (!db.open()) {
        qDebug() << "Не удалось открыть БД:" << db.lastError().text();
        return;
    }
    qDebug() << "База данных успешно открыта.";

    // Создаём таблицу users, если её ещё нет
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "login TEXT UNIQUE, "
               "password TEXT)");
}

DatabaseManager* DatabaseManager::getInstance() {
    if (!instance)
        instance = new DatabaseManager();
    return instance;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
