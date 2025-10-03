#include "mytcpserver.h"
#include <QCoreApplication>
#include <QStringList>

// Здесь мы пишем реализацию функций из заголовка mytcpserver.h
/*
 * Какие команды сервер понимает (все пока только заглушки):
 *
 *   register <login>:<password> - регистрация (будет работать позже)
 *   login <login>:<password> - вход (будет работать позже)
 *   triple_des <text>|<key> - шифрование 3DES (будет работать позже)
 *   sha512 <text> - хеш SHA-512 (будет работать позже)
 *   bisection <left>:<right>:<eps> - метод деления пополам (бисекции)
 *   shortest <u>:<v> - кратчайший путь в графе
 *   help - список всех команд
 *   ping - проверка связи
 *
 * Ввод в PuTTY: просто набираем команду и жмём Enter.
 * Сервер отвечает строкой (заканчивается символами \r\n).
 */

// ------------------- ДЕСТРУКТОР -------------------
MyTcpServer::~MyTcpServer()
{
    // Когда объект сервера удаляется — закрываем порт
    if (mTcpServer) mTcpServer->close();
}

// ------------------- КОНСТРУКТОР -------------------
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this); // создаём объект TCP-сервера

    // Если приходит новый клиент → вызываем slotNewConnection
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    // Запускаем сервер на порту 33333
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

// ------------------- НОВОЕ ПОДКЛЮЧЕНИЕ -------------------
void MyTcpServer::slotNewConnection()
{
    // Берём сокет нового клиента
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();
    clients.append(clientSocket); // добавляем в список клиентов

    // Отправляем приветственное сообщение
    clientSocket->write("OK: server ready\r\n");

    // Подключаем сигналы к нашим слотам через лямбды
    connect(clientSocket, &QTcpSocket::readyRead, this, [=](){
        this->slotServerRead(clientSocket);
    });
    connect(clientSocket, &QTcpSocket::disconnected, this, [=](){
        this->slotClientDisconnected(clientSocket);
    });
}

// ------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ -------------------
static QByteArray ok(const QString &s){ return ("OK: " + s + "\r\n").toUtf8(); }
static QByteArray err(const QString &s){ return ("ERR: " + s + "\r\n").toUtf8(); }

// ------------------- ЧТЕНИЕ ДАННЫХ ОТ КЛИЕНТА -------------------
void MyTcpServer::slotServerRead(QTcpSocket* socket)
{
    // Считываем всё, что клиент прислал
    QByteArray raw = socket->readAll();
    QString buf = QString::fromUtf8(raw);

    // -----------------------
    // Убираем разные переводы строк:
    //   Windows \r\n
    //   Linux \n
    //   MacOS \r
    //
    // Делаем всё одинаковым: заменяем \r на \n.
    buf.replace('\r', '\n');

    // Теперь режем по \n (каждая строка = одна команда).
    // Qt::SkipEmptyParts убирает пустые строки.
    const QStringList lines = buf.split('\n', Qt::SkipEmptyParts);

    // Перебираем все команды по одной
    for (const QString &lineRaw : lines) {
        QString line = lineRaw.trimmed(); // убираем пробелы по краям
        if (line.isEmpty()) continue;

        qDebug() << "cmd:" << line; // выводим команду в консоль для отладки

        // Разделяем на "команду" и "аргументы"
        const int sp = line.indexOf(' ');
        const QString cmd = (sp < 0 ? line : line.left(sp)).toLower(); // команда
        const QString tail = (sp < 0 ? "" : line.mid(sp+1)); // аргументы

        // ------------------- ОБРАБОТКА КОМАНД -------------------

        if (cmd == "ping") {
            socket->write(ok("pong"));
            continue;
        }

        if (cmd == "help") {
            socket->write(ok(
                "commands: ping | help | "
                "register <login>:<password> | login <login>:<password> | "
                "triple_des <text>|<key> | sha512 <text> | "
                "bisection <left>:<right>:<eps> | shortest <u>:<v>"
                ));
            continue;
        }

        if (cmd == "register") {
            // ждём формат login:password
            if (!tail.contains(':')) { socket->write(err("usage: register login:password")); continue; }
            const QStringList p = tail.split(':');
            if (p.size()!=2){ socket->write(err("usage: register login:password")); continue; }
            const QString login = p[0].trimmed();
            Q_UNUSED(login); // пока не используем
            // позже будет запись в БД
            socket->write(ok("register stub (saved later)"));
            continue;
        }

        if (cmd == "login") {
            // ждём формат login:password
            if (!tail.contains(':')) { socket->write(err("usage: login login:password")); continue; }
            const QStringList p = tail.split(':');
            if (p.size()!=2){ socket->write(err("usage: login login:password")); continue; }
            const QString login = p[0].trimmed();
            Q_UNUSED(login);
            // позже будет проверка в БД
            socket->write(ok("login stub (always ok for now)"));
            continue;
        }

        if (cmd == "triple_des") {
            if (!tail.contains('|')) { socket->write(err("usage: triple_des text|key")); continue; }
            // здесь будет шифрование 3DES
            socket->write(ok("triple_des stub"));
            continue;
        }

        if (cmd == "sha512") {
            if (tail.isEmpty()) { socket->write(err("usage: sha512 text")); continue; }
            // здесь будет SHA-512
            socket->write(ok("sha512 stub"));
            continue;
        }

        if (cmd == "bisection") {
            if (tail.count(':') != 2) { socket->write(err("usage: bisection left:right:eps")); continue; }
            // здесь будет метод деления пополам
            socket->write(ok("bisection stub"));
            continue;
        }

        if (cmd == "shortest") {
            if (!tail.contains(':')) { socket->write(err("usage: shortest u:v")); continue; }
            // здесь будет кратчайший путь в графе
            socket->write(ok("shortest stub"));
            continue;
        }

        // Если команда неизвестна
        socket->write(err("unknown command"));
    }
}

// ------------------- КЛИЕНТ ОТКЛЮЧИЛСЯ -------------------
void MyTcpServer::slotClientDisconnected(QTcpSocket* socket)
{
    clients.removeAll(socket); // убираем клиента из списка
    socket->deleteLater();     // освобождаем память
}
