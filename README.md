# Calorie Simulator

A console program in C to keep track of the calories you eat in a day. You pick foods from three lists
(meats, soups and fruits) or from a list of your own, follow the total against a daily goal of 2000
calories, and save each day to a log that feeds weekly and monthly reports.

## What it does

| Option | What happens |
|---|---|
| 1. Meats, 2. Soups, 3. Fruits | Shows the list with the calories of each food. Type a number to add that food to today's total. |
| 4. Add Custom food | Adds a food of your own, with its name and calories, to `ownfood.txt`. |
| 5. List Custom | Shows your own foods. Type a number to add one to today's total. |
| 6. Statistic | **Today:** how many items you ate, the total, the calories left and a progress bar. **Last 7 Days** and **This Month:** read `log.txt` and show each day's total and the daily average. |
| 0. Exit | Asks whether to save today's total in `log.txt`, then closes. |

The menu shows the running total every time it appears. Today's total stays in memory until you exit, so
save it when the program asks.

## How to run

On Windows, run `main.exe` from this folder. To build it yourself with GCC:

```
gcc main.c -o calorie_simulator
```

Run the program from this folder: it reads the food lists and the log from the current directory.

## Data files

| File | Contents | Example line |
|---|---|---|
| `meat.txt`, `soup.txt`, `fruits.txt` | One food per line: the name and the calories, separated by a space | `Banana 89` |
| `ownfood.txt` | Your own foods, in the same format; option 4 adds lines here | `Protein_Shake 120` |
| `log.txt` | One line for each saved day | see below |

Food names cannot contain spaces; use `_` instead, as in `Chicken_Breast`.

A line of `log.txt` looks like this:

```
[2025.12.08 18:23] Total: 2035 / 2000 cal | Itens: 11 | Status: Exceeded (35 cal)
```

The weekly report counts the last 7 days and the monthly report the current month, so the sample days
already in `log.txt`, from December 2025, only show up in those reports during that period.

## How the code is organized

Everything is in `main.c`. Each food list is a linked list of `FoodNode` (name, calories and a pointer to
the next food), built from its text file when the program starts; `AppData` keeps the start of the four
lists. All the memory is freed before the program closes. The daily goal is the `daily_goal` constant in
`main()`. The comments in the code are in Hungarian.
