# Animal Shelter Care Board

An initial C++ console project for helping shelter volunteers track routine
animal care. The sample instructions are placeholders entered by staff; this
program does not give medical advice.

## Project description and goals

Shelter volunteers need a shared way to see the routine care recorded for each
animal, mark work as it is completed, and identify unfinished tasks. The goal
is to make daily handoffs clearer and reduce missed routine tasks by keeping
animal records and staff-entered instructions together. The application will
organize instructions supplied by shelter staff; it will not recommend
diagnoses, medication, or treatment.

## Planned features

1. Register an animal with a unique ID, name, species, and staff-entered care
   instructions.
2. Generate a daily checklist.
3. Mark a checklist task complete and show which care remains unfinished.
4. Search or filter animals by ID, name, or species.
5. Save and load animal records and task status between program runs.
6. Provide a GUI overview for volunteers to review animals and update tasks.


## Initial design

```text
CareBoard
  contains many AnimalRecord objects
      AnimalRecord
        contains one CarePlan (through unique_ptr<CarePlan>)
        contains staff-entered CareInstruction values
        owns the generated CareTask checklist

CarePlan (abstract)
  + generateDailyTasks(instructions) [pure virtual]
  DogCarePlan -> labels generated tasks "Dog care"
  CatCarePlan -> labels generated tasks "Cat care"

Console driver -> CareBoard -> AnimalRecord -> CarePlan
                              -> CareTask checklist
```

### Implemented in this checkpoint

- **CareBoard:** stores animals. Its public operations 
registerAnimal, generateDailyChecklists, markTaskComplete, printDailyChecklists, and printUnfinishedCare implement the starting application workflow. It enforces non-empty and unique IDs and verifies that a requested animal exists. 
- **AnimalRecords:** keeps care plan. From here its generated generateDailyChecklist and markTaskComplete update task to complete. 
- **CarePlan:** is an abstract base class. It's virtual generateDailyTasks function that is called through the base-class pointer held by AnimalRecord. 
- **DogCare and CatCare:** they are labels that generate their own tasks. 
- **CareTask:** stores a category, task name, staff-entered details, and 
completion status.

### Planned, not implemented

- **Application rules:** `CareBoard` and the domain classes will continue to
  enforce validation and care-task status rules; GUI button handlers will
  call these domain operations rather than reimplementing the rules.
- **File saving/loading:** a planned `CareBoardFileStore` module will
  serialize and restore animal records, staff instructions, and task status.
  The application layer will pass data to/from this module; persistence is
  not implemented yet.
- **GUI interactions:** a planned GUI view/controller will display animals,
  checklists, and unfinished care. User actions will call `CareBoard`
  operations, with results shown by the GUI; no GUI is implemented or
  required for this checkpoint.
- **GUI concept:** a home screen will show an animal list and unfinished-task
  count. Selecting an animal opens its record and checklist; a task control
  marks an item complete. A separate form will add animals and staff-entered
  instructions. GUI event handlers will validate basic input and call the
  same `CareBoard` operations as other interfaces rather than containing
  application rules.
- **Error handling:** reject missing animal IDs/names and duplicate IDs;
  report unknown animal IDs and invalid task indexes instead of silently
  ignoring requests. The starter throws standard exceptions for these
  invalid operations. A future GUI or file layer should catch errors at its
  boundary, show a clear message, and avoid saving partial/invalid data.
- Possible overload: `markTaskComplete(animalId, taskIndex)` and a future
  `markTaskComplete(animalId, taskName)` would support selection by either
  index or task name. Not implemented.
- Possible template: a future lookup helper could search either
  `std::vector<AnimalRecord>` or `std::list<AnimalRecord>`. This is a
  provisional idea and is not implemented.

## Build and run

**Operating system:** macOS or Windows
**Compiler:** Apple Clang from Xcode Command Line Tools (`c++`)  or VS Community
**C++ standard:** C++20 
**Dependencies:** C++ standard library.

From the repository root, run:

```sh
xcode-select --install
mkdir -p build
c++ -std=c++17 -Wall -Wextra -pedantic -Iinclude \
  src/care_board.cpp src/main.cpp -o build/care_board
./build/care_board
```

If Command Line Tools are already installed, the first command is unnecessary.
The `build/` directory and executable are generated locally and should not be
committed.o 
