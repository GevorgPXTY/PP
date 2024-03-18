/* 5. Об издательствах.Писатель работает с двумя издательствами.Он
написал 3 книги.Первую отослал(сообщение MPI) в 1е издательство, вторую – во
второе, третью – снова в первое.Теперь писатель ожидает гонорара, который может
поступить от издательств в любом порядке.Подождав некоторое время, издательства
высылают(сообщение MPI) гонорары, которые являются случайными числами
от 10000 р до 100000. Получив гонорар, писатель пишет в стандартный поток
вывода : «я получил гонорар(сумма), от издательства(номер) за книгу(номер)».
*/
#include <iostream> // Подключение библиотеки для ввода/вывода
#include <mpi.h> // Подключение библиотеки MPI для параллельных вычислений
#include <random> // Подключение библиотеки для генерации случайных чисел

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Инициализация MPI

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получение ранга текущего процесса
    int num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes); // Получение количества процессов

    // Генератор случайных чисел для генерации гонораров
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(10000, 100000);

    if (rank == 0) { // Если ранг равен 0, то это писатель
        // Писатель отправляет книги в издательства
        int book1 = 1, book2 = 2, book3 = 3;
        MPI_Send(&book1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&book2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&book3, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Вывод информации о отправленных книгах
        std::cout << "Писатель отправил первую книгу в издательство 1" << std::endl;
        std::cout << "Писатель отправил вторую книгу в издательство 2" << std::endl;
        std::cout << "Писатель отправил третью книгу в издательство 1" << std::endl;
        std::cout << "Писатель ожидает гонорары от издательств..." << std::endl;

        // Писатель принимает три гонорара
        int received_payment1, received_payment2, received_payment3;
        MPI_Recv(&received_payment1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&received_payment2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&received_payment3, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Вывод информации о полученных гонорарах
        std::cout << "Писатель получил гонорар " << received_payment1 << " руб. от издательства 1 за книгу 1\n";
        std::cout << "Писатель получил гонорар " << received_payment2 << " руб. от издательства 2 за книгу 2\n";
        std::cout << "Писатель получил гонорар " << received_payment3 << " руб. от издательства 1 за книгу 3\n";
    }
    else if (rank == 1) { // Если ранг равен 1, то это первое издательство
        // Принимаем сообщения о книгах и отправляем гонорары
        int received_book1, received_book3;
        MPI_Recv(&received_book1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&received_book3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Вывод информации о принятых книгах
        std::cout << "Издательство 1 приняло книгу от писателя" << std::endl;

        // Генерация гонораров
        int payment1 = dis(gen);
        int payment3 = dis(gen);

        // Отправка гонораров писателю
        MPI_Send(&payment1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&payment3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else if (rank == 2) { // Если ранг равен 2, то это второе издательство
        // Принимаем сообщение о книге и отправляем гонорар
        int received_book2;
        MPI_Recv(&received_book2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Вывод информации о принятой книге
        std::cout << "Издательство 2 приняло книгу от писателя" << std::endl;

        // Генерация гонорара
        int payment2 = dis(gen);

        // Отправка гонорара писателю
        MPI_Send(&payment2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // Завершение работы с MPI
    return 0;
}
