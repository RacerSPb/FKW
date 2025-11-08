#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    std::string verEngine{"0.1"}; // В дальнейшем переменная будет передаваться в функцию
    std::vector<std::string> vecTextSearch;

    std::ifstream inFile(pathConfigJSON);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Config file is missing.");
    }
    inFile >> jsonConfig;
    inFile.close();

    if (!jsonConfig.contains("config"))
    {
        throw std::runtime_error("Config file is empty.");
    }

    if (start)
    {
        std::cout << "Starting " << jsonConfig["config"]["name"].get<std::string>()
                  << " ver. " << jsonConfig["config"]["version"].get<std::string>() << std::endl;
        std::cout << "Для выхода их программы наждмите Ctrl+C или установите параметр \"database_update_time\": 0."
                  << std::endl;
        start = false;
    }

    // Заготовка под проверку на соответствие версии движка и конфигурационного файла
    if (jsonConfig["config"]["version"].get<std::string>() != verEngine)
    {
        std::cerr << "config.json has incorrect file version." << std::endl;
    }

    // Обработка поля "files"
    if (!jsonConfig.contains("files"))
    {
        std::cerr << "Error: Field \"files\" is missing." << std::endl;
    }
    else
    {
        // Получаем путь к файлу с текстом
        for (auto &&pathToFile : jsonConfig["files"])
        {
            // Получение данных из текстового файла
            std::ifstream inFile(pathToFile.get<std::string>());
            if (!inFile.is_open())
            {
                std::cerr << "Error: Could not open " << pathToFile.get<std::string>() << std::endl;
                // Добавляем в вектор пустую строку т.к. в противном случае docid будет некорректно указывать на файл
                vecTextSearch.push_back("");
            }
            else
            {
                // Получение данных из текстового файла
                std::string text, line;
                while (getline(inFile, line))
                {
                    text += line;
                    // т.к. файл может быть многострочный, добавляем в качечтве разделителя пробел
                    text.push_back(' ');
                }
                vecTextSearch.push_back(text);
            }
            inFile.close();
        }
    }
    return vecTextSearch;
}

int ConverterJSON::GetResponsesLimit()
{
    int maxResponses = jsonConfig["config"]["max_responses"].get<int>();
    if (maxResponses <= 0)
    {
        // Значение по умолчанию 5
        return 5;
    }
    else
    {
        return maxResponses;
    }
}

int ConverterJSON::GetDatabaseUpdateTime()
{
    return jsonConfig["config"]["database_update_time"].get<int>();
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::vector<std::string> vecRequests;

    std::ifstream inFile(pathRequestsJSON);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Could not open " << pathRequestsJSON << std::endl;
    }
    else
    {
        inFile >> jsonRequests;
    }
    inFile.close();

    if (!jsonRequests.contains("requests"))
    {
        std::cerr << "Error: Field \"requests\" is missing." << std::endl;
    }
    else
    {
        // Получаем значение поля "requests"
        for (auto &&textRequest : jsonRequests["requests"])
        {
            vecRequests.push_back(textRequest.get<std::string>());
        }
    }
    return vecRequests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    std::ofstream outFile;
    outFile.open(pathAnswersJSON, std::ios::trunc);
    if (outFile.is_open())
    {
        nlohmann::json jAnswers;
        int countRequest{1};
        for (auto &&request : answers)
        {
            nlohmann::json jResult, jRelevance;

            std::string requestID = "request" + zeroPadNumber(countRequest);
            jResult = {"result", !request.empty()};
            if (!request.empty())
            {
                nlohmann::json jPairRelevance;
                for (auto &&[docid, rank] : request)
                {
                    nlohmann::json jDocid, jRank;
                    jDocid = {"docid", docid};
                    jRank = {"rank", customRound(rank, 3)};
                    jPairRelevance += {jDocid, jRank};
                }
                jRelevance = {"relevance", jPairRelevance};
                jAnswers["answers"][requestID] = {jResult, jRelevance};
            }
            else
            {
                jAnswers["answers"][requestID] = {jResult};
            }
            ++countRequest;
        }
        outFile << jAnswers.dump(4);
    }
    else
    {
        std::cerr << "Failed to create file answers.json." << std::endl;
    }
    outFile.close();
}

std::string ConverterJSON::zeroPadNumber(int &num)
{
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << num;
    return ss.str();
}

std::string ConverterJSON::customPrecision(float &value, int decimals)
{
    std::ostringstream ss;
    ss << std::setprecision(decimals) << value;
    return ss.str();
}

double ConverterJSON::customRound(float &value, int decimals)
{
    double factor = pow(10, decimals);
    return std::round(value * factor) / factor;
}

ConverterJSON::~ConverterJSON()
{
}
