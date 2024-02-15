#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>  //setw()

using namespace std;

class BinaryTree {
public:

    BinaryTree() : root{nullptr} {}

    [[nodiscard]] unsigned int getIterations() const { return add_count + remove_count + traversal_count; }

    ~BinaryTree() {
        deleteNode(root);
    }

    void add(int value) {
        root = addNode(root, value);
    }

    vector<int> sort() {
        vector<int> sorted_array;
        inorderTraversal(root, sorted_array);
        return sorted_array;
    }

private:

    struct Node {
        int value;
        Node *left{};
        Node *right{};

        explicit Node(int val) : value(val), left{nullptr}, right{nullptr} {}
    };

    unsigned int add_count = 0;
    unsigned int remove_count = 0;
    unsigned int traversal_count = 0;

    Node *root{};

    Node *addNode(Node *node, int value) {
        add_count++;
        if (node == nullptr) {
            return new Node(value);
        }
        if (value < node->value) {
            node->left = addNode(node->left, value);
        } else {
            node->right = addNode(node->right, value);
        }
        return node;
    }

    void inorderTraversal(Node *node, vector<int> &sorted_array) {
        if (node != nullptr) {
            traversal_count++;
            inorderTraversal(node->left, sorted_array);
            sorted_array.push_back(node->value);
            inorderTraversal(node->right, sorted_array);
        }
    }

    void deleteNode(Node *node) {
        if (node != nullptr) {
            remove_count++;
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
        }
    }


};

class Solution {
public:

    void Generation() {
        const int left_border = -25000, right_border = 25000;

        cout << "Введите количество элементов: ";
        cin >> count_elements;

        ofstream input_file_stream(name_file_input);
        if (!input_file_stream.is_open()) {
            throw "Ошибка. Невозможно создать/открыть файл"s;
        }
        for (int i = 0; i < count_elements - 1; ++i) {
            input_file_stream << left_border + rand() % (right_border - left_border + 1) << " ";
        }
        input_file_stream << left_border + rand() % (right_border - left_border + 1);
        input_file_stream.close();

        ReadFile();
    }

    void simpleSortMethod() {
        vector<int> random_numbers_local(random_numbers.begin(), random_numbers.end());

        int min_index;
        unsigned long long int iterations = 0;
        unsigned int start_time = clock();

        for (int k = 0; k < count_elements - 1; k++) {
            min_index = k;
            for (int i = 1 + k; i < count_elements; i++) {
                if (random_numbers_local[i] < random_numbers_local[min_index]) {
                    min_index = i;
                }
            }
            int temp = random_numbers_local[k];
            random_numbers_local[k] = random_numbers_local[min_index];
            random_numbers_local[min_index] = temp;
            ++iterations;
        }
        unsigned long long int end_time = clock();
        long double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

        writeHighStr();
        writeTimes(search_time, iterations, const_cast<string &>(sort_method_names[0]));
        writeInFile("simpleTXT", random_numbers_local);

    }

    void bubbleSortMethod() {
        vector<int> random_numbers_local(random_numbers.begin(), random_numbers.end());
        int length = count_elements;
        unsigned long long int iterations = 0;
        unsigned int start_time = clock();
        while (length != 0) {
            int max_index = 0;
            for (int i = 1; i < length; ++i) {
                if (random_numbers_local[i - 1] > random_numbers_local[i]) {
                    int temp = random_numbers_local[i - 1];
                    random_numbers_local[i - 1] = random_numbers_local[i];
                    random_numbers_local[i] = temp;
                    ++iterations;
                    max_index = i;
                }
            }
            length = max_index;
        }
        unsigned long long int end_time = clock();
        long double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

        writeTimes(search_time, iterations, sort_method_names[1]);
        writeInFile("bubbleTXT", random_numbers_local);

    }

    void combSortMethod() {
        vector<int> random_numbers_local(random_numbers.begin(), random_numbers.end());
        double factor = 1.2473309,
                step = count_elements - 1;
        unsigned long long int iterations = 0;
        unsigned int start_time = clock();
        while (step >= 1) {
            for (int i = 0; i + step < count_elements; ++i) {
                if (random_numbers_local[i] > random_numbers_local[i + static_cast<int>(step)]) {
                    int temp = random_numbers_local[i + static_cast<int>(step)];
                    random_numbers_local[i + static_cast<int>(step)] = random_numbers_local[i];
                    random_numbers_local[i] = temp;
                    ++iterations;
                }
            }
            step /= factor;
        }
        unsigned long long int end_time = clock();
        long double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

        writeTimes(search_time, iterations, sort_method_names[2]);
        writeInFile("combTXT", random_numbers_local);

    }

