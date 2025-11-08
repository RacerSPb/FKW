#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main()
{
    try
    {
        ConverterJSON converterJson;
        InvertedIndex invertedIndex;
        do
        {
            // Индексация, процесс подсчёта слов в тексте
            invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());

            // Инициализация поискового сервера
            SearchServer searchServer(invertedIndex);

            // Получаем список запросов из requests.json
            std::vector<std::string> vecRequests = converterJson.GetRequests();
            if (vecRequests.empty())
            {
                std::cerr << "Warning: No requests found in requests.json" << std::endl;
            }
            else
            {
                // Обработка поисковых запросов
                auto results = searchServer.search(vecRequests, converterJson);
                std::cout << "Search completed successfully. Results saved to answers.json" << std::endl;
            }
            /* code */
            std::this_thread::sleep_for(std::chrono::seconds(converterJson.GetDatabaseUpdateTime()));
        } while (converterJson.GetDatabaseUpdateTime() > 0);

        std::cout << "Programm finished. OK!" << std::endl;
        return 0;
    }
    catch (const std::exception &exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }
}