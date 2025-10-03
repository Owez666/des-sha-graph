# Ветка echo-server

В этой ветке будет простой echo-сервер, который возвращает клиенту полученные от него данные.  
Используется для проверки взаимодействия между сервером и клиентами.

Реализован простой TCP-сервер (Qt, C++), который:
- запускается на порту **33333**;
- принимает команды от клиента (PuTTY);
- парсит команды (`ping`, `help`, `register`, `login`, …);
- пока отвечает **заглушками** (логика будет сделана в следующих заданиях).

---

## Запуск проекта
1. Открыть `echoServer.pro` в Qt Creator.  
2. Собрать и запустить (зелёная кнопка ▶).  
3. В консоль Qt должно появиться:  
<img width="600" height="500" alt="image" src="https://github.com/user-attachments/assets/f0792b65-36c1-4720-92c6-0ecb015f4ede" />

---

## Подключение через PuTTY
1. Запустить **PuTTY**.  
2. Указать:
   - Host Name: `127.0.0.1`  
   - Port: `33333`  
   - Connection type: `Raw`  

<img width="600" height="600" alt="image" src="https://github.com/user-attachments/assets/6d187878-994d-4465-a156-2ec0d12c86e3" />


---

## Примеры работы
После подключения сервер присылает приветственное сообщение:  

<img width="564" height="233" alt="image" src="https://github.com/user-attachments/assets/9fadcd05-b3d6-4650-b7ac-acf397853e13" />

Команда `ping`:

<img width="405" height="262" alt="image" src="https://github.com/user-attachments/assets/727b259d-5f31-45c1-9731-6926addf9bf7" />

Команды `help` и `register` (с заглушкой):

<img width="2473" height="754" alt="image" src="https://github.com/user-attachments/assets/d7215835-c72c-4f53-a840-204215f9aea5" />

## Итог
Сервер запускается, принимает клиентов, обрабатывает команды и отвечает заглушками.