    void quickSort() {
        vector<int> random_numbers_local(random_numbers.begin(), random_numbers.end());
        unsigned long long int iterations = 0;
        unsigned int start_time = clock();
        sort(random_numbers_local.begin(), random_numbers_local.end(), [&iterations](auto el1, auto el2) -> bool {
            if (el1 < el2) {
                ++iterations;
                return true;
            }
            return false;
        });
        unsigned long long int end_time = clock();
        long double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

        writeTimes(search_time, iterations, sort_method_names[3]);
        writeInFile("quickTXT", random_numbers_local);
    }

    void binarySort() {
        vector<int> random_numbers_local = random_numbers;
        BinaryTree tree;
        unsigned int start_time = clock();
        for (int &value: random_numbers_local) {
            tree.add(value);
        }
        unsigned long long int end_time = clock();
        long double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        random_numbers_local = tree.sort();
        unsigned long long int iterations = tree.getIterations();
        writeTimes(search_time, iterations, sort_method_names[4]);
        writeInFile("binaryTXT", random_numbers_local);
    }

private:
    int count_elements{};
    const string name_file_input = "input.txt",
            name_file_times = "times.txt";


    mutable vector<string> sort_method_names = {"Сортировка простого выбора ",
                                                "Сортировка пузырьком ",
                                                "Сортировка расческой ",
                                                "Быстрая сортировка ",
                                                "Бинарная сортировка "};
    vector<int> random_numbers = {};

    void ReadFile() {
        ifstream read_file_stream(name_file_input);
        if (!read_file_stream.is_open()) {
            throw "Ошибка. Невозможно открыть файл"s;
        }
        for (int i = 0; i < count_elements; ++i) {
            string temp;
            read_file_stream >> temp;
            random_numbers.push_back(stoi(temp));
        }
        cout << endl;
    }

    void writeTimes(long double &search_time, unsigned long long int &iterations, string &name_sort) const {
        ofstream file_stream(name_file_times, ios::app);
        if (!file_stream.is_open()) {
            throw "Ошибка. Невозможно создать/открыть файл"s;
        }
        cout << name_sort + "выполнена."s
             << " Время выполнения: " << fixed << search_time << " секунд."s
             << " Количество итераций: " << iterations << endl;

        file_stream << std::left;
        file_stream << std::setw(46) << name_sort << " \t|" << "   \t"
                    << std::setw(15) << count_elements << "| "
                    << std::setw(13) << fixed << search_time << "|\t\t"
                    << std::setw(30) << iterations << endl;
        file_stream.close();
    }

    void writeHighStr() const {
        ofstream file_stream(name_file_times, ios::app);
        if (!file_stream.is_open()) {
            throw "Ошибка. Невозможно создать/открыть файл"s;
        }
        file_stream << std::string(100, '-') << endl;
        file_stream << std::left;
        file_stream << std::setw(46) << "\t  Название алгоритма"s << "|"
                    << std::setw(40) << " Количество элементов "s << "|"
                    << std::setw(14) << " Время работы "s << "|"
                    << std::setw(30) << "\t\tКоличество итераций"s << endl;
        file_stream << std::string(100, '-') << endl;

        file_stream.close();
    }

    static void writeInFile(const string &name_file, const vector<int> &local_numbers) {
        ofstream file_stream(name_file);
        if (!file_stream.is_open()) {
            throw "Ошибка. Невозможно создать/открыть файл";
        }
        for (const int &value: local_numbers) {
            file_stream << value << " ";
        }
        file_stream.close();
    }


};


int main() {
    setlocale(LC_ALL, "RU");
    srand(time(nullptr));

    Solution solution;
    solution.Generation();              // Генерация случайных чисел и создание массива
    solution.simpleSortMethod();        // Сортировка простым выбором
    solution.bubbleSortMethod();        // Сортировка пузырьком
    solution.combSortMethod();          // Сортировка "расческой"
    solution.quickSort();               // Быстрая сортировка
    solution.binarySort();              // Сортировка с помощью бинарного дерева


    return 0;
}