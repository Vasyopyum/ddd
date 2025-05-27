#include <iostream>
#include <list>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
using namespace std;
int getIntegerInput(const string& prompt);

// class animal отвечает за вид,вохраст,ВРЕМЯ С РОЖДЕНИЯ И ТД(Характеристики животного
class Animal {
public:
    string name;
    int age, weight;
    enum Climate { DESERT, FOREST, ARCTIC, OCEAN } climate;
    bool isCarnivore;
    enum Gender { MALE, FEMALE } gender;
    int daysSinceBirth; // Дней с рождения
    int daysSinceLastCheck; // Дней с последней проверки ветеринаром
    bool isSick; // Болен ли животное

    Animal(string n, int a, int w, Climate c, bool carn, Gender g)
        : name(n), age(a), weight(w), climate(c), isCarnivore(carn),
        gender(g), daysSinceBirth(0), daysSinceLastCheck(0), isSick(false) {
    }

    int calculatePrice() const {
        int basePrice = 60;
        int price = basePrice;
        price += weight * 2;
        price -= age * 5;
        price += isCarnivore ? 100 : 0;
        price += static_cast<int>(climate) * 50;
        return max(price, 10);
    }

    void checkHealth() {
        daysSinceLastCheck = 0;
        isSick = false;
    }

    void updateHealth() {
        daysSinceLastCheck++;
        if (daysSinceLastCheck >= 3) {
            isSick = true;
        }
    }

    bool canBreed() const {
        return age >= 5 && !isSick; // Теперь проверяем возраст в годах
    }

    void grow() {
        daysSinceBirth++;
    }
};
// class Loan
class Loan {
public:
    int amount;
    int remaining;
    int daysPassed;

    Loan(int amt) : amount(amt), remaining(amt), daysPassed(0) {}

    int calculateDailyPayment() const {
        if (amount < 100) return remaining * 0.25;
        else if (amount < 1000) return remaining * 0.15;
        else if (amount < 10000) return remaining * 0.05;
        return remaining * 0.02;
    }
};
// class enclosure
class Enclosure {
public:
    Animal::Climate climate;
    int capacity;
    list<Animal> animals;
    int dailyCost;
    string sizeType;
    bool isClean; // Убран ли вольер

    Enclosure(Animal::Climate c, string size)
        : climate(c), sizeType(size), isClean(false) {
        if (size == "малый") {
            capacity = 5;
        }
        else if (size == "средний") {
            capacity = 10;
        }
        else if (size == "большой") {
            capacity = 15;
        }
        dailyCost = calculateDailyCost();
    }

    bool canAddAnimal(const Animal& animal) {
        if (animals.size() >= capacity) return false;
        if (animal.climate != climate) return false;
        if (!animals.empty()) {
            bool hasCarnivore = animals.front().isCarnivore;
            if (hasCarnivore != animal.isCarnivore) return false;
        }
        return true;
    }

    void addAnimal(const Animal& animal) {
        if (canAddAnimal(animal)) {
            animals.push_back(animal);
        }
    }

    void removeAnimal(const string& name) {
        for (auto it = animals.begin(); it != animals.end(); ++it) {
            if (it->name == name) {
                animals.erase(it);
                return;
            }
        }
    }

    int calculateCost() const {
        int baseCost = 100;
        int cost = baseCost;
        cost += capacity * 10;
        cost += static_cast<int>(climate) * 50;
        if (sizeType == "средний") cost *= 1.5;
        else if (sizeType == "большой") cost *= 2;
        return max(cost, 150);
    }

    int calculateDailyCost() const {
        int baseDailyCost = 10;
        int dailyCost = baseDailyCost;
        dailyCost += capacity / 10;
        dailyCost += static_cast<int>(climate) * 5;
        if (sizeType == "средний") dailyCost *= 1.5;
        else if (sizeType == "большой") dailyCost *= 2;
        return max(dailyCost, 10);
    }

