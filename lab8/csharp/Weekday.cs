using System;

public class Weekday
{
    public static int getWeekday(int year, int month, int day)
    {
        int[] MonthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
        } else if (day > MonthDays[month - 1]) {
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

    public static void Main()
    {
        Console.OutputEncoding = new System.Text.UTF8Encoding(false);
        Console.Write("请输入日期（格式xxxx xx xx）：");
        string input = "";
        string[] tokens;
        do {
            string line = Console.ReadLine();
            if (line == null) throw new FormatException("需要年、月、日三个整数");
            input += " " + line;
            tokens = input.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
        } while (tokens.Length < 3);
        int result = getWeekday(int.Parse(tokens[0]), int.Parse(tokens[1]), int.Parse(tokens[2]));
        Console.WriteLine("输入日期为：星期" + result);
    }
}
