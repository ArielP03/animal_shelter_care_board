# COP 3003 Git worksheet starter

A small console program for the Git/GitHub worksheets. Enter a checkout day; the program validates it and calculates a due day seven days later.

This is practice code for cloning, editing, building, checking, committing, and pushing. It is not a completed team project and does not fulfill the final GUI, class, inheritance, template, or persistence requirements. Your team may choose a different final project topic.

## Files

- `src/main.cpp`: complete runnable C++ starter.
- `README.md`: setup, build/run commands, and checks.
- `WORKSHEET_CHANGES.md`: three small change options and evidence fields.
- `sample_output.txt`: baseline examples before changes.
- `.gitignore`: excludes generated build files.

## Before class

Use C++14 or later and a working C++ compiler. No external libraries, GUI framework, package downloads, or CMake are needed. Run commands from the starter folder containing this README. Install/configure the compiler before class; the worksheet is not an installation session.

One teammate copies these files into the team's existing local clone, reviews them, commits the starter, and pushes it before October 21. If a README already exists, combine its useful instructions with these instead of discarding them. Keep `src` at the repository root so the commands below work. Other members clone the shared GitHub repository. Do not use a downloaded ZIP as the ongoing substitute for a Git clone.

The initial shared commit can use:

```text
git status
git add src/main.cpp README.md WORKSHEET_CHANGES.md sample_output.txt .gitignore
git diff --staged
git commit -m "Add runnable loan-day starter and build instructions"
git push
```

Run those commands inside the existing clone. Use your own Git identity. The person adding the starter does not receive credit for other members' changes.

## Build and run

Create the build folder once. If it already exists, skip that command. Rebuild after every source change. If compilation fails, fix it before running: an older executable may still exist.

### Linux with GCC

```text
mkdir build
g++ -std=c++14 -Wall -Wextra -pedantic src/main.cpp -o build/loan_starter
./build/loan_starter
```

### macOS with Clang

```text
mkdir build
clang++ -std=c++14 -Wall -Wextra -pedantic src/main.cpp -o build/loan_starter
./build/loan_starter
```

### Windows with MinGW GCC in PowerShell

```text
mkdir build
g++ -std=c++14 -Wall -Wextra -pedantic src/main.cpp -o build/loan_starter.exe
.\build\loan_starter.exe
```

### Windows with Microsoft C++

Open the Developer Command Prompt for Visual Studio, then change to this folder:

```text
mkdir build
cl /nologo /EHsc /std:c++14 /W4 src\main.cpp /Fo:build\main.obj /Fe:build\loan_starter.exe
build\loan_starter.exe
```

The Linux GCC command was verified for this starter. The macOS and Windows commands are provided for those compiler environments but were not executed during preparation. If you use an IDE, create a C++ console project using this same source file and record the build/run steps that work on your computer.

## Baseline checks

Run the program again for each input. These results apply before worksheet changes.

| Input | Expected result |
|---|---|
| `10` | Checkout day 10; due day 17 |
| `0` | Checkout day 0; due day 7 |
| `365` | Checkout day 365; due day 372 |
| `-1` or `366` | Range error; no loan summary |
| `abc`, `10.5`, `10abc`, or an empty line | Whole-number error; no loan summary |

Surrounding spaces are allowed. Day numbers are sequence numbers, not calendar dates: day 372 is valid as a due day. A successful run returns exit code 0. Invalid or missing input returns 1, intentionally.

## Understand the code

- The constants define the loan period and permitted checkout days.
- `isValidCheckoutDay` checks the range.
- `calculateDueDay` adds the loan period.
- `printLoanSummary` displays the calculated values.
- `main` reads one complete line, checks its format and range, then calls the processing and display functions.
- `parser >> extra` skips whitespace and detects any extra non-whitespace character. That rejects inputs such as `10abc` instead of silently accepting 10.

Read and explain the code before changing it. AI is permitted under the approved project policy: understand every keyword, symbol, and expression you submit and be prepared to explain its purpose and behavior yourself in front of the class.

## Use with the worksheets

Follow the turn-taking routine: pull from a clean working folder, make one small change, rebuild, check it, review the diff, commit, and push. Only one member edits/pushes at a time during the introductory exercise. Each student makes their own identifiable technical change.

Pull shared changes, inspect a teammate's commit, follow the build instructions, and repeat a relevant check. An already-current clone is acceptable; inspect an existing teammate commit rather than claiming a new download occurred.

Keep source and instructions in Git. Do not commit the generated `build` folder. Reuse commit links, expected/observed results, and teammate confirmations in the October 23 Design and GitHub Setup submission.
