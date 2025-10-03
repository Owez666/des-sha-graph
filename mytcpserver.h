#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
// Эта конструкция нужна, чтобы файл случайно не подключился дважды.
// Если вдруг мы подключим этот .h несколько раз в разных местах,
// компилятор не будет ругаться на "повторное объявление".

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QDebug>
// Подключаем нужные библиотеки Qt:
// QObject - базовый класс для всех объектов Qt
// QTcpServer - умеет слушать порт и принимать подключения клиентов
// QTcpSocket - сам клиентский сокет (канал общения с одним клиентом)
// QList - список (как массив, но удобнее)
// QByteArray - удобный контейнер для байтов/текста
// QDebug - для вывода отладочной информации в консоль

/*
 * Пояснение:
 * У нас будет сервер, который может принимать сразу несколько клиентов.
 * Мы будем хранить всех подключившихся клиентов в списке (QList<QTcpSocket*>).
 * Когда клиент подключается, вызывается slotNewConnection().
 * Когда клиент что-то прислал, вызывается slotServerRead(...).
 * Когда клиент отключился, вызывается slotClientDisconnected(...).
 * Подключили SQLite (через Singleton DatabaseManager)
 * и теперь команды register/login реально работают с базой данных.
 */

class MyTcpServer : public QObject
{
    Q_OBJECT // Макрос Qt. Без него сигналы/слоты работать не будут.
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    // Конструктор — вызывается при создании объекта сервера
    ~MyTcpServer();
    // Деструктор — вызывается при удалении объекта сервера

public slots:
    // "слоты" — специальные функции, которые Qt вызывает автоматически
    void slotNewConnection();
    // вызывается, когда пришёл новый клиент
    void slotClientDisconnected(QTcpSocket* socket);
    // вызывается, когда клиент отключился
    void slotServerRead(QTcpSocket* socket);
    // вызывается, когда клиент прислал данные

private:
    QTcpServer *mTcpServer = nullptr;
    // объект-сервер (слушает порт и принимает клиентов)

    QList<QTcpSocket*> clients;
    // список всех клиентов, которые подключились
};

#endif // MYTCPSERVER_H
