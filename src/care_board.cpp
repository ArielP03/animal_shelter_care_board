#include "care_board.h"

#include <iostream>
#include <stdexcept>
#include <utility>


std::vector<CareTask> makeTasks(const std::vector<CareInstruction>& instructions,
                                const std::string& category) {
    std::vector<CareTask> tasks;
    tasks.reserve(instructions.size());

    for (const CareInstruction& instruction : instructions) {
        tasks.push_back(
            {category, instruction.taskName, instruction.details, false});
    }

    return tasks;
}


std::vector<CareTask> DogCarePlan::generateDailyTasks(
    const std::vector<CareInstruction>& instructions) const {
    return makeTasks(instructions, "Dog care");
}

std::vector<CareTask> CatCarePlan::generateDailyTasks(
    const std::vector<CareInstruction>& instructions) const {
    return makeTasks(instructions, "Cat care");
}

AnimalRecord::AnimalRecord(std::string id, std::string name,
                           std::string species,
                           std::unique_ptr<CarePlan> carePlan,
                           std::vector<CareInstruction> careInstructions)
    : id_(std::move(id)),
      name_(std::move(name)),
      species_(std::move(species)),
      carePlan_(std::move(carePlan)),
      careInstructions_(std::move(careInstructions)) {
    if (!carePlan_) {
        throw std::invalid_argument("An animal must have a care plan.");
    }
}

const std::string& AnimalRecord::id() const {
    return id_;
}

const std::string& AnimalRecord::name() const {
    return name_;
}

const std::string& AnimalRecord::species() const {
    return species_;
}

const std::vector<CareTask>& AnimalRecord::dailyTasks() const {
    return dailyTasks_;
}

void AnimalRecord::generateDailyChecklist() {
    dailyTasks_ = carePlan_->generateDailyTasks(careInstructions_);
}

void AnimalRecord::markTaskComplete(std::size_t taskIndex) {
    if (taskIndex >= dailyTasks_.size()) {
        throw std::out_of_range("Care task index is out of range.");
    }
    dailyTasks_[taskIndex].completed = true;
}

void CareBoard::registerAnimal(AnimalRecord animal) {
    if (animal.id().empty()) {
        throw std::invalid_argument("Animal ID cannot be empty.");
    }
    if (animal.name().empty()) {
        throw std::invalid_argument("Animal name cannot be empty.");
    }
    for (const AnimalRecord& existing : animals_) {
        if (existing.id() == animal.id()) {
            throw std::invalid_argument("Animal IDs must be unique.");
        }
    }
    animals_.push_back(std::move(animal));
}

void CareBoard::generateDailyChecklists() {
    for (AnimalRecord& animal : animals_) {
        animal.generateDailyChecklist();
    }
}

void CareBoard::markTaskComplete(const std::string& animalId,
                                 std::size_t taskIndex) {
    for (AnimalRecord& animal : animals_) {
        if (animal.id() == animalId) {
            animal.markTaskComplete(taskIndex);
            return;
        }
    }
    throw std::invalid_argument("Animal ID was not found.");
}

void CareBoard::printDailyChecklists() const {
    std::cout << "Daily care checklists\n";
    for (const AnimalRecord& animal : animals_) {
        std::cout << animal.name() << " (" << animal.id() << ", "
                  << animal.species() << ")\n";
        for (std::size_t index = 0; index < animal.dailyTasks().size(); ++index) {
            const CareTask& task = animal.dailyTasks()[index];
            std::cout << "  " << (index + 1) << ". ["
                      << (task.completed ? 'x' : ' ') << "] "
                      << task.category << " - " << task.taskName << ": "
                      << task.details << '\n';
        }
    }
}

void CareBoard::printUnfinishedCare() const {
    std::cout << "Unfinished care\n";
    bool foundUnfinishedTask = false;

    for (const AnimalRecord& animal : animals_) {
        for (const CareTask& task : animal.dailyTasks()) {
            if (!task.completed) {
                std::cout << "  " << animal.name() << " (" << animal.id()
                          << "): " << task.taskName << '\n';
                foundUnfinishedTask = true;
            }
        }
    }

    if (!foundUnfinishedTask) {
        std::cout << "  None\n";
    }
}