    void clean() {
        isClean = true;
    }
    int getIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Некорректный ввод. Попробуйте снова.\n";
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }



    void breedAnimals() {
        cout << "\nВыберите двух животных для спаривания:\n";
        int index = 1;
        for (auto& animal : animals) {
            cout << index++ << ". " << animal.name << " (" << (animal.gender == Animal::MALE ? "Самец" : "Самка") << ")\n";
        }

        int choice1 = getIntegerInput("Введите номер первого животного: ");
        int choice2 = getIntegerInput("Введите номер второго животного: ");

        if (choice1 <= 0 || choice1 > animals.size() || choice2 <= 0 || choice2 > animals.size() || choice1 == choice2) {
            cout << "Неверный выбор животных!\n";
            return;
        }

        auto it1 = animals.begin();
        advance(it1, choice1 - 1);
        auto it2 = animals.begin();
        advance(it2, choice2 - 1);

        if (it1->gender == it2->gender || !it1->canBreed() || !it2->canBreed()) {
            cout << "Эти животные не могут спариваться!\n";
            return;
        }

        string babyName;
        cout << "Родился новый детеныш от " << it1->name << " и " << it2->name << "!\n";
        cout << "Введите имя для детеныша: ";
        cin.ignore();
        getline(cin, babyName);
        if (babyName.empty()) {
            // Создаем гибридное имя, если животные разных видов
            if (it1->name != it2->name) {
                string name1 = it1->name.substr(0, it1->name.length() / 2);
                string name2 = it2->name.substr(it2->name.length() / 2);
                babyName = name1 + name2;
            }
            else {
                babyName = it1->name + "-младший";
            }
        }

        // Создаем гибридное животное
        string hybridSpecies = (it1->name == it2->name) ? it1->name :
            it1->name.substr(0, it1->name.length() / 2) +
            it2->name.substr(it2->name.length() / 2);

        Animal baby(hybridSpecies, 0, (it1->weight + it2->weight) / 10,
            it1->climate, it1->isCarnivore, static_cast<Animal::Gender>(rand() % 2));

        // Устанавливаем гибридные характеристики
        baby.climate = static_cast<Animal::Climate>((static_cast<int>(it1->climate) + static_cast<int>(it2->climate)) / 2);
        baby.isCarnivore = (it1->isCarnivore || it2->isCarnivore); // Если хотя бы один хищник - детеныш хищник

        if (canAddAnimal(baby)) {
            addAnimal(baby);
            cout << "Детеныш " << baby.name << " (" << hybridSpecies << ") добавлен в вольер!\n";
        }
        else {
            cout << "Не удалось добавить детеныша " << baby.name << " - нет места в вольере!\n";
        }
    }


    void growAnimals() {
        for (auto& animal : animals) {
            animal.grow();
        }
    }
};
//class employee
class Employee {
public:
    string name;
    string position;
    int salary;
    int maxAnimals;
    int currentAnimals;
    int actionsLeft;
    list<Enclosure*> assignedEnclosures;

    Employee(string n, string pos, int sal, int max)
        : name(n), position(pos), salary(sal), maxAnimals(max), currentAnimals(0), actionsLeft(0) {
    }

    void resetDailyActions() {
        if (position == "Ветеринар") actionsLeft = 5;
        else if (position == "Кормилец") actionsLeft = 5;
        else if (position == "Уборщик") actionsLeft = 3;
    }

    bool canPerformAction() const {
        return actionsLeft > 0;
    }

    void performAction() {
        if (actionsLeft > 0) actionsLeft--;
    }
};
//class zoo
class Zoo {
public:
    string name;
    int money, food, popularity;
    int day;
    list<Enclosure> enclosures;
    list<Employee> employees;
    list<Loan> loans;

    Zoo(string n, int initialMoney)
        : name(n), money(initialMoney), food(0), popularity(50), day(1) {
    }


    bool takeLoan(int amount) {
        if (amount <= 0) return false;
        loans.emplace_back(amount);
        money += amount;
        return true;
    }

    bool payLoan(int index, bool fullPayment) {
        if (index < 0 || index >= loans.size()) return false;
        auto it = loans.begin();
        advance(it, index);
        if (fullPayment) {
            if (money < it->remaining) return false;
            money -= it->remaining;
            loans.erase(it);
        }
        else {
            int payment = it->calculateDailyPayment();
            if (money < payment) return false;
            money -= payment;
            it->remaining -= payment;
            it->daysPassed++;
            if (it->remaining <= 0) {
                loans.erase(it);
            }
        }
        return true;
    }

    void processDailyLoanPayments() {
        for (auto it = loans.begin(); it != loans.end(); ) {
            int payment = it->calculateDailyPayment();
            if (money >= payment) {
                money -= payment;
                it->remaining -= payment;
                it->daysPassed++;
                ++it;
            }
            else {
                popularity -= 5;
                ++it;
            }
            if (it != loans.end() && it->remaining <= 0) {
                it = loans.erase(it);
            }
        }
    }

