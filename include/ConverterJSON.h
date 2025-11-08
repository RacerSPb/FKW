#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

/**
 * Класс для работы с JSON-файлами
 */

class ConverterJSON
{
private:
    std::string pathConfigJSON{"config.json"};
    std::string pathRequestsJSON{"requests.json"};
    std::string pathAnswersJSON{"answers.json"};
    nlohmann::json jsonConfig, jsonRequests;
    bool start{true};
    std::string zeroPadNumber(int &);
    std::string customPrecision(float &, int);
    double customRound(float &, int);
    /* data */
public:
    ConverterJSON(/* args */) = default;
    ~ConverterJSON();

    /**
     * Метод получения содержимого файлов
     * @return Возвращает список с содержимым файлов перечисленных
     * в config.json
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * Метод считывает поле max_responses для определения предельного
     * количества ответов на один запрос
     * @return Возвращает количество ответов на один запрос
     */
    int GetResponsesLimit();

    /**
     * Метод считывает поле database_update_time для определения периодичности
     * переиндексации базы
     * @return Возвращает время ожидания в секундах
     */
    int GetDatabaseUpdateTime();

    /**
     * Метод получения запросов из файла requests.json
     * @return возвращает список запросов из файла requests.json
     */
    std::vector<std::string> GetRequests();

    /**
     * Положить в файл answers.json результаты поисковых запросов
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>);
};
