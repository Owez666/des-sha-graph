#include "mytcpserver.h"
#include <QCoreApplication>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include "databasemanager.h"
// Подключаем наш Singleton для работы с БД (SQLite).

// ------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ -------------------
static QByteArray ok(const QString &s){ return ("OK: " + s + "\r\n").toUtf8(); }
static QByteArray err(const QString &s){ return ("ERR: " + s + "\r\n").toUtf8(); }

// ------------------- ДЕСТРУКТОР -------------------
MyTcpServer::~MyTcpServer()
{
    if (mTcpServer) mTcpServer->close(); // закрываем сервер при завершении
}

// ------------------- КОНСТРУКТОР -------------------
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    // 1. Инициализируем базу данных (через Singleton).
    DatabaseManager::getInstance();

    // 2. Создаём объект TCP-сервера.
    mTcpServer = new QTcpServer(this);

    // 3. Если приходит новый клиент → вызываем slotNewConnection.
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    // 4. Запускаем сервер на порту 33333.
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

// ------------------- НОВОЕ ПОДКЛЮЧЕНИЕ -------------------
void MyTcpServer::slotNewConnection()
{
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();
    clients.append(clientSocket);

    clientSocket->write("OK: server ready\r\n");

    connect(clientSocket, &QTcpSocket::readyRead, this, [=](){
        this->slotServerRead(clientSocket);
    });
    connect(clientSocket, &QTcpSocket::disconnected, this, [=](){
        this->slotClientDisconnected(clientSocket);
    });
}

// ------------------- ЧТЕНИЕ ДАННЫХ ОТ КЛИЕНТА -------------------
void MyTcpServer::slotServerRead(QTcpSocket* socket)
{
    QByteArray raw = socket->readAll();
    QString buf = QString::fromUtf8(raw);

    buf.replace('\r', '\n');
    const QStringList lines = buf.split('\n', Qt::SkipEmptyParts);

    for (const QString &lineRaw : lines) {
        QString line = lineRaw.trimmed();
        if (line.isEmpty()) continue;

        qDebug() << "cmd:" << line;

        const int sp = line.indexOf(' ');
        const QString cmd = (sp < 0 ? line : line.left(sp)).toLower();
        const QString tail = (sp < 0 ? "" : line.mid(sp+1));

        // ------------------- ОБРАБОТКА КОМАНД -------------------

        if (cmd == "ping") { socket->write(ok("pong")); continue; }

        if (cmd == "help") {
            socket->write(ok(
                "commands: ping | help | "
                "register <login>:<password> | login <login>:<password> | "
                "triple_des <text>|<key> | sha512 <text> | "
                "bisection <left>:<right>:<eps> | shortest <u>:<v>"
                ));
            continue;
        }

        // ----------- REGISTER (теперь реально в БД) -----------
        if (cmd == "register") {
            if (!tail.contains(':')) { socket->write(err("usage: register login:password")); continue; }
            QStringList p = tail.split(':');
            if (p.size()!=2){ socket->write(err("usage: register login:password")); continue; }
            QString login = p[0].trimmed();
            QString pass  = p[1].trimmed();

            QSqlDatabase db = DatabaseManager::getInstance()->getDatabase();
            QSqlQuery q(db);
            q.prepare("INSERT INTO users(login,password) VALUES(:l,:p)");
            q.bindValue(":l", login);
            q.bindValue(":p", pass);

            if (q.exec())
                socket->write(ok("user registered"));
            else
                socket->write(err("регистрация не выполнена (логин занят?)"));
            continue;
        }

        // ----------- LOGIN (теперь реально в БД) -----------
        if (cmd == "login") {
            if (!tail.contains(':')) { socket->write(err("usage: login login:password")); continue; }
            QStringList p = tail.split(':');
            if (p.size()!=2){ socket->write(err("usage: login login:password")); continue; }
            QString login = p[0].trimmed();
            QString pass  = p[1].trimmed();

            QSqlDatabase db = DatabaseManager::getInstance()->getDatabase();
            QSqlQuery q(db);
            q.prepare("SELECT id FROM users WHERE login=:l AND password=:p");
            q.bindValue(":l", login);
            q.bindValue(":p", pass);

            if (q.exec() && q.next())
                socket->write(ok("login ok"));
            else
                socket->write(err("неверный логин/пароль"));
            continue;
        }

        // ----------- ОСТАЛЬНЫЕ КОМАНДЫ ПОКА ЗАГЛУШКИ -----------
        if (cmd == "triple_des") {
            if (!tail.contains('|')) { socket->write(err("usage: triple_des text|key")); continue; }
            socket->write(ok("triple_des stub"));
            continue;
        }

        if (cmd == "sha512") {
            if (tail.isEmpty()) { socket->write(err("usage: sha512 text")); continue; }
            socket->write(ok("sha512 stub"));
            continue;
        }

        if (cmd == "bisection") {
            if (tail.count(':') != 2) { socket->write(err("usage: bisection left:right:eps")); continue; }
            socket->write(ok("bisection stub"));
            continue;
        }

        if (cmd == "shortest") {
            if (!tail.contains(':')) { socket->write(err("usage: shortest u:v")); continue; }
            socket->write(ok("shortest stub"));
            continue;
        }

        socket->write(err("unknown command"));
    }
}

// ------------------- КЛИЕНТ ОТКЛЮЧИЛСЯ -------------------
void MyTcpServer::slotClientDisconnected(QTcpSocket* socket)
{
    clients.removeAll(socket);
    socket->deleteLater();
}