    bool hasVeterinarian() {
        for (auto& emp : employees) {
            if (emp.position == "Ветеринар") return true;
        }
        return false;
    }

    bool hasCleaners() {
        for (auto& emp : employees) {
            if (emp.position == "Уборщик") return true;
        }
        return false;
    }

    bool hasFeeders() {
        for (auto& emp : employees) {
            if (emp.position == "Кормилец") return true;
        }
        return false;
    }

    void nextDay() {
        cout << "\nДень " << day << "\n";
        for (auto& emp : employees) {
            emp.resetDailyActions();
        }
        processDailyLoanPayments();

        bool vetExists = hasVeterinarian();
        for (auto& emp : employees) {
            if (emp.position == "Ветеринар") {
                while (emp.canPerformAction()) {
                    bool animalChecked = false;
                    for (auto enclosure : emp.assignedEnclosures) {
                        for (auto& animal : enclosure->animals) {
                            animal.checkHealth();
                            animalChecked = true;
                            emp.performAction();
                            if (!emp.canPerformAction()) break;
                        }
                        if (!emp.canPerformAction()) break;
                    }
                    if (!animalChecked) break;
                }
            }
        }


        bool feederExists = hasFeeders();
        if (feederExists) {
            for (auto& emp : employees) {
                if (emp.position == "Кормилец") {
                    while (emp.canPerformAction()) {
                        bool fedAnimals = false;
                        for (auto enclosure : emp.assignedEnclosures) {
                            if (food > 0 && !enclosure->animals.empty()) {
                                auto& animal = enclosure->animals.front();
                                food--;
                                animal.isSick = false;
                                fedAnimals = true;
                                emp.performAction();
                                if (!emp.canPerformAction()) break;
                            }
                        }
                        if (!fedAnimals) break;
                    }
                }
            }
        }


        bool cleanerExists = hasCleaners();
        if (cleanerExists) {
            for (auto& emp : employees) {
                if (emp.position == "Уборщик") {
                    while (emp.canPerformAction()) {
                        bool cleanedEnclosure = false;
                        for (auto enclosure : emp.assignedEnclosures) {
                            if (!enclosure->isClean) {
                                enclosure->clean();
                                popularity += 2;
                                cleanedEnclosure = true;
                                emp.performAction();
                                if (!emp.canPerformAction()) break;
                            }
                        }
                        if (!cleanedEnclosure) break;
                    }
                }
            }
        }

        for (auto& enc : enclosures) {
            enc.isClean = false;
        }

        if (!vetExists) {
            for (auto& enc : enclosures) {
                for (auto& animal : enc.animals) {
                    animal.updateHealth();
                }
            }
        }

        cout << "Бюджет прошлого дня: " << money << " монет\n";

        int visitors = 2 * popularity;
        int totalAnimals = getTotalAnimals();
        int income = visitors * totalAnimals;
        cout << "Посетители сегодня: " << visitors << "\n";

        // Обработка особых посетителей
        int bonusPopularity = 0;
        for (int i = 0; i < visitors; i++) {
            int chance = rand() % 1200;
            if (chance < 1) { // 1% шанс на знаменитость
                int bonus = 10;
                cout << "Знаменитость посетила ваш зоопарк! +" << bonus << " к популярности\n";
                bonusPopularity += bonus;
            }
            else if (chance < 5) { // 4% шанс на фотографа
                int bonus = 5;
                cout << "Фотограф посетил ваш зоопарк! +" << bonus << " к популярности\n";
                bonusPopularity += bonus;
            }
            else if (chance < 15) { // 10% шанс на бонусного посетителя
                int bonus = 1 + rand() % 3;
                cout << "Особый посетитель оставил положительный отзыв! +" << bonus << " к популярности\n";
                bonusPopularity += bonus;
            }
        }

        if (bonusPopularity > 0) {
            popularity += bonusPopularity;
            cout << "Общий бонус к популярности: +" << bonusPopularity << "\n";
        }

        cout << "Доход за день: +" << income << " монет\n";
        money += income;

        for (auto& emp : employees) {
            money -= emp.salary;
            emp.currentAnimals = 0;
        }

        for (auto& enc : enclosures) {
            money -= enc.dailyCost;
        }

        vector<string> sickAnimals;
        for (auto& enc : enclosures) {
            for (auto it = enc.animals.begin(); it != enc.animals.end(); ) {
                if (it->isSick) {
                    sickAnimals.push_back(it->name);
                    it = enc.animals.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        if (!sickAnimals.empty()) {
            cout << "\nУведомления\n";
            for (const string& name : sickAnimals) {
                cout << "Животное \"" << name << "\" умерло от болезни.\n";
            }
        }

        int fluctuation = popularity * 0.1;
        int change = (rand() % (2 * fluctuation + 1)) - fluctuation;
        popularity += change;
        popularity = max(popularity, 0);

        cout << "Бюджет текущего дня: " << money << " монет\n";
        day++;
    }

    int getTotalAnimals() {
        int total = 0;
        for (auto& enc : enclosures) total += enc.animals.size();
        return total;
    }
};

int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Попробуйте снова.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}


const string DESERT_SPECIES[] = { "Ктулху", "Дагон", "пингвин клоун", "космо гули", "Азатот" };
const string FOREST_SPECIES[] = { "Ньярлатхотет", "Ночные призраки", "Зуги", "Гуги", "Гончие тиндала" };
const string ARCTIC_SPECIES[] = { "Шогготы", "Бьякхи", "Космо водяной ", "Леденая кошка", "Арктическаяя обезьяна" };
const string OCEAN_SPECIES[] = { "Морской гашишь", "Водяной кот кокос", "Морской дракон", "Водяной мох  прямо ходящий", "Океанический гигант" };

string getRandomSpecies(Animal::Climate climate) {
    switch (climate) {
    case Animal::DESERT: return DESERT_SPECIES[rand() % 5];
    case Animal::FOREST: return FOREST_SPECIES[rand() % 5];
    case Animal::ARCTIC: return ARCTIC_SPECIES[rand() % 5];
    case Animal::OCEAN: return OCEAN_SPECIES[rand() % 5];
    default: return "Неизвестный вид";
    }
}

vector<string> getSpeciesByClimate(Animal::Climate climate) {
    switch (climate) {
    case Animal::DESERT: return vector<string>(begin(DESERT_SPECIES), end(DESERT_SPECIES));
    case Animal::FOREST: return vector<string>(begin(FOREST_SPECIES), end(FOREST_SPECIES));
    case Animal::ARCTIC: return vector<string>(begin(ARCTIC_SPECIES), end(ARCTIC_SPECIES));
    case Animal::OCEAN: return vector<string>(begin(OCEAN_SPECIES), end(OCEAN_SPECIES));
    default: return {};
    }
}

Animal generateRandomAnimal() {
    Animal::Climate climates[] = { Animal::DESERT, Animal::FOREST, Animal::ARCTIC, Animal::OCEAN };
    int randomAge = rand() % 20 + 1;
    int randomWeight = rand() % 490 + 10;
    Animal::Climate randomClimate = climates[rand() % 4];
    bool isCarnivore = rand() % 2 == 0;
    Animal::Gender randomGender = static_cast<Animal::Gender>(rand() % 2);
    string randomSpecies = getRandomSpecies(randomClimate);
    return Animal(randomSpecies, randomAge, randomWeight, randomClimate, isCarnivore, randomGender);
}

void assignEnclosures(Zoo& zoo) {
    for (auto& emp : zoo.employees) {
        emp.assignedEnclosures.clear();
        for (auto& enc : zoo.enclosures) {
            if (emp.currentAnimals < emp.maxAnimals) {
                emp.assignedEnclosures.push_back(&enc);
                emp.currentAnimals += enc.animals.size();
                if (emp.currentAnimals >= emp.maxAnimals) break;
            }
        }
    }
}
// void manageAnimals  : case1(()
void manageAnimals(Zoo& zoo) {
    cout << "\nУправление животными\n";
    cout << "1. Купить готовое животное\n";
    cout << "2. Продать животное\n";
    cout << "3. Просмотреть животных\n";
    cout << "4. Размножить животных\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nПокупка животного\n";
        const int NUM_RANDOM_ANIMALS = 5;
        vector<Animal> randomAnimals;
        for (int i = 0; i < NUM_RANDOM_ANIMALS; ++i) {
            randomAnimals.push_back(generateRandomAnimal());
        }
        cout << "Доступные животные:\n";
        for (int i = 0; i < randomAnimals.size(); ++i) {
            Animal& animal = randomAnimals[i];
            string climateName;
            switch (animal.climate) {
            case Animal::DESERT: climateName = "Пустыня"; break;
            case Animal::FOREST: climateName = "Лес"; break;
            case Animal::ARCTIC: climateName = "Арктика"; break;
            case Animal::OCEAN: climateName = "Океан"; break;
            }
            cout << i + 1 << ". Вид: " << animal.name
                << ", Возраст: " << animal.age << " лет"
                << ", Вес: " << animal.weight << " кг"
                << ", Пол: " << (animal.gender == Animal::MALE ? "Самец" : "Самка")
                << ", Климат: " << climateName
                << ", Тип: " << (animal.isCarnivore ? "Хищник" : "Травоядное")
                << ", Цена: " << animal.calculatePrice() << " монет\n";
        }
        int animalChoice = getIntegerInput("Введите номер животного для покупки: ");
        if (animalChoice <= 0 || animalChoice > randomAnimals.size()) {
            cout << "Неверный номер!\n";
            break;
        }
        Animal selectedAnimal = randomAnimals[animalChoice - 1];
        int price = selectedAnimal.calculatePrice();
        cout << "Итоговая цена животного: " << price << " монет\n";
        cout << "Хотите купить это животное?\n1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");
        if (confirm != 1) {
            cout << "Покупка отменена.\n";
            break;
        }
        if (zoo.money < price) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }
        string name;
        cout << "Введите имя для животного: ";
        cin.ignore();
        getline(cin, name);
        if (!name.empty()) {
            selectedAnimal.name = name;
        }
        vector<Enclosure*> suitableEnclosures;
        for (auto& enc : zoo.enclosures) {
            if (enc.climate == selectedAnimal.climate && enc.canAddAnimal(selectedAnimal)) {
                suitableEnclosures.push_back(&enc);
            }
        }
        if (suitableEnclosures.empty()) {
            cout << "Ошибка: Нет подходящего вольера!\n";
            break;
        }
        cout << "\nВыберите вольер для размещения животного:\n";
        for (int i = 0; i < suitableEnclosures.size(); ++i) {
            Enclosure* enc = suitableEnclosures[i];
            string climate;
            switch (enc->climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << i + 1 << ". Климат: " << climate
                << ", Животных: " << enc->animals.size() << "/" << enc->capacity << "\n";
        }
        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > suitableEnclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }
        Enclosure* selectedEnclosure = suitableEnclosures[enclosureChoice - 1];
        selectedEnclosure->addAnimal(selectedAnimal);
        zoo.money -= price;
        cout << "Животное \"" << selectedAnimal.name << "\" успешно добавлено в вольер!\n";
        assignEnclosures(zoo);
        break;
    }
    case 2: {
        cout << "\nПродажа животных\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
            index++;
        }
        int enclosureChoice = getIntegerInput("Выберите вольер: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер!\n";
            break;
        }
        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);
        if (encIt->animals.empty()) {
            cout << "В этом вольере нет животных!\n";
            break;
        }
        cout << "\nЖивотные в вольере:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            cout << index << ". " << animal.name
                << ", Возраст: " << animal.age << " лет"
                << ", Вес: " << animal.weight << " кг"
                << ", Пол: " << (animal.gender == Animal::MALE ? "Самец" : "Самка")
                << ", Цена: " << animal.calculatePrice() * 0.8 << " монет\n";
            index++;
        }
        int animalChoice = getIntegerInput("Введите номер животного для продажи: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }
        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);
        int price = animalIt->calculatePrice() * 0.8;
        cout << "Животное \"" << animalIt->name << "\" можно продать за " << price << " монет.\n";
        cout << "Вы уверены, что хотите продать это животное?\n1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");
        if (confirm != 1) {
            cout << "Продажа отменена.\n";
            break;
        }
        string animalName = animalIt->name;
        zoo.money += price;
        encIt->animals.erase(animalIt);
        cout << "Животное \"" << animalName << "\" продано за " << price << " монет.\n";
        assignEnclosures(zoo);
        break;
    }
    case 3: {
        cout << "\nСписок животных:\n";
        if (zoo.enclosures.empty()) {
            cout << "Нет вольеров с животными!\n";
            break;
        }
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << "Вольер (" << climate << "):\n";
            if (enc.animals.empty()) {
                cout << "  Нет животных\n";
                continue;
            }
            for (auto& animal : enc.animals) {
                cout << "  - " << animal.name
                    << ", Возраст: " << animal.age << " лет"
                    << ", Вес: " << animal.weight << " кг"
                    << ", Пол: " << (animal.gender == Animal::MALE ? "Самец" : "Самка")
                    << ", Тип: " << (animal.isCarnivore ? "Хищник" : "Травоядное")
                    << ", Состояние: " << (animal.isSick ? "Болен" : "Здоров") << "\n";
            }
        }
        break;
    }
    case 4: {
        cout << "\nРазмножение животных:\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
            index++;
        }
        int enclosureChoice = getIntegerInput("Выберите вольер: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный выбор!\n";
            break;
        }
        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);
        encIt->breedAnimals();
        break;
    }
    case 0:
        return;
    default:
        cout << "Неверный выбор!\n";
    }
}

