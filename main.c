#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Ez a láncolt lista egy eleme.
typedef struct FoodNode
{
    char *name;            // Az étel neve (Adat)
    int calories;          // Kalória érték (Adat)
    struct FoodNode *next; // Mutató a következő elemre
} FoodNode;
// Itt tároljuk a listák elejét (a mozdonyokat).
typedef struct
{
    FoodNode *meats;  // Húsok listájának eleje
    FoodNode *soups;  // Levesek listájának eleje
    FoodNode *fruits; // Gyümölcsök listájának eleje
    // ... others lists here
    FoodNode *custom; // Saját ételek listájának eleje
} AppData;
// --- ÚJ CSOMÓPONT LÉTREHOZÁSA (CREATE NODE) ---
FoodNode *create_node(char *name_buffer, int cal_value)
{
    // 1. Memória lefoglalása az új vagonnak
    FoodNode *new_node = (FoodNode *)malloc(sizeof(FoodNode));
    if (new_node == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }

    // 2. Név másolása (dinamikus memória a szövegnek)
    new_node->name = malloc(strlen(name_buffer) + 1);
    strcpy(new_node->name, name_buffer);
    // 3. Adatok beállítása
    new_node->calories = cal_value;
    new_node->next = NULL;
    // 4. FONTOS: Mivel ez az új vagon, még nincs utána semmi.
    return new_node;
}
// --- LISTA BETÖLTÉSE FÁJLBÓL (LOAD LIST) ---
FoodNode *load_food_list(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Warning: Could not open file %s\n", filename);
        return NULL; // Ha nincs fájl, üres listát adunk vissza
    }

    FoodNode *head = NULL; // A lista eleje (Fej)
    FoodNode *tail = NULL; // A lista vége (Farok)
    char buffer[100];
    int cal;

    // Olvasás a fájlból soronként
    while (fscanf(file, "%s %d", buffer, &cal) == 2)
    {
        // Új elem létrehozása
        FoodNode *new_node = create_node(buffer, cal);

        // BEKÖTÉS A LISTÁBA:
        if (head == NULL)
        {
            // Ha a lista még üres, ez lesz az első ÉS az utolsó elem is.
            head = new_node;
            tail = new_node;
        }
        else
        {
            // Ha már van elem, akkor a végére fűzzük.
            tail->next = new_node; // Az eddigi utolsó most már erre mutat
            tail = new_node;       // Frissítjük, hogy ez az új utolsó elem
        }
    }

    fclose(file);
    printf("List loaded from %s successfully.\n", filename);
    return head; // Visszaadjuk a lánc elejét
}
// --- EGYÉNI ÉTEL HOZZÁADÁSA (ADD CUSTOM FOOD) ---
// A lista végére fűzi az új elemet és menti fájlba.
FoodNode *add_custom_food(FoodNode *head, const char *filename)
{
    char name_buffer[100];
    int cal_input;
    printf("Type food (use _ without any space): ");
    scanf("%s", name_buffer);

    printf("Type calories ");
    scanf("%d", &cal_input);

    // 1. Új elem készítése a memóriában
    FoodNode *new_node = create_node(name_buffer, cal_input);

    // 2. Lista frissítése
    if (head == NULL)
    {
        // Ha üres volt a lista, ez lesz az első elem (a fej).
        head = new_node;
    }
    else
    {
        // Ha nem üres, megkeressük az utolsó elemet.
        FoodNode *current = head;
        while (current->next != NULL) // Amíg van következő...
        {
            current = current->next; // ...lépünk tovább.
        }
        current->next = new_node; // Most a current az utolsó elem. Hozzákötjük az újat.
    }

    // 3. Mentés fájlba (hozzáfűzés / append)
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Error saving to file.!\n");
    }
    else
    {
        fprintf(file, "%s %d\n", name_buffer, cal_input);
        fclose(file);
        printf("Successful! '%s' save in '%s'.\n", name_buffer, filename);
    }
    return head; // Visszaadjuk a (lehet, hogy módosult) listát
}
// --- KATEGÓRIA KIÍRÁSA (PRINT LIST) ---
void print_category(FoodNode *head)
{

    if (head == NULL)
    {
        printf("  empty list\n");
    }
    else
    {
        // A mutatót a lista elejére állítjuk
        FoodNode *current = head;
        int i = 1;
        // Amíg nem érünk a végére
        while (current != NULL)
        {
            printf("  %d. %s  (%d cal)\n", i, current->name, current->calories);
            current = current->next; // Lépés a következő elemre
            i++;
        }
    }
}
// --- ALKALMAZÁS INICIALIZÁLÁSA ---
AppData initialize_app()
{
    AppData data;

    printf("Loading... \n");

    data.meats = load_food_list("meat.txt");
    data.soups = load_food_list("soup.txt");
    data.fruits = load_food_list("fruits.txt");
    //... others .txt files
    // Próbáljuk betölteni a saját listát
    data.custom = load_food_list("ownfood.txt");

    printf("Successfully loaded");
    return data;
}
// --- LISTA FELSZABADÍTÁSA (FREE LIST) ---
// Nagyon fontos a memória szivárgás elkerülése végett!
void free_list(FoodNode *head)
{
    FoodNode *current = head;
    while (current != NULL)
    {
        FoodNode *temp = current; // Elmentjük a jelenlegit
        current = current->next;  // Lépünk a következőre

        // Felszabadítjuk a nevet (string)
        if (temp->name != NULL)
            free(temp->name);

        // Felszabadítjuk magát a csomópontot
        free(temp);
    }
}
// --- TELJES ADATBÁZIS TÖRLÉSE / DELETE FULL DATABASE ---
void free_app_data(AppData *db)
{
    free_list(db->meats);
    free_list(db->soups);
    free_list(db->fruits);
    //...
    free_list(db->custom);

    // Biztonsági okokból nullázzuk a mutatókat
    db->meats = NULL;
    db->soups = NULL;
    db->fruits = NULL;
    db->custom = NULL;
}
// --- ÉTEL KIVÁLASZTÁSA (SELECT FOOD) ---
int food_select(FoodNode *head)
{
    print_category(head); // Kiírjuk a listát

    if (head == NULL)
        return 0;

    int choice;
    printf("Type number of food or 0 to exit ");
    scanf("%d", &choice);

    if (choice <= 0)
        return 0; // Megszakítás

    FoodNode *current = head;
    int counter = 1;

    // Megkeressük a kiválasztott sorszámú elemet
    while (current != NULL)
    {
        if (counter == choice)
        {
            // MEGTALÁLTUK!
            printf("\n You ate %s (%d cal) \n", current->name, current->calories);
            return current->calories;
        }

        current = current->next; // Tovább lépünk
        counter++;
    }

    printf("invalid option!\n");
    return 0;
}

