Сетеая версия чата, построенная по принципу клиент-сервер, с возможностью сохранения данных в базе. Для Windows.

Для работы необходимо сначала запустить сервер, затем одного или нескольких клиентов. Закрывать приложения следует в обратном порядке: сначала клиентов, затем сервер.

Клиенты и сервер управляются командами с консоли. Для вывода справки по командам введите help или ?. После входа пользователя по команде login (l+) становится доступен другой набор команд, также просматриваемый командой help. Тестовые данные пользователей и сообщения чата хранятся в папке _Test_database. Т.к. пароли не хранятся в явном виде (хранится только их хэш), для всех пользователей из тестовой таблицы установлены пароли qwerty и 123.

ПС. Приложения сделаны двухпоточными, однако управление потоками в них не реализовано (поскольку выходит за рамки изученных модулей), что может в некоторых случаях приводить к неадекватному поведению программ.