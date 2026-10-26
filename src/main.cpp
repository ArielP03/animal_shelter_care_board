#include <iostream>
#include <sstream>
#include <string>

// Day numbers are sequence numbers, not calendar dates.
constexpr int LOAN_DAYS = 7;
constexpr int FIRST_CHECKOUT_DAY = 0;
constexpr int LAST_CHECKOUT_DAY = 365;

bool isValidCheckoutDay(int day) {
    return day >= FIRST_CHECKOUT_DAY && day <= LAST_CHECKOUT_DAY;
}

int calculateDueDay(int checkoutDay, int loanDays) {
    return checkoutDay + loanDays;
}

void printLoanSummary(int checkoutDay, int dueDay) {
    std::cout << "Checkout day: " << checkoutDay << '\n';
    std::cout << "Due day: " << dueDay << '\n';
}

int main() {
    std::cout << "Equipment Loan Starter\n";
    std::cout << "Enter checkout day (" << FIRST_CHECKOUT_DAY
              << " to " << LAST_CHECKOUT_DAY << "): ";

    std::string input;
    if (!std::getline(std::cin, input)) {
        std::cout << "Error: no input was received.\n";
        return 1;
    }

    std::istringstream parser(input);
    int checkoutDay = 0;
    char extra = '\0';

    // Reject missing numbers and extra non-whitespace characters.
    if (!(parser >> checkoutDay) || (parser >> extra)) {
        std::cout << "Error: enter one whole-number day.\n";
        return 1;
    }

    if (!isValidCheckoutDay(checkoutDay)) {
        std::cout << "Error: checkout day must be between "
                  << FIRST_CHECKOUT_DAY << " and "
                  << LAST_CHECKOUT_DAY << ".\n";
        return 1;
    }

    const int dueDay = calculateDueDay(checkoutDay, LOAN_DAYS);
    printLoanSummary(checkoutDay, dueDay);
    return 0;
}
