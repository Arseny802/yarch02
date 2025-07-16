## Изучите [README.md](README.md) файл и структуру проекта.

## Задание 1


[Контейнерная диаграмма в нотации С4](docs/c4/containers.puml)

## Задание 2

### 1. Proxy


Мной был реализован микросервис на C++ в [src/microservices/proxy](src/microservices/proxy/)
Изменил значение переменной "MOVIES_MIGRATION_PERCENT" на 75 и написал тесты, проверяющие переход.

### 2. Kafka


Мной был реализован микросервис на C++ в [src/microservices/events](src/microservices/events/)

[Скриншот пройденных тестов](screenshots/passed_postman_tests.png)
[Скриншот состояния топиков Kafka](screenshots/kafka_topics_status.png)

## Задание 3


Добился "зеленой" сборки и "зеленых" тестов в Github Actions.

[Cкриншот вывода event-service](screenshots/events-service.jpg)
[Cкриншот вывода тестов test:kubernetes](screenshots/test_kubernetes.jpg)

## Задание 4


[Скриншот вывода https://cinemaabyss.example.com/api/movies](screenshots/movies-service.jpg)

## Задание 5


[Скриншот работы circuit breaker'а](screenshots/circuit-breaker.jpg)