void manageEnclosures(Zoo& zoo) {
    cout << "\nУправление вольерами\n";
    cout << "1. Построить вольер\n";
    cout << "2. Просмотреть вольеры\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nНовый вольер\n";
        cout << "Выберите климат для вольера:\n";
        cout << "0. Пустыня\n1. Лес\n2. Арктика\n3. Океан\n";
        Animal::Climate climate = static_cast<Animal::Climate>(getIntegerInput("Ваш выбор: "));
        cout << "Выберите размер вольера:\n";
        cout << "1. Малый (5 животных) - 150 монет\n";
        cout << "2. Средний (10 животных) - 300 монет\n";
        cout << "3. Большой (15 животных) - 500 монет\n";
        int sizeChoice = getIntegerInput("Ваш выбор: ");
        string sizeType;
        if (sizeChoice == 1) sizeType = "малый";
        else if (sizeChoice == 2) sizeType = "средний";
        else if (sizeChoice == 3) sizeType = "большой";
        else {
            cout << "Неверный выбор размера!\n";
            break;
        }
        Enclosure newEnclosure(climate, sizeType);
        int cost = newEnclosure.calculateCost();
        cout << "Стоимость вольера: " << cost << " монет\n";
        cout << "Хотите построить этот вольер?\n1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");
        if (confirm != 1) {
            cout << "Строительство отменено.\n";
            break;
        }
        if (zoo.money < cost) {
            cout << "Недостаточно средств для строительства!\n";
            break;
        }
        zoo.enclosures.emplace_back(climate, sizeType);
        zoo.money -= cost;
        cout << "Вольер успешно построен!\n";
        assignEnclosures(zoo);
        break;
    }
    case 2: {
        cout << "\nСписок вольеров:\n";
        if (zoo.enclosures.empty()) {
            cout << "Нет вольеров!\n";
            break;
        }
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Размер: " << enc.sizeType
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity
                << ", Состояние: " << (enc.isClean ? "Чистый" : "Грязный")
                << ", Расходы в день: " << enc.dailyCost << "\n";
            index++;
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Неверный выбор!\n";
    }
}