// --- STATISTICS ---
void print_statistics(int current_calories, int daily_goal, int items_count)
{
    int remaining = daily_goal - current_calories;
    float percentage = (float)current_calories / daily_goal * 100;

    // Prevent negative percentage or greater than 100% on the visual bar
    if (percentage > 100)
        percentage = 100;

    printf("\n==================================\n");
    printf("          DAILY STATISTIC           \n");
    printf("==================================\n");

    printf("Items consumed: %d\n", items_count);
    printf("Daily Goal:     %d cal\n", daily_goal);
    printf("Current:        %d cal\n", current_calories);

    if (remaining > 0)
    {
        printf("Remaining:      %d cal\n", remaining);
    }
    else
    {
        printf("WARNING: You have exceeded the target by %d calories!\n", abs(remaining));
    }

    // Visual Progress Bar
    printf("\nProgress: [");
    int bars = (int)(percentage / 5); // Each bar is worth 5% (Total 20 bars)
    for (int i = 0; i < 20; i++)
    {
        if (i < bars)
            printf("#"); //Filled part
        else
            printf("-"); // Empty part
    }
    printf("] %.1f%%\n", (float)current_calories / daily_goal * 100);
    printf("==================================\n");
}

// --- SAVE HISTORY ---
void save_daily_log(int total, int goal, int items)
{
    FILE *file = fopen("log.txt", "a");
    if (file == NULL)
    {
        printf("Error saving log.!\n");
        return;
    }

    // Get the current system time.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Formats the date and time: [YYYY.MM.DD HH:MM]
    // Example in the file: "[2025.12.08 18:30] Total: 1500/2000 | Items: 5 | Status: OK"

    fprintf(file, "[%d.%02d.%02d %02d:%02d] ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min);

    fprintf(file, "Total: %d/%d cal | Itens: %d | ", 
            total, goal, items);

    // Add a note indicating whether the goal was met or not.
    if (total <= goal)
    {
        fprintf(file, "Status: Success!\n");
    }
    else
    {
        fprintf(file, "Status: Exceeded (%d cal)\n", total - goal);
    }

    fclose(file);
    printf(">> History successfully saved to 'log.txt'.\n");
}

// --- HISTORICAL STATISTICS (WEEKLY/MONTHLY) ---
void view_history_stats(int mode) {
    FILE *file = fopen("log.txt", "r");
    if (file == NULL) {
        printf("No history found (log.txt file does not exist).\n");
        return;
    }

    char line[256]; // Buffer to read the entire line.
    
    //Variables for statistics
    int total_calories = 0;
    int entries_count = 0;
    
    //Take the time NOW to compare.
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    int current_month = now_tm->tm_mon; // 0 a 11

    printf("\n==================================\n");
    if (mode == 1) printf("    WEEKLY REPORT (7 DAYS)    \n");
    else           printf("      MONTHLY REPORT (CURRENT MONTH)     \n");
    printf("==================================\n");

    // Reads the file line by line.
    while (fgets(line, sizeof(line), file)) {
        int day, month, year, cal;

        // sscanf extracts numbers from formatted text
        // Log format: "[2025.12.08 18:30] Total: 1500 / 2000..."
        // The asterisk %*d means "read the number but ignore (hours and minutes)"
        if (sscanf(line, "[%d.%d.%d %*d:%*d] Total: %d", &year, &month, &day, &cal) == 4) {
            
            // --- 1. Recreate the file date in a time structure ---
            struct tm file_date = {0};
            file_date.tm_mday = day;
            file_date.tm_mon  = month - 1;    // Jan is 0 in C, but 1 in the file.
            file_date.tm_year = year - 1900;  // Years are counted from 1900.
            file_date.tm_isdst = -1;          // Let the system decide daylight saving time.

            // Converts the structure back to SECONDS (time_t)
            time_t file_seconds = mktime(&file_date);
            
            int include_this = 0;

            if (mode == 1) { 
                // --- WEEKLY MODE ---
                // Calculates the difference in seconds between NOW and the FILE DATE.
                double seconds_diff = difftime(now, file_seconds);
                // 604800 seconds = 7 days. If the difference is smaller, it's within a week.
                if (seconds_diff >= 0 && seconds_diff <= 604800) {
                    include_this = 1;
                }
            } 
            else if (mode == 2) {
                // --- MONTHLY MODE---
                // It only checks if the month and year are the same as the current ones.
                if ((month - 1) == current_month && (year - 1900) == now_tm->tm_year) {
                    include_this = 1;
                }
            }

            // If it passed the filter, add it up!
            if (include_this) {
                total_calories += cal;
                entries_count++;
                // Displays a mini-log on the screen
                printf(" - %02d/%02d: %d cal\n", day, month, cal);
            }
        }
    }
    
    fclose(file);

    // --- FINAL RESULTS ---
    printf("----------------------------------\n");
    if (entries_count > 0) {
        printf("Days recorded:   %d\n", entries_count);
        printf("Total consumed:  %d cal\n", total_calories);
        printf("Daily average:   %d cal\n", total_calories / entries_count);
    } else {
        printf("No data found for this period..\n");
    }
    printf("==================================\n");
}

int main(void)
{
    // 1. INICIALIZÁLÁS (Inicializar)
    // Betöltjük az adatokat a fájlokból
    AppData myApp = initialize_app();

    int total_calories = 0;

    int const daily_goal = 2000;
    int items_eaten_count = 0;

    int running = 1;

    while (running)
    {
        printf("\n--- MENÜ ---\n");
        printf("1. Meats\n");
        printf("2. Soups\n");
        printf("3. Fruits\n");
        printf("4. Add Custom food\n");
        printf("5. List Custom\n");
        printf("6. Statistic\n");
        printf("0. Exit\n");
        printf("Total Calories: %d\n", total_calories);
        printf("Response: ");

        int option;
        scanf("%d", &option);
        int calories_added = 0;

        switch (option)
        {
        case 1:
            printf("\n--- MEAT ---\n");
            calories_added = food_select(myApp.meats);
            total_calories += calories_added;
            break;
        case 2:
            printf("\n--- SOUP ---\n");
            calories_added = food_select(myApp.soups);
            total_calories += calories_added;
            break;
        case 3:
            printf("\n--- FRUITS ---\n");
            calories_added = food_select(myApp.fruits);
            total_calories += calories_added;
            break;
        case 4:
            // Itt frissítjük a custom listát, mert a függvény visszaadja az új fejet (head)
            myApp.custom = add_custom_food(myApp.custom, "ownfood.txt");
            break;
        case 5:
            printf("\n--- OWN FOOD ---\n");
            // Itt csak kiírjuk, nem eszünk belőle (de használhatnánk a food_select-et is)
            calories_added = food_select(myApp.custom);
            total_calories += calories_added;
            break;
        case 6:
            // Statistics Submenu
            printf("\n--- STATISTIC ---\n");
            printf("1. Today \n");
            printf("2. Last 7 Days (History)\n");
            printf("3. This Month (History)\n");
            printf("Response: ");
            
            int stat_opt;
            scanf("%d", &stat_opt);
            
            if (stat_opt == 1) {
                // Call the visual function of TODAY
                print_statistics(total_calories, daily_goal, items_eaten_count);
            } 
            else if (stat_opt == 2) {
                // Calls up the new function mode 1 (Weekly)
                view_history_stats(1);
            }
            else if (stat_opt == 3) {
                // Call the new function mode 2 (Monthly)
                view_history_stats(2);
            }
            
            printf("\nPress Enter to continue...");
            getchar(); getchar(); 
            break;
        case 0:
            running = 0;
            char save_choice;
            printf("Do you want to save today's history in log.txt? (y/n): ");
            getchar();
            scanf("%c", &save_choice);

            if (save_choice == 'y' || save_choice == 'Y')
            {
                save_daily_log(total_calories, daily_goal, items_eaten_count);
            }
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid option\n");
        }
        if (calories_added > 0)
        {
            items_eaten_count++;
        }
    }

    // 2. TAKARÍTÁS (Limpeza)
    // Kilépés előtt felszabadítjuk a memóriát!
    free_app_data(&myApp);

    return 0;
}