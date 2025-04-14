#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    // Work in progress
    // To Do:
    // Add a program menu
    // Implement safer user input functions
    // Save/Retrieve payroll history

    // time
    int start_hour,end_hour, start_minute, end_minute;
    int previous_end_hour = 0, previous_end_minute = 0;

    char turn_over_char_input; // user input

    // bools
    bool is_turn_over = false;
    bool first_iteration = true;

    // constants
    const double PROGRAM_VERSION = 1.0;
    const int OVERTIME_BUFFER_MINUTES = 35;
    const int DAY_PAY = 130; // enter day pay
    const int OVERTIME_PAY_RATE = 20; // enter overtime hourly rate

    // sums
    int total_sum = 0;
    int days_worked = 0;
    int buffered_turn_over_hours = 0;
    int buffered_overtime_hours = 0;
    int overtime_hours = 0;
    int turn_over_hours = 0;
    int overtime_sum = 0;
    int turn_over_sum = 0;

    // single-day
    int day_turn_over_pay = 0;
    int turn_over_hour_diff = 0;
    int turn_over_minute_diff = 0;
    int work_hours_day = 0;
    int extra_minutes = 0;
    int day_overtime_hours = 0;
    int day_overtime_pay = 0;

    // funcs
    void calculate_workday_time(const int *start_minute, const int *end_minute, int *end_hour,  int *start_hour, int *work_hours_day, int *extra_minutes);
    void calculate_overtime(const int *work_hours_day, const int *extra_minutes, const int *overtime_buffer_m, const int *over_pay_rate, int *day_overtime_hours, int *buffered_overtime_hours, int *overtime_hours, int *day_overtime_pay);
    bool calculate_turnover(const bool *is_ta, const int *over_pay_rate, const int *prev_et, const int *st, const int *stm, const int *prev_etm, const int *overtime_buffer_m, int *turn_over_hour_diff, int *turn_over_minute_diff, int *buffered_turn_over_hours, int *day_turn_over_pay, int *turn_over_hours);
    bool time_user_input(char prompt[], char prompt2[], int *hour, int *minute);

    printf("                                     __     ___\n");
    printf("  /\\  /\\___  _   _ ___  ___    ___  / _|   / _ \\__ _ _   _\n");
    printf(" / /_/ / _ \\| | | / __|/ _ \\  / _ \\| |_   / /_)/ _` | | | |\n");
    printf("/ __  / (_) | |_| \\__ \\  __/ | (_) |  _| / ___/ (_| | |_| |\n");
    printf("\\/ /_/ \\___/ \\__,_|___/\\___|  \\___/|_|   \\/    \\__,_|\\__, |\n");
    printf("                                                     |___/ \n");
    printf("Version. %.1f -- Work in Progress \n\n", PROGRAM_VERSION);

    printf("Make sure you have entered your day pay, and overtime hourly rate. \n");
    printf("Current values: \n");
    printf("Day pay: %d\n", DAY_PAY);
    printf("Overtime hourly rate: %d\n", OVERTIME_PAY_RATE);


    while (1) {

        if(!time_user_input("Start time", "(set -1 to finish)", &start_hour, &start_minute))
        {
            break;
        }

        if(!time_user_input("End time", "", &end_hour, &end_minute))
        {
            break;
        }

        printf("Start hour: %d, minute: %d, End hour: %d, minute %d \n", start_hour, start_minute, end_hour, end_minute);

        while (1) {
            if (!first_iteration) {
                printf("Was this day consecutive (y/n) ? (turn over) \n");
                scanf(" %c",&turn_over_char_input);
                if(turn_over_char_input == 'y') {
                    is_turn_over=true;
                    printf("Your answered: Yes \n");
                    break;
                }else if (turn_over_char_input == 'n') {
                    is_turn_over=false;
                    printf("Your answered: No \n");
                    break;
                }
                printf("Invalid input! \n");
            }
            else{
                break;
            }
        }
        calculate_workday_time(&start_minute, &end_minute, &end_hour, &start_hour , &work_hours_day, &extra_minutes);

        day_overtime_hours = 0;
        day_overtime_pay = 0;

        calculate_overtime(&work_hours_day, &extra_minutes, &OVERTIME_BUFFER_MINUTES, &OVERTIME_PAY_RATE, &day_overtime_hours, &buffered_overtime_hours, &overtime_hours, &day_overtime_pay);

        if (!calculate_turnover(&is_turn_over, &OVERTIME_PAY_RATE, &previous_end_hour, &start_hour, &start_minute, &previous_end_minute, &OVERTIME_BUFFER_MINUTES, &turn_over_hour_diff, &turn_over_minute_diff, &buffered_turn_over_hours, &day_turn_over_pay, &turn_over_hours))
        {
            return 0;
        }

        previous_end_hour = end_hour;
        previous_end_minute = end_minute;

        overtime_sum += day_overtime_pay;
        turn_over_sum += day_turn_over_pay;
        total_sum = total_sum + day_overtime_pay + day_turn_over_pay + DAY_PAY;
        days_worked++;

        printf("-----------------------\n");
        printf("Hours worked this day: %d \n", work_hours_day);
        printf("Minutes: %d \n", extra_minutes);
        printf("-----------------------\n");
        printf("Overtime hours today: %d\n", day_overtime_hours);
        printf("Overtime pay today: %d\n", day_overtime_pay);
        if (turn_over_hour_diff > 0) {
            printf("Turn over hours today: %d\n", turn_over_hour_diff);
            printf("Turn over pay today: %d\n", day_turn_over_pay);
        }
        printf("-----------------------\n");
        printf("All Overtime hours: %d\n", overtime_hours);
        printf("Of which, buffered sum: %d\n", buffered_overtime_hours);
        printf("All Turn-over hours: %d\n", turn_over_hours);
        printf("Of which, buffered sum: %d\n", buffered_turn_over_hours);
        printf("Days worked: %d\n", days_worked);
        printf("-----------------------\n");
        printf("Day pay sum: %d\n", days_worked * DAY_PAY);
        printf("Overtime sum: %d\n", overtime_sum);
        printf("Turn-over sum: %d\n", turn_over_sum);
        printf("Total sum: %d\n", total_sum);
        printf("-----------------------\n");

        first_iteration = false;
    }

    return 0;
}