void manageResources(Zoo& zoo) {
    cout << "\nУправление ресурсами\n";
    cout << "1. Купить еду\n";
    cout << "2. Заказать рекламу\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nПокупка еды:\n";
        int amount = getIntegerInput("Сколько кг еды хотите купить? ");
        if (amount <= 0) {
            cout << "Неверное количество!\n";
            break;
        }
        int cost = amount * 2;
        if (zoo.money < cost) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }
        zoo.food += amount;
        zoo.money -= cost;
        cout << "Куплено " << amount << " кг еды за " << cost << " монет.\n";
        break;
    }
    case 2: {
        const int COST_PER_POPULARITY = 20;
        cout << "Стоимость одной единицы популярности: " << COST_PER_POPULARITY << " монет\n";
        int cost = getIntegerInput("Введите сумму для рекламной кампании: ");
        if (cost <= 0) {
            cout << "Неверная сумма!\n";
            break;
        }
        if (zoo.money >= cost) {
            int popularityIncrease = cost / COST_PER_POPULARITY;
            zoo.money -= cost;
            zoo.popularity += popularityIncrease;
            cout << "Популярность увеличена на " << popularityIncrease << "!\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Неверный выбор!\n";
    }
}
void manageLoans(Zoo& zoo) {
    while (true) {
        cout << "\nУправление кредитами\n";
        cout << "Текущий баланс: " << zoo.money << " монет\n";
        if (!zoo.loans.empty()) {
            cout << "\nТекущие кредиты:\n";
            int index = 1;
            int totalDebt = 0;
            for (auto& loan : zoo.loans) {
                int dailyPayment = loan.calculateDailyPayment();
                cout << index++ << ". Сумма: " << loan.amount << " монет\n";
                cout << "   Остаток: " << loan.remaining << " монет\n";
                cout << "   Ежедневный платеж: " << dailyPayment << " монет\n";
                cout << "   Дней с момента взятия: " << loan.daysPassed << "\n";
                totalDebt += loan.remaining;
            }
            cout << "Общий долг: " << totalDebt << " монет\n";
        }
        else {
            cout << "\nУ вас нет активных кредитов.\n";
        }
        cout << "\n1. Взять кредит\n";
        cout << "2. Погасить кредит\n";
        cout << "0. Назад\n";
        int choice = getIntegerInput("Выберите действие: ");
        switch (choice) {
        case 1: { // Взятие кредита
            if (!zoo.loans.empty()) {
                cout << "У вас уже есть активный кредит. Сначала погасите его.\n";
                break;
            }
            cout << "\nДоступные варианты кредитов:\n";
            cout << "1. Малый кредит (25% в день)\n";
            cout << "   Доступно: " << zoo.money * 5 << " монет\n";
            cout << "2. Средний кредит (15% в день)\n";
            cout << "   Доступно: " << zoo.money * 10 << " монет\n";
            cout << "3. Крупный кредит (5% в день)\n";
            cout << "   Доступно: " << zoo.money * 15 << " монет\n";
            int loanType = getIntegerInput("Выберите тип кредита (1-3): ");
            if (loanType < 1 || loanType > 3) {
                cout << "Неверный выбор!\n";
                break;
            }
            int amount;
            switch (loanType) {
            case 1: amount = zoo.money * 5; break;
            case 2: amount = zoo.money * 10; break;
            case 3: amount = zoo.money * 15; break;
            }
            cout << "Вы хотите взять " << amount << " монет? (1 - Да, 2 - Нет): ";
            int confirm = getIntegerInput("");
            if (confirm == 1) {
                zoo.takeLoan(amount);
                cout << "Кредит на " << amount << " монет успешно получен!\n";
            }
            else {
                cout << "Отмена операции.\n";
            }
            break;
        }
        case 2: { // Погашение кредита
            if (zoo.loans.empty()) {
                cout << "У вас нет активных кредитов.\n";
                break;
            }
            cout << "\nВыберите кредит для погашения:\n";
            int i = 1;
            for (auto& loan : zoo.loans) {
                cout << i++ << ". Остаток: " << loan.remaining << " монет\n";
            }
            int loanChoice = getIntegerInput("Номер кредита: ");
            if (loanChoice < 1 || loanChoice > zoo.loans.size()) {
                cout << "Неверный номер кредита!\n";
                break;
            }
            cout << "1. Погасить полностью\n";
            cout << "2. Погасить очередной платеж\n";
            int paymentType = getIntegerInput("Ваш выбор: ");
            if (paymentType == 1 || paymentType == 2) {
                bool success = zoo.payLoan(loanChoice - 1, paymentType == 1);
                if (success) {
                    cout << "Платеж успешно проведен!\n";
                }
                else {
                    cout << "Недостаточно средств для платежа!\n";
                }
            }
            else {
                cout << "Неверный выбор!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
}

void manageEmployees(Zoo& zoo) {
    cout << "\nУправление сотрудниками\n";
    cout << "1. Нанять сотрудника\n";
    cout << "2. Уволить сотрудника\n";
    cout << "3. Просмотреть список сотрудников\n";
    cout << "0. Назад\n";
    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nНаем сотрудника:\n";
        string name;
        cout << "Имя сотрудника: ";
        cin.ignore();
        getline(cin, name);
        cout << "Выберите должность:\n";
        cout << "1. Уборщик\n2. Ветеринар\n3. Кормилец\n";
        int position = getIntegerInput("Ваш выбор: ");
        string posName;
        int salary, maxAnimals;
        switch (position) {
        case 1:
            posName = "Уборщик";
            salary = 80;
            maxAnimals = 15;
            break;
        case 2:
            posName = "Ветеринар";
            salary = 150;
            maxAnimals = 20;
            break;
        case 3:
            posName = "Кормилец";
            salary = 100;
            maxAnimals = 30;
            break;
        default:
            cout << "Неверный выбор!\n";
            return;
        }
        if (zoo.money >= salary) {
            zoo.employees.emplace_back(name, posName, salary, maxAnimals);
            zoo.money -= salary;
            cout << "Сотрудник " << name << " нанят на должность " << posName << "!\n";
            assignEnclosures(zoo);
        }
        else {
            cout << "Недостаточно средств для найма!\n";
        }
        break;
    }
    case 2: {
        if (zoo.employees.empty()) {
            cout << "Нет сотрудников для увольнения!\n";
            break;
        }
        cout << "\nСписок сотрудников:\n";
        int i = 1;
        for (auto& emp : zoo.employees) {
            cout << i++ << ". " << emp.name << " (" << emp.position << ")\n";
        }
        int empToFire = getIntegerInput("Введите номер сотрудника для увольнения: ");
        if (empToFire < 1 || empToFire > zoo.employees.size()) {
            cout << "Неверный номер!\n";
            break;
        }
        auto it = zoo.employees.begin();
        advance(it, empToFire - 1);
        cout << "Сотрудник " << it->name << " уволен.\n";
        zoo.employees.erase(it);
        assignEnclosures(zoo);
        break;
    }
    case 3: {
        if (zoo.employees.empty()) {
            cout << "Нет сотрудников!\n";
            break;
        }
        cout << "\nСписок сотрудников:\n";
        for (auto& emp : zoo.employees) {
            cout << "- " << emp.name << " (" << emp.position << ")\n";
            cout << "  Зарплата: " << emp.salary << " монет/день\n";
            cout << "  Макс. животных: " << emp.maxAnimals << "\n";
            cout << "  Назначенные вольеры: " << emp.assignedEnclosures.size() << "\n";
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Неверный выбор!\n";
    }
}


int main() {
    srand(time(0));
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");
    string zooName;
    cout << "Введите название зоопарка: ";
    getline(cin, zooName);
    int initialMoney = getIntegerInput("Введите начальный капитал: ");
    while (initialMoney < 0) {
        cout << "Недопустимое значение. ";
        initialMoney = getIntegerInput("Введите начальный капитал: ");
    }
    Zoo zoo(zooName, initialMoney);
    zoo.employees.emplace_back("Василий Бог", "Глава парка", 50, 50);

    while (true) {
        cout << "\nМеню управления зоопарком '" << zoo.name << "'\n";
        cout << "День: " << zoo.day << "\n";
        cout << "Деньги: " << zoo.money << " монет\n";
        cout << "Еда: " << zoo.food << " кг\n";
        cout << "Популярность: " << zoo.popularity << "\n";
        cout << "Животных: " << zoo.getTotalAnimals() << "\n";
        cout << "Вольеров: " << zoo.enclosures.size() << "\n";
        cout << "Сотрудников: " << zoo.employees.size() << "\n";
        if (!zoo.loans.empty()) {
            int totalDebt = 0;
            for (auto& loan : zoo.loans) totalDebt += loan.remaining;
            cout << "Общий долг: " << totalDebt << " монет\n";
        }

        cout << "\n[1] Управление животными\n";
        cout << "[2] Управление сотрудниками\n";
        cout << "[3] Управление вольерами\n";
        cout << "[4] Управление ресурсами\n";
        cout << "[5] Управление кредитами\n";
        cout << "[0] Следующий день\n";

        int choice = getIntegerInput("Ваш выбор: ");
        if (choice == 0) {
            zoo.nextDay();
            // Проверка условий окончания игры
            if (zoo.money < 0) {
                cout << "\nИгра окончена! Вы банкрот.\n";
                break;
            }
            if (zoo.day > 10) {
                if (zoo.loans.empty()) {
                    cout << "\nПоздравляем! Вы успешно управляли зоопарком 10 дней!\n";
                    cout << "Итоговые показатели:\n";
                    cout << "- Денег: " << zoo.money << " монет\n";
                    cout << "- Животных: " << zoo.getTotalAnimals() << "\n";
                    cout << "- Популярность: " << zoo.popularity << "\n";
                    break;
                }
                else {
                    cout << "Вы не можете завершить игру с непогашенными кредитами!\n";
                }
            }
        }
        else if (choice == 1) {
            manageAnimals(zoo);
        }
        else if (choice == 2) {
            manageEmployees(zoo);
        }
        else if (choice == 3) {
            manageEnclosures(zoo);
        }
        else if (choice == 4) {
            manageResources(zoo);
        }
        else if (choice == 5) {
            manageLoans(zoo);
        }
        else {
            cout << "Неверный выбор. Попробуйте снова.\n";
        }

        // Дополнительная проверка на банкротство
        if (zoo.money < 0) {
            cout << "\nИгра окончена! У вас закончились деньги.\n";
            break;
        }
    }

    cout << "\nСпасибо за игру!\n";
    cout << "Нажмите Enter для выхода...";
    cin.ignore();
    cin.get();
    return 0;
}