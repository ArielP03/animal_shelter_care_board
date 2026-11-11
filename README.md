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
- **Qt GUI:** the main application window adds animals, generates checklists,
  displays tasks and completion status, and marks selected tasks complete
  through the existing `CareBoard` operations.

### Planned, not implemented

- **Application rules:** `CareBoard` and the domain classes will continue to
  enforce validation and care-task status rules; GUI button handlers will
  call these domain operations rather than reimplementing the rules.
- **File saving/loading:** a planned `CareBoardFileStore` module will
  serialize and restore animal records, staff instructions, and task status.
  The application layer will pass data to/from this module; persistence is
  not implemented yet.
- **Future GUI improvements:** enter custom staff instructions in the GUI,
  search/filter animals, and save/load records between runs.
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
**Dependencies:** C++ standard library and Qt 6 Widgets.

### Visual Studio Community on Windows

Install the **Desktop development with C++** workload, then open the repository
folder in Visual Studio. Visual Studio will detect `CMakeLists.txt` and
`CMakePresets.json`. Install Qt 6 for your compiler, select the `windows-debug` preset, and build
the `animal_shelter_care_board` target. The executable will be created under
`out/build/windows-debug/`.

The same build can be performed from a Developer PowerShell for Visual Studio:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
.\out\build\windows-debug\animal_shelter_care_board.exe
```

### Qt GUI build and run

The Qt interface is implemented in `src/main.cpp` and uses the existing
`CareBoard` classes. On macOS with Homebrew, install CMake and Qt 6:

```sh
brew install cmake
brew install qt@6
```

Then configure the project from the repository root:

```sh
export PATH="/opt/homebrew/bin:$PATH"
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6
cmake --build build --target animal_shelter_care_board
./build/animal_shelter_care_board
```

On Intel Macs, the Qt prefix is usually `/usr/local/opt/qt@6` instead of
`/opt/homebrew/opt/qt@6`.

The `build/` directory and executable are generated locally and should not be
committed.
