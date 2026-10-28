#ifndef CARE_BOARD_H
#define CARE_BOARD_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

struct CareInstruction {
    std::string taskName;
    std::string details;
};

struct CareTask {
    std::string category;
    std::string taskName;
    std::string details;
    bool completed = false;
};

class CarePlan {
public:
    virtual ~CarePlan() = default;
    virtual std::vector<CareTask> generateDailyTasks(
        const std::vector<CareInstruction>& instructions) const = 0;
};

class DogCarePlan final : public CarePlan {
public:
    std::vector<CareTask> generateDailyTasks(
        const std::vector<CareInstruction>& instructions) const override;
};

class CatCarePlan final : public CarePlan {
public:
    std::vector<CareTask> generateDailyTasks(
        const std::vector<CareInstruction>& instructions) const override;
};

class AnimalRecord {
public:
    AnimalRecord(std::string id, std::string name, std::string species,
                 std::unique_ptr<CarePlan> carePlan,
                 std::vector<CareInstruction> careInstructions);

    const std::string& id() const;
    const std::string& name() const;
    const std::string& species() const;
    const std::vector<CareTask>& dailyTasks() const;

    void generateDailyChecklist();
    void markTaskComplete(std::size_t taskIndex);

private:
    std::string id_;
    std::string name_;
    std::string species_;
    std::unique_ptr<CarePlan> carePlan_;
    std::vector<CareInstruction> careInstructions_;
    std::vector<CareTask> dailyTasks_;
};

class CareBoard {
public:
    void registerAnimal(AnimalRecord animal);
    void generateDailyChecklists();
    void markTaskComplete(const std::string& animalId, std::size_t taskIndex);
    void printDailyChecklists() const;
    void printUnfinishedCare() const;

private:
    std::vector<AnimalRecord> animals_;
};

#endif
