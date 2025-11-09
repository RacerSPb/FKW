![# Search-in-files](search-in-files.png)<br>
![GitHub author](https://img.shields.io/badge/made_by-RacerSPb-009919)
![GitHub repo size](https://img.shields.io/github/repo-size/RacerSPb/FKW)
![GitHub author](https://img.shields.io/badge/made_on-C++-red)
<br><i> Search for words in files</i> | <i> Поиск слов в файлах.</i>

## Описание | Description
![GitHub author](https://img.shields.io/badge/-RUS-red)<br>
Приложение для поиска запрашиваемых слов (JSON файл requests.json) в текстовых файлах ресурсов (путь к ресурсам указан в config.json). Результат поиска помещается в answers.json.<br><br>
![GitHub author](https://img.shields.io/badge/-ENG-blue)<br>
Application for searching of request words (JSON file requests.json) in resources text files (path to resources files are specified in config.json). Search result is putting into answers.json.
## Technologies
![GitHub author](https://img.shields.io/badge/C++-15.2.0-005199?style=for-the-badge) 
![GitHub author](https://img.shields.io/badge/CMake-4.1.2-005199?style=for-the-badge)
![GitHub author](https://img.shields.io/badge/JSON-3.12.0-orange?style=for-the-badge)
![GitHub author](https://img.shields.io/badge/GTest-1.17.0-green?style=for-the-badge)<br>


## Спецификация файлов
* config.json<br>
Файл, в котором указаны название и версия приложения<br>
Здесь Вы также можете изменить максимальное количество релевантных ответов, которые будут помещены в answers.json (max_respones), а также время в секундах через которое движок повторит поиск (database_update_time).<br>
Содержание по умолчанию:<br>
```json
{
    "config": {
        "name": "SkillboxSearchEngine",
        "version": "0.1",
        "max_responses": 5,
        "database_update_time": 0
    },
    "files": [
        "../resources/file001.txt",
        "../resources/file002.txt",
        "../resources/file003.txt",
        "../resources/file004.txt"
    ]
}
```
* requests.json<br>
Файл в котором указаны запросы на поиск.<br>
Каждый отдельный запрос следует вводить после "," в новой строке.<br>
Пример содержимого:<br>
```json
{
    "requests": [
        "milk water",
        "sugar",
		"cappuccino"
    ]
}
```
* answers.json<br>
Файл в котором будет записан результат работы поискового движка в формате JSON<br>
Пример содержимого:<br>
```json
{
    "answers": {
        "request001": {
            "relevance": [
                {
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 0,
                    "rank": 0.7
                },
                {
                    "docid": 1,
                    "rank": 0.3
                }
            ],
            "result": true
        },
        "request002": {
            "result": false
        },
        "request003": {
            "relevance": [
                {
                    "docid": 3,
                    "rank": 1.0
                }
            ],
            "result": true
        }
    }
}
```
<i>NOTE:<br>
docid - идентификатор релевантного документа,<br>
rank - относительная релевантность,<br>
result - true (если обнаружена релевантность) или false (если отсутствуют совпадения).</i>

[Техническое задание на разработку](Финальная_работа_курса_Разработчик_на_C_с_нуля.pdf)