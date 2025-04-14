![C](https://img.shields.io/badge/language-C-green.svg)

## House of Pay payroll calculator

Work in Progress

![image](https://github.com/user-attachments/assets/4f028047-fd59-45aa-9f20-2ca4812630bb)


##  Calculates
1. **Daily Sum**
2. **Overtime daily sum**
3. **Turn-Over daily sum**
4. **Overtime and Turn-Over Buffer (35-minute)**
5. **Total sums**
6. **Days worked**

##  Required

To use this script, you need to fill in your daily pay and overtime hourly rate:
```c
    const int DAY_PAY = 0; // enter day pay
    const int OVERTIME_PAY_RATE = 0; // enter overtime hourly rate
```

##  Instructions
- Input time in HH:MM 24-hour format, e.g. 08:35, 17:00 , 2:10, 21:58.  
- After the first day entry, you will be asked if the next day was a consecutive work day to calculate turn-over (if any). Answer with y/n.  
- Enter any negative value to exit.

