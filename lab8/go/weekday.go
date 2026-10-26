package main

import "fmt"

func getWeekday(year, month, day int) int {
	MonthDays := [12]int{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	if ((year%4 == 0) && (year%100 != 0)) || year%400 == 0 {
		MonthDays[1] = 29
	}
	if year < 0 || month < 0 || day < 0 {
		return 0
	}
	if year > 10000 {
		return -1
	} else if month > 12 {
		return -2
	} else if day > MonthDays[month-1] {
		return -3
	}
	totalDays := 0
	for i := 1; i < year; i++ {
		if ((i%4 == 0) && (i%100 != 0)) || i%400 == 0 {
			totalDays += 366
		} else {
			totalDays += 365
		}
	}
	for i := 1; i < month; i++ {
		totalDays += MonthDays[i-1]
	}
	totalDays += day
	tempDay := totalDays % 7
	if tempDay == 0 {
		tempDay = 7
	}
	return tempDay
}

func main() {
	var year, month, day int
	fmt.Print("请输入日期（格式xxxx xx xx）：")
	if _, err := fmt.Scan(&year, &month, &day); err != nil {
		panic(err)
	}
	weekday := getWeekday(year, month, day)
	fmt.Printf("输入日期为：星期%d\n", weekday)
}
