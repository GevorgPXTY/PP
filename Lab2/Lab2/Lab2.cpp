#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
// Функция для генерации случайных чисел в заданном диапазоне
int generateRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

// Функция для заполнения вектора случайными числами
void fillVector(std::vector<int>& vec) {
    for (int i = 0; i < 100; ++i) {
        vec.push_back(generateRandomNumber(-1000, 1000));
    }
}

// Функция для нахождения суммы элементов вектора
long long sumVector(const std::vector<int>& vec) {
    long long sum = 0;
    for (int num : vec) {
        sum += num;
    }
    return sum;
}

int main() {
    setlocale(LC_ALL, "Russian");
    const int numVectors = 30000;
    const int numThreads[] = { 1, 2, 4 };

    for (int numThread : numThreads) {
        auto start = std::chrono::steady_clock::now();

        // Создаем вектор векторов для хранения данных
        std::vector<std::vector<int>> vectors(numVectors);

        // Заполняем векторы случайными числами в отдельных потоках
        std::vector<std::thread> threads;
        for (int i = 0; i < numVectors; ++i) {
            threads.emplace_back([&vectors, i]() { fillVector(vectors[i]); });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        // Находим сумму элементов векторов в отдельных потоках
        long long totalSum = 0;
        threads.clear();
        for (int i = 0; i < numVectors; i += numThread) {
            threads.emplace_back([&vectors, &totalSum, i, numThread]() {
                for (int j = i; j < i + numThread && j < numVectors; ++j) {
                    totalSum += sumVector(vectors[j]);
                }
                });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        auto end = std::chrono::steady_clock::now();
        std::cout << "Время выполнения с " << numThread << " потоком(ами): "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " мс" << std::endl;
    }

    return 0;
}
