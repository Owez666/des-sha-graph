# Ветка server-db

Сервер на Qt с использованием SQLite.  
Поддерживает команды `register` и `login`, данные сохраняются в базе.  

---

## Как запустить?
1. Открыть проект в **Qt Creator** (`echoServer.pro`).
2. Собрать и запустить проект.
3. В консоли Qt Creator должно появиться сообщение **"server is started", "база данных успешно открыта"**:

<img width="600" height="400" alt="image" src="https://github.com/user-attachments/assets/22613aba-4e1f-41b4-ab17-a1a371d93d4c" />

### 1. В папке сборки появился файл базы
После запуска создаётся файл `project_data.db` — это сама база данных SQLite.

<img width="600" height="500" alt="image" src="https://github.com/user-attachments/assets/a6d007b2-2fd4-4dd1-ae46-d2f333534416" />

### 2. Структура базы данных
Внутри автоматически создаётся таблица `users` с полями:
- `id` (INTEGER PRIMARY KEY AUTOINCREMENT)  
- `login` (TEXT UNIQUE)  
- `password` (TEXT)

<img width="500" height="200" alt="image" src="https://github.com/user-attachments/assets/53359d0c-8d80-443d-8cfd-849472d21e79" />


### 3. Регистрация и успешный вход (Ошибка при неверном логине/пароле - если ввести пользователя, которого нет в базе)

<img width="600" height="500" alt="image" src="https://github.com/user-attachments/assets/c6abda85-306d-48e4-9d72-cdce319f99bf" />

### 4. Проверка содержимого базы
Открыв `project_data.db`, видно, что пользователь реально сохранился в таблице.

<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/7ef6bfae-b50c-4410-af55-cd2b8ed654cf" />

##  Итог
- Сервер запускается на порту **33333**.  
- Реализована регистрация и авторизация через SQLite.  
- Данные пользователей хранятся в файле `project_data.db`.  
- Ошибки при неверном вводе корректно обрабатываются. 