bool time_user_input(char prompt[], char prompt2[], int *hour, int *minute)
{
    int temp_hour = 0;
    int temp_minute = 0;

    printf("%s (HH:MM 24-hour format): %s \n", prompt, prompt2);
    while (1) {
        if (scanf(" %d:%d",&temp_hour,&temp_minute) == 2 && temp_hour >= 0 && temp_hour < 24 && temp_minute >= 0 && temp_minute < 60) {
            *hour = temp_hour;
            *minute = temp_minute;
            return 1;
        }
        if (temp_hour < 0 || temp_minute < 0)
        {
            return 0; // program end
        }

        printf("Error, invalid  input!\n");
        while (getchar() != '\n');
    }
}

void calculate_workday_time(const int *start_minute, const int *end_minute, int *end_hour,  int *start_hour, int *work_hours_day, int *extra_minutes){
    if (*start_minute <= *end_minute) {
        *extra_minutes = *end_minute - *start_minute;
    }
    else {
        *extra_minutes = 60 - abs(*end_minute - *start_minute);
    }
    if(*end_hour == 0)
    {
        *end_hour = 24;
    }
    if(*start_hour == 0)
    {
        *start_hour = 24;
    }
    if (*end_hour < *start_hour) {
        *work_hours_day = *end_hour + 24 - *start_hour;
    }
    else {
        *work_hours_day = *end_hour - *start_hour;
    }
}

void calculate_overtime(const int *work_hours_day, const int *extra_minutes, const int *overtime_buffer_minutes, const int *over_pay_rate, int *day_overtime_hours, int *buffered_overtime_hours, int *overtime_hours, int *day_overtime_pay){
    *day_overtime_hours = *work_hours_day - 12;
    if (*extra_minutes > *overtime_buffer_minutes) {
        (*day_overtime_hours)++;
        (*buffered_overtime_hours)++;
    }else {
        //printf("Buffer missed. \n");
    }
    if (*day_overtime_hours > 0) {
        *day_overtime_pay = *day_overtime_hours * (*over_pay_rate);
        *overtime_hours = *overtime_hours + *day_overtime_hours;
    }
    else {
        *day_overtime_hours = 0;
    }
}

bool calculate_turnover(const bool *is_turn_over, const int *over_pay_rate, const int *previous_end_hour, const int *start_hour, const int *start_minute, const int *previous_end_minute, const int *overtime_buffer_m, int *turn_over_hour_diff, int *turn_over_minute_diff, int *buffered_turn_over_hours, int *day_turn_over_pay, int *turn_over_hours){
    *turn_over_hour_diff = 0;
    if (*is_turn_over) {
        // turn over hour difference
        if (*previous_end_hour < 13 && *previous_end_hour < *start_hour) {
            *turn_over_hour_diff = 12 - (*start_hour - *previous_end_hour);
        }
        else if (*previous_end_hour >= 13 && *previous_end_hour <= 24) {
            int diff24 = 24 - *previous_end_hour;
            *turn_over_hour_diff = 12 - (diff24 + *start_hour);
        }
        else {
            printf("Turn over hours are invalid. \n");
            return 0;
        }
        // turn over minute difference buffer calc
        *turn_over_minute_diff = *start_minute - *previous_end_minute;
        if (*turn_over_minute_diff < 0) {
            *turn_over_minute_diff = 60 + *turn_over_minute_diff;
            //printf("Turn over Minute Diff 2 (Normalized): %d\n", *turnoverMinuteDiff);
        }
        //printf("Turn over Minute Diff: %d\n", *turnoverMinuteDiff);
        if (*turn_over_minute_diff - *overtime_buffer_m > 0) {
            //printf("Turn-over buffer surpassed!\n");
            ++*turn_over_hour_diff;
            ++*buffered_turn_over_hours;
        }
        // turn over pay calculation
        *day_turn_over_pay = 0;
        if (*turn_over_hour_diff > 0) {
            *day_turn_over_pay = *over_pay_rate * 2 * (*turn_over_hour_diff);
            (*turn_over_hours) += *turn_over_hour_diff;
        }
    }
    return 1;
}