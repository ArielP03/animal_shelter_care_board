#include "care_board.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVariant>
#include <QWidget>

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    CareBoard board;

    QMainWindow window;
    window.setWindowTitle("Animal Shelter Care Board");
    window.resize(900, 650);

    auto* pages = new QStackedWidget(&window);
    window.setCentralWidget(pages);

    const auto makePage = [pages](const QString& title) {
        auto* page = new QWidget();
        auto* layout = new QVBoxLayout(page);
        auto* helpText = new QLabel(title);
        helpText->setWordWrap(true);
        layout->addWidget(helpText);
        pages->addWidget(page);
        return std::pair<QWidget*, QVBoxLayout*>(page, layout);
    };

    auto* homePage = new QWidget();
    auto* homeLayout = new QVBoxLayout(homePage);
    auto* homeTitle = new QLabel("Animal Shelter Care Board");
    homeTitle->setObjectName("pageHeading");
    auto* homeDescription =
        new QLabel("Choose an option to manage routine animal care.");
    homeLayout->addWidget(homeTitle);
    homeLayout->addWidget(homeDescription);
// addAnimalPage 
    auto addAnimalPage =
        makePage("Enter the animal's information below.");
    auto* idInput = new QLineEdit();
    auto* nameInput = new QLineEdit();
    auto* speciesInput = new QComboBox();
    speciesInput->addItems({"Dog", "Cat"});
    auto* animalForm = new QFormLayout();
    animalForm->addRow("Animal ID:", idInput);
    animalForm->addRow("Name:", nameInput);
    animalForm->addRow("Species:", speciesInput);
    addAnimalPage.second->addLayout(animalForm);
    auto* saveAnimalButton = new QPushButton("Add animal");
    addAnimalPage.second->addWidget(saveAnimalButton);

    auto* generateButton = new QPushButton("Generate checklists");
    auto* generateStatus = new QLabel();
    
    auto checklistPage = makePage(
        "Daily checklists");
    auto* checklistTable = new QTableWidget();
    checklistPage.second->addWidget(checklistTable);

    auto completePage = makePage(
        "Select an unfinished checklist task, then mark it complete.");
    auto* completeTable = new QTableWidget();
    auto* completeButton = new QPushButton("Mark selected task complete");
    completePage.second->addWidget(completeTable);
    completePage.second->addWidget(completeButton);

    auto unfinishedPage = makePage( "Review all care tasks that are not yet complete.");
    auto* unfinishedTable = new QTableWidget();
    unfinishedPage.second->addWidget(unfinishedTable);

    pages->addWidget(homePage);

    const auto configureTaskTable = [](QTableWidget* table) {
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels(
            {"Animal ID", "Animal", "Species", "Task", "Details", "Status"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->verticalHeader()->setVisible(false);
    };
    configureTaskTable(checklistTable);
    configureTaskTable(completeTable);
    configureTaskTable(unfinishedTable);

    const auto populateTaskTable = [&board](QTableWidget* table,
                                            bool unfinishedOnly,
                                            bool selectableTasks) {
        table->setRowCount(0);

        for (const AnimalRecord& animal : board.animals()) {
            for (std::size_t index = 0; index < animal.dailyTasks().size();
                 ++index) {
                const CareTask& task = animal.dailyTasks()[index];
                if (unfinishedOnly && task.completed) {
                    continue;
                }

                const int row = table->rowCount();
                table->insertRow(row);
                const QStringList values = {
                    QString::fromStdString(animal.id()),
                    QString::fromStdString(animal.name()),
                    QString::fromStdString(animal.species()),
                    QString::fromStdString(task.category + " - " +
                                           task.taskName),
                    QString::fromStdString(task.details),
                    task.completed ? QString::fromUtf8("\xe2\x9c\x93 Done")
                                   : QStringLiteral("Pending")};

                for (int column = 0; column < values.size(); ++column) {
                    auto* item = new QTableWidgetItem(values[column]);
                    if (selectableTasks && !task.completed) {
                        item->setFlags(Qt::ItemIsEnabled |
                                       Qt::ItemIsSelectable);
                    } else {
                        item->setFlags(Qt::ItemIsEnabled);
                    }
                    table->setItem(row, column, item);
                }

                if (selectableTasks && !task.completed) {
                    auto* animalIdItem = table->item(row, 0);
                    animalIdItem->setData(
                        Qt::UserRole, QString::fromStdString(animal.id()));
                    animalIdItem->setData(
                        Qt::UserRole + 1,
                        QVariant::fromValue<qulonglong>(index));
                }
            }
        }

        if (table->rowCount() == 0) {
            const int row = table->rowCount();
            table->insertRow(row);
            auto* emptyItem = new QTableWidgetItem(
                "No tasks to display. Generate daily checklists first.");
            emptyItem->setFlags(Qt::ItemIsEnabled);
            table->setItem(row, 0, emptyItem);
            table->setSpan(row, 0, 1, table->columnCount());
        }
    };

    const auto addBackButton = [&pages, homePage](
                                   QVBoxLayout* layout) {
        auto* backButton = new QPushButton("Back to menu");
        layout->addWidget(backButton);
        backButton->setMinimumHeight(48);
        backButton->setMaximumWidth(300);
        backButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #080808;"
            "    border: 0.5px solid #ccc;"
            "    border-radius: 8px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #BAAFAF;"
            "}"
        );
        layout->setAlignment(Qt::AlignCenter);
        QObject::connect(backButton, &QPushButton::clicked, pages,
                         [pages, homePage]() { pages->setCurrentWidget(homePage); });
    };

    addBackButton(addAnimalPage.second);
    addBackButton(checklistPage.second);
    addBackButton(completePage.second);
    addBackButton(unfinishedPage.second);

    const auto navigateTo = [pages](QWidget* page) {
        pages->setCurrentWidget(page);
    };

    const auto addMenuButton = [homeLayout](const QString& title) {
        auto* button = new QPushButton(title);
        button->setMinimumHeight(48);
        button->setMaximumWidth(300);
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: #080808;"
            "    border: 0.5px solid #ccc;"
            "    border-radius: 8px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #BAAFAF;"
            "}"
        );
        homeLayout->addWidget(button);
        homeLayout->addSpacing(12);
        homeLayout->setAlignment(Qt::AlignCenter);
        return button;
    };

    auto* addMenuButtonWidget = addMenuButton("Add an animal");
    auto* generateMenuButton = addMenuButton("Generate daily checklists");
    auto* checklistMenuButton = addMenuButton("View daily checklists");
    auto* completeMenuButton = addMenuButton("Mark a task complete");
    auto* unfinishedMenuButton = addMenuButton("View unfinished care");
    auto* exitButton = addMenuButton("Exit");

    auto* homeStatus = new QLabel();
    homeLayout->addWidget(homeStatus);
    homeLayout->addStretch();

    QObject::connect(addMenuButtonWidget, &QPushButton::clicked, &window,
                     [navigateTo, addAnimalPage]() {
                         navigateTo(addAnimalPage.first);
                     });
    QObject::connect(generateMenuButton, &QPushButton::clicked, &window, [&]() {
        board.generateDailyChecklists();

        populateTaskTable(checklistTable, false, false);
        populateTaskTable(completeTable, false, true);
        populateTaskTable(unfinishedTable, true, false);

        QMessageBox::information(
            &window, "Checklists", "Daily checklists generated.");
        });
    QObject::connect(checklistMenuButton, &QPushButton::clicked, &window,
                     [navigateTo, checklistPage, &populateTaskTable,
                      checklistTable]() {
                         populateTaskTable(checklistTable, false, false);
                         navigateTo(checklistPage.first);
                     });
    QObject::connect(completeMenuButton, &QPushButton::clicked, &window,
                     [navigateTo, completePage, &populateTaskTable,
                      completeTable]() {
                         populateTaskTable(completeTable, false, true);
                         navigateTo(completePage.first);
                     });
    QObject::connect(unfinishedMenuButton, &QPushButton::clicked, &window,
                     [navigateTo, unfinishedPage, &populateTaskTable,
                      unfinishedTable]() {
                         populateTaskTable(unfinishedTable, true, false);
                         navigateTo(unfinishedPage.first);
                     });
    QObject::connect(exitButton, &QPushButton::clicked, &window,
                     &QMainWindow::close);

    QObject::connect(saveAnimalButton, &QPushButton::clicked, &window, [&]() {
        const QString id = idInput->text().trimmed();
        const QString name = nameInput->text().trimmed();
        if (id.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(&window, "Missing information",
                                 "Enter both an animal ID and name.");
            return;
        }

        const bool isDog = speciesInput->currentText() == "Dog";
        std::unique_ptr<CarePlan> plan;
        std::vector<CareInstruction> instructions;
        if (isDog) {
            plan = std::make_unique<DogCarePlan>();
            instructions = {{"Feeding", "Follow staff instructions."},
                            {"Walk", "Follow staff instructions."}};
        } else {
            plan = std::make_unique<CatCarePlan>();
            instructions = {{"Feeding", "Follow staff instructions."},
                            {"Litter check", "Follow staff instructions."}};
        }

        try {
            board.registerAnimal(AnimalRecord(
                id.toStdString(), name.toStdString(),
                isDog ? "dog" : "cat", std::move(plan),
                std::move(instructions)));
            idInput->clear();
            nameInput->clear();
            homeStatus->setText(QString("%1 was added.")
                                    .arg(QString::fromStdString(name.toStdString())));
            navigateTo(homePage);
        } catch (const std::exception& error) {
            QMessageBox::warning(&window, "Could not add animal",
                                 QString::fromUtf8(error.what()));
        }
    });

    QObject::connect(generateButton, &QPushButton::clicked, &window, [&]() {
        board.generateDailyChecklists();
        populateTaskTable(checklistTable, false, false);
        populateTaskTable(completeTable, false, true);
        populateTaskTable(unfinishedTable, true, false);
        generateStatus->setText("Daily checklists generated.");
    });

    QObject::connect(completeButton, &QPushButton::clicked, &window, [&]() {
        const int selectedRow = completeTable->currentRow();
        auto* selectedItem =
            selectedRow >= 0 ? completeTable->item(selectedRow, 0) : nullptr;
        if (selectedItem == nullptr ||
            !selectedItem->data(Qt::UserRole).isValid() ||
            !selectedItem->data(Qt::UserRole + 1).isValid()) {
            QMessageBox::information(
                &window, "Select a task",
                "Select an unfinished checklist task first.");
            return;
        }

        const std::string animalId =
            selectedItem->data(Qt::UserRole).toString().toStdString();
        const std::size_t taskIndex = static_cast<std::size_t>(
            selectedItem->data(Qt::UserRole + 1).toULongLong());
        try {
            board.markTaskComplete(animalId, taskIndex);
            populateTaskTable(completeTable, false, true);
            populateTaskTable(checklistTable, false, false);
            populateTaskTable(unfinishedTable, true, false);
        } catch (const std::exception& error) {
            QMessageBox::warning(&window, "Could not update task",
                                 QString::fromUtf8(error.what()));
        }
    });

    pages->setCurrentWidget(homePage);
    window.show();
    return app.exec();
}
