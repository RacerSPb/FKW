#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input, ConverterJSON &converterJson)
{
    std::vector<std::vector<RelativeIndex>> vecResult;
    for (auto &&query : queries_input)
    {
        // Разбиваем поисковый запрос на отдельные слова
        std::vector<std::string> vecWords;
        std::string oneWord;
        std::istringstream stream(query);

        while (stream >> oneWord)
        {
            vecWords.push_back(oneWord);
        }

        // Формируем список уникальных слов
        std::sort(vecWords.begin(), vecWords.end());
        vecWords.resize(std::distance(vecWords.begin(), std::unique(vecWords.begin(), vecWords.end())));

        // Считаем Rabs для каждого документа
        std::unordered_map<size_t, int> mapRabs;

        for (auto &&word : vecWords)
        {
            auto entriesWord = _index.GetWordCount(word);

            // Для каждого документа, содержащего это слово, добавляем в релевантность
            for (auto &&entry : entriesWord)
            {
                mapRabs[entry.doc_id] += entry.count;
            }
        }
        
        // Если нет релевантных документов, добавляем пустой результат
        if (mapRabs.empty())
        {
            vecResult.push_back({});
            continue;
        }

        // Находим Rabs
        int maxRabs{0};
        for (auto &&[doc_id, Rabs] : mapRabs)
        {
            if (Rabs > maxRabs)
            {
                maxRabs = Rabs;
            }
        }

        // Считаем Rrel для каждого документа
        std::vector<RelativeIndex> vecRrel;
        for (auto &&[doc_id, Rabs] : mapRabs)
        {
            RelativeIndex relIndex;
            relIndex.doc_id = doc_id;
            relIndex.rank = Rabs / (float)maxRabs;
            vecRrel.push_back(relIndex);
        }

        // Сортируем документы по убыванию релевантности: от большей к меньшей
        std::sort(vecRrel.begin(), vecRrel.end(),
                  [](const RelativeIndex &a,
                     const RelativeIndex &b)
                  {
                      return a.rank > b.rank;
                  });

        if (vecRrel.size() > converterJson.GetResponsesLimit())
        {
            vecRrel.resize(converterJson.GetResponsesLimit());
        }

        vecResult.push_back(vecRrel);
    }
    // Записываем результат работы в файл answers.json в соответствии с форматом
    std::vector<std::vector<std::pair<int, float>>> answers;
    for (auto &&requestID : vecResult)
    {
        // Преремещаем из структуры в словарь для соответсвия формату в ConverterJSON
        std::vector<std::pair<int, float>> vecRelativeIndex;
        for (auto &&relativeIndex : requestID)
        {
            vecRelativeIndex.emplace_back(relativeIndex.doc_id, relativeIndex.rank);
        }
        answers.push_back(vecRelativeIndex);
    }
    converterJson.putAnswers(answers);

    return vecResult;
}