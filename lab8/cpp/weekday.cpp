#include <array>
#include <iostream>

int getWeekday(int year, int month, int day) {
    std::array<int, 12> MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0) {
        MonthDays[1] = 29;
    }
    if (year < 0 || month < 0 || day < 0) {
        return 0;
    }
    if (year > 10000) {
        return -1;
    } else if (month > 12) {
        return -2;
    } else if (day > MonthDays.at(month - 1)) {
        return -3;
    }
    int totalDays = 0;
    for (int i = 1; i < year; i++) {
        if (((i % 4 == 0) && (i % 100 != 0)) || i % 400 == 0) {
            totalDays += 366;
        } else {
            totalDays += 365;
        }
    }
    for (int i = 1; i < month; i++) {
        totalDays += MonthDays[i - 1];
    }
    totalDays += day;
    int tempDay = totalDays % 7;
    if (tempDay == 0) {
        tempDay = 7;
    }
    return tempDay;
}

int main() {
    int year, month, day;
    std::cout << "请输入日期（格式xxxx xx xx）：" << std::flush;
    if (!(std::cin >> year >> month >> day)) return 1;
    int weekday = getWeekday(year, month, day);
    std::cout << "输入日期为：星期" << weekday << '\n';
    return 0;
}
