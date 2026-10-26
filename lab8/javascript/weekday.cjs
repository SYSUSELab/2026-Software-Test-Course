'use strict';

function monthDayAt(values, index) {
    if (index < 0 || index >= values.length) {
        throw new RangeError('month index out of range');
    }
    return values[index];
}

function getWeekday(year, month, day) {
    const MonthDays = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    if (((year % 4 === 0) && (year % 100 !== 0)) || year % 400 === 0) {
        MonthDays[1] = 29;
    }
    if (year < 0 || month < 0 || day < 0) {
        return 0;
    }
    if (year > 10000) {
        return -1;
    } else if (month > 12) {
        return -2;
    } else if (day > monthDayAt(MonthDays, month - 1)) {
        return -3;
    }
    let totalDays = 0;
    for (let i = 1; i < year; i++) {
        if (((i % 4 === 0) && (i % 100 !== 0)) || i % 400 === 0) {
            totalDays += 366;
        } else {
            totalDays += 365;
        }
    }
    for (let i = 1; i < month; i++) {
        totalDays += MonthDays[i - 1];
    }
    totalDays += day;
    let tempDay = totalDays % 7;
    if (tempDay === 0) {
        tempDay = 7;
    }
    return tempDay;
}

module.exports = {getWeekday};

if (require.main === module) {
    process.stdout.write('请输入日期（格式xxxx xx xx）：');
    const rl = require('node:readline').createInterface({input: process.stdin});
    const tokens = [];
    let completed = false;
    rl.on('line', line => {
        if (completed) return;
        tokens.push(...line.trim().split(/\s+/).filter(Boolean));
        if (tokens.length >= 3) {
            const values = tokens.slice(0, 3).map(value => {
                if (!/^[+-]?\d+$/.test(value)) throw new TypeError('需要整数');
                const number = Number(value);
                if (number < -2147483648 || number > 2147483647) throw new RangeError('整数超出 int32 范围');
                return number;
            });
            const result = getWeekday(...values);
            console.log('输入日期为：星期' + result);
            completed = true;
            rl.close();
            process.stdin.pause();
        }
    });
    rl.on('close', () => {
        if (!completed) {
            process.stderr.write('需要年、月、日三个整数\n');
            process.exitCode = 1;
        }
    });
}
