/*
Заполнить 30000 векторов, содержащих по 100 целых чисел в диапазоне [-1000,
1000]. В отдельных потоках исполнения найти сумму элементов для каждого вектора.
Использовать 1, 2 и 4 потока. Найти время исполнения программы в каждом случае.
*/

#include <iostream> 
#include <vector> 
#include <thread> 
#include <chrono> 
#include <random> 

// Функция для генерации случайных чисел в заданном диапазоне
int generateRandomNumber(int min, int max) {
    static std::random_device rd; // Создаем генератор случайных чисел
    static std::mt19937 gen(rd()); // Создаем генератор случайных чисел с использованием алгоритма Mersenne Twister
    std::uniform_int_distribution<int> dis(min, max); // Создаем распределение для целых чисел в заданном диапазоне
    return dis(gen); // Возвращаем случайное число из заданного диапазона
}

// Функция для заполнения вектора случайными числами
void fillVector(std::vector<int>& vec) {
    for (int i = 0; i < 100; ++i) { // Цикл по 100 итераций для заполнения вектора
        vec.push_back(generateRandomNumber(-1000, 1000)); // Добавляем в вектор случайное число из диапазона [-1000, 1000]
    }
}

// Функция для нахождения суммы элементов вектора
long long sumVector(const std::vector<int>& vec) {
    long long sum = 0; // Инициализируем переменную для хранения суммы элементов
    for (int num : vec) { // Цикл по всем элементам вектора
        sum += num; // Добавляем текущий элемент к сумме
    }
    return sum; // Возвращаем сумму элементов вектора
}

int main() {
    setlocale(LC_ALL, "Russian"); // Устанавливаем русскую локаль для вывода сообщений на русском языке
    const int numVectors = 30000; // Количество векторов
    const int numThreads[] = { 1, 2, 4 }; // Массив с количеством потоков исполнения

    for (int numThread : numThreads) { // Перебираем количество потоков
        auto start = std::chrono::steady_clock::now(); // Запоминаем текущее время начала работы программы

        // Создаем вектор векторов для хранения данных
        std::vector<std::vector<int>> vectors(numVectors);

        // Заполняем векторы случайными числами в отдельных потоках
        std::vector<std::thread> threads; // Вектор для хранения потоков
        for (int i = 0; i < numVectors; ++i) { // Цикл по всем векторам
            threads.emplace_back([&vectors, i]() { fillVector(vectors[i]); }); // Создаем поток для заполнения вектора случайными числами
        }
        for (auto& thread : threads) { // Цикл ожидания завершения всех потоков
            thread.join(); // Ожидаем завершения работы потока
        }

        // Находим сумму элементов векторов в отдельных потоках
        std::vector<long long> sums; // Переменная для хранения общей суммы всех элементов векторов
        threads.clear(); // Очищаем вектор потоков для последующего использования
        for (int i = 0; i < numThread; i += 1) { // Цикл по векторам с учетом количества потоков
            sums.push_back(0);
            threads.emplace_back([&vectors, &sums, i, numThread]() { // Создаем поток для вычисления суммы элементов векторов
                for (int j = i; j < numVectors; j += numThread) { // Цикл по векторам для текущего потока
                    sums[i] += sumVector(vectors[j]); // Вычисляем сумму элементов вектора и добавляем к общей сумме
                    //std::cout << sums[i] << std::endl;
                }
                });
            
        }
            
        for (auto& thread : threads) { // Цикл ожидания завершения всех потоков
            thread.join(); // Ожидаем завершения работы потока
        }

        long long totalSum = 0;
        for (long long sum : sums) {
            totalSum += sum;
        }
        std::cout << totalSum << std::endl;

        auto end = std::chrono::steady_clock::now(); // Запоминаем текущее время окончания работы программы
        std::cout << "Время выполнения с " << numThread << " потоком(ами): " // Выводим информацию о времени выполнения с указанным количеством потоков
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() // Вычисляем время выполнения программы в миллисекундах
            << " мс" << std::endl; // Выводим время выполнения программы в миллисекундах
    }

    return 0; // Возвращаем 0, чтобы указать успешное завершение программы
}
