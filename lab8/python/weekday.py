
import sys


def _month_day_at(values, index):
    if index < 0 or index >= len(values):
        raise IndexError("month index out of range")
    return values[index]


def getWeekday(year, month, day):
    MonthDays = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    if ((year % 4 == 0) and (year % 100 != 0)) or year % 400 == 0:
        MonthDays[1] = 29
    if year < 0 or month < 0 or day < 0:
        return 0
    if year > 10000:
        return -1
    elif month > 12:
        return -2
    elif day > _month_day_at(MonthDays, month - 1):
        return -3
    totalDays = 0
    for i in range(1, year):
        if ((i % 4 == 0) and (i % 100 != 0)) or i % 400 == 0:
            totalDays += 366
        else:
            totalDays += 365
    for i in range(1, month):
        totalDays += MonthDays[i - 1]
    totalDays += day
    tempDay = totalDays % 7
    if tempDay == 0:
        tempDay = 7
    return tempDay


def main():
    sys.stdout.reconfigure(encoding="utf-8")
    print("请输入日期（格式xxxx xx xx）：", end="", flush=True)
    tokens = []
    while len(tokens) < 3:
        line = sys.stdin.readline()
        if not line:
            raise ValueError("需要年、月、日三个整数")
        tokens.extend(line.split())
    year, month, day = map(int, tokens[:3])
    print("输入日期为：星期" + str(getWeekday(year, month, day)))


if __name__ == "__main__":
    main()
