#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <unordered_map>

struct Entry
{
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator==(const Entry &other) const
    {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};


class InvertedIndex
{
private:
    std::vector<std::string> docs;                             // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    
    mutable std::mutex dict_mutex;
    void CountingWords(size_t, const std::string &);

public:
    InvertedIndex(/* args */) = default;

    /**
     * Обновить или заполнить базу документов, по которой будем совершатьпоиск
     * @param texts_input содержимое документов
     */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
     * Метод определяет количество вхождений слова word в загруженной базе документов
     * @param word слово, частоту вхождений которого необходимо определить
     * @return возвращает подготовленный список с частотой слов
     */
    std::vector<Entry> GetWordCount(const std::string &word);

    ~InvertedIndex();
};
