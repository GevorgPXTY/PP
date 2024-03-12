// Lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <mpi.h>
#include <random>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    // Генератор случайных чисел для гонораров
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(10000, 100000);

    // Писатель отправляет книги в издательства
    int first_publisher = (rank % 2) + 1; // Издательство 1 или 2
    int second_publisher = (first_publisher == 1) ? 2 : 1; // Издательство, отличное от первого

    if (rank == 0) {
        std::cout << "Писатель отправляет книги в издательства...\n";
    }

    // Писатель отправляет книги в издательства (первая и третья книги в первое, вторая во второе)
    int book1 = 1, book2 = 2, book3 = 3;
    if (rank == 0 || rank == 2) {
        MPI_Send(&book1, 1, MPI_INT, first_publisher, 0, MPI_COMM_WORLD);
        MPI_Send(&book3, 1, MPI_INT, first_publisher, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Send(&book2, 1, MPI_INT, second_publisher, 0, MPI_COMM_WORLD);
    }

    // Писатель ожидает гонорары от издательств
    int received_publisher, received_book, received_payment;
    MPI_Recv(&received_publisher, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&received_book, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&received_payment, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Писатель выводит полученные гонорары
    std::cout << "Я получил гонорар " << received_payment << " руб. от издательства " << received_publisher
        << " за книгу " << received_book << std::endl;

    MPI_Finalize();
    return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
