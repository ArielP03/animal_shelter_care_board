#include "care_board.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

int main() {
    CareBoard board;
    int choice = 0;

    while (true) {
        std::cout << "\nAnimal Shelter Care Board\n"
                  << "1. Add animal\n"
                  << "2. Generate daily checklists\n"
                  << "3. Mark task complete\n"
                  << "4. Print daily checklists\n"
                  << "5. Print unfinished care\n"
                  << "6. Exit\n"
                  << "Choice: ";

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a number from 1 to 6.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    std::string id;
                    std::string name;
                    std::string species;
                    std::cout << "Enter ID, name, and species (dog/cat): ";
                    if (!(std::cin >> id >> name >> species)) {
                        return 1;
                    }

                    std::unique_ptr<CarePlan> plan;
                    std::vector<CareInstruction> instructions;
                    if (species == "dog" || species == "Dog") {
                        plan = std::make_unique<DogCarePlan>();
                        instructions = {
                            {"Feeding", "Follow staff instructions."},
                            {"Walk", "Follow staff instructions."}};
                    } else if (species == "cat" || species == "Cat") {
                        plan = std::make_unique<CatCarePlan>();
                        instructions = {
                            {"Feeding", "Follow staff instructions."},
                            {"Litter check", "Follow staff instructions."}};
                    } else {
                        std::cout << "Species must be dog or cat.\n";
                        break;
                    }

                    board.registerAnimal(AnimalRecord(
                        id, name, species, std::move(plan), instructions));
                    std::cout << "Animal added.\n";
                    break;
                }
                case 2:
                    board.generateDailyChecklists();
                    std::cout << "Checklists generated.\n";
                    break;
                case 3: {
                    std::string id;
                    int taskNumber = 0;
                    std::cout << "Enter animal ID and task number: ";
                    if (!(std::cin >> id >> taskNumber)) {
                        if (std::cin.eof()) {
                            return 0;
                        }
                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter an animal ID and a task number.\n";
                        break;
                    }

                    if (taskNumber < 1) {
                        std::cout << "Task number must be at least 1.\n";
                        break;
                    }
                    board.markTaskComplete( id, static_cast<std::size_t>(taskNumber - 1));
                    std::cout << "Task marked complete.\n";
                    break;
                }
                case 4:
                    board.printDailyChecklists();
                    break;
                case 5:
                    board.printUnfinishedCare();
                    break;
                case 6:
                    std::cout << "Goodbye.\n";
                    return 0;
                default:
                    std::cout << "Choose a number from 1 to 6.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }

    return 1;
}
