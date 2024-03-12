#include <iostream>
#include <vector> 
#include <chrono> 
#include <random> 
#include <omp.h> // Подключение заголовочного файла OpenMP

// Функция для генерации случайных чисел в заданном диапазоне
int generateRandomNumber(int min, int max) {
    static std::random_device rd; // Создание статического объекта для генерации случайных чисел
    static std::mt19937 gen(rd()); // Инициализация генератора случайных чисел с использованием seed
    std::uniform_int_distribution<int> dis(min, max); // Создание равномерного распределения в заданном диапазоне
    return dis(gen); // Возвращение случайного числа
}

// Функция для заполнения вектора случайными числами
void fillVector(std::vector<int>& vec) {
    for (int i = 0; i < 100; ++i) { // Цикл для заполнения вектора 100 случайными числами
        vec.push_back(generateRandomNumber(-1000, 1000)); // Добавление случайного числа в вектор
    }
}

// Функция для нахождения суммы элементов вектора
long long sumVector(const std::vector<int>& vec) {
    long long sum = 0; // Переменная для хранения суммы элементов
    for (int num : vec) { // Цикл для перебора элементов вектора
        sum += num; // Прибавление текущего элемента к сумме
    }
    return sum; // Возвращение суммы
}

int main() {
    setlocale(LC_ALL, "Russian"); // Установка локали для русского языка

    const int numVectors = 30000; // Количество векторов
    const int numThreads[] = { 1, 2, 4 }; // Массив с количеством потоков

    // Цикл для тестирования разного количества потоков
    for (int numThread : numThreads) {
        auto start = std::chrono::steady_clock::now(); // Засечка времени начала выполнения

        // Создание вектора векторов для хранения данных
        std::vector<std::vector<int>> vectors(numVectors);

        // Заполнение векторов случайными числами в параллельных потоках
#pragma omp parallel for num_threads(numThread)
        /*Эта директива используется для создания параллельной области,
        в которой цикл выполняется параллельно в нескольких потоках.
        Аргумент num_threads(numThread) указывает OpenMP на количество потоков, которые должны быть созданы для выполнения цикла.
        Это число передается переменной numThread, которая определяется в цикле for.
        Каждый поток выполняет часть итераций цикла, деление на части осуществляется автоматически OpenMP.*/
        for (int i = 0; i < numVectors; ++i) {
            fillVector(vectors[i]);
        }

        // Нахождение суммы элементов векторов в параллельных потоках
        long long totalSum = 0; // Переменная для хранения общей суммы
#pragma omp parallel for num_threads(numThread) reduction(+:totalSum)
        for (int i = 0; i < numVectors; ++i) {
            totalSum += sumVector(vectors[i]);
        }

        auto end = std::chrono::steady_clock::now(); // Засечка времени окончания выполнения
        std::cout << "Время выполнения с " << numThread << " потоком(ами): "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " мс" << std::endl; // Вывод времени выполнения
    }

    return 0; // Возврат нуля в качестве признака успешного завершения программы
}
//При использовании разного кол-ва потоков заметной разницы во времени нет.
//Думаю, это обуславливается слишком частой синхронизацией и крайне малыми вычислениями для многопоточности.
//В реальных задачах многопоточность используют для куда более длительных вычислений и все выделенные потоки для данных вычислений являются фоновыми.

