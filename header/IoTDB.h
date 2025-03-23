//
// Created by Даниил on 04.03.2025.
//

#ifndef APP_SERVER_IOTDB_H
#define APP_SERVER_IOTDB_H

#include "DBManager.h"

class IoTDB : DBManager
{
public:
    IoTDB(); // инциализация полей класса, а также вызов конструктора DBManager и метода open_connetcion
    ~IoTDB(); // деструктор класса который так же вызывает деструктор класс DBManager
    void insert(double in_temp); // запись текущей температуры
    void insert(int in_status);  // текущий статус(определить кодовые значения статусов)
    void reg(std::string in_id); // уникальный номер iot
    bool auth(std::string in_id); // аутентификация iot




};

#endif //APP_SERVER_IOTDB_H
