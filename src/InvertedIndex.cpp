#include "InvertedIndex.h"

void InvertedIndex::CountingWords(size_t doc_id, const std::string &strWord)
{
    if (strWord.empty())
    {
        return;
    }

    std::istringstream stream(strWord);
    std::string oneWord;
    // Словарь для подсчета слов
    std::unordered_map<std::string, size_t> countWord;

    while (stream >> oneWord)
    {
        ++countWord[oneWord];
    }

    std::lock_guard<std::mutex> lock(dict_mutex);

    // Добавляем результат в частотный словарь
    for (auto &&[word, count] : countWord)
    {
        freq_dictionary[word].emplace_back(Entry{doc_id, count});
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    if (input_docs.empty())
    {
        std::cout << "Attention: The list of documents is empty." << std::endl;
        return;
    }

    // Очищаем коллекции
    docs.clear();
    freq_dictionary.clear();

    // Заполняем коллекцию
    docs = std::move(input_docs);

    // Создаем вектор потоков
    std::vector<std::thread> vecThreads;

    // Запускаем поток для каждоой коллекции
    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id)
    {
        vecThreads.emplace_back(&InvertedIndex::CountingWords, this, doc_id, docs[doc_id]);
    }
    // Ждем завершения всех потоков
    for (auto &thread : vecThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    // Защищаем доступ к общему словарю
    std::lock_guard<std::mutex> lock(dict_mutex);

    if (auto it = freq_dictionary.find(word); it != freq_dictionary.end())
    {
        return it->second;
    }
    return {};
}

InvertedIndex::~InvertedIndex()
{
}