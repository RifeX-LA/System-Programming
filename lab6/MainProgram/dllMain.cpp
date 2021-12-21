#include <stdio.h>
#include <iostream> 
#include "..\Functional\plugins_manager.h"

#pragma comment (lib, "functional.lib")

bool LANG = true;

void ShowMenu(bool lang) {
    if (!lang) {
        printf("%s\n", "Меню:");
        printf("%s\n", "1 - Загрузить плагины");
        printf("%s\n", "2 - Выгрузить плагины");
        printf("%s\n", "3 - Показать информацию о загруженных плагинах");
        printf("%s\n", "4 - Выполнить плагины");
        printf("%s\n", "5 - Очистить консоль");
        printf("%s\n", "6 - Выход");
    }
    else {
        printf("%s\n", "Menu:");
        printf("%s\n", "1 - Load plugins");
        printf("%s\n", "2 - Unload plugins");
        printf("%s\n", "3 - Show info about loaded plugins");
        printf("%s\n", "4 - Execute plugins");
        printf("%s\n", "5 - Clean console");
        printf("%s\n", "6 - Exit");
    }
}

int main() {
	setlocale(LC_ALL, "Russian");
	printf("Choose the language\n");
	printf("%s\n", "\"0\", русский");
	printf("%s\n", "\"1\" english");
	scanf_s("%i", &LANG);

	bool menu = true;
    bool choosing;
	int choice;
	while (menu) {
        ShowMenu(LANG);

		choosing = true;
		while (choosing) {
			if (!LANG) {
                printf("%s", "Ваш выбор: ");
            }
			else {
                printf("%s", "Your choice: ");
            }
			scanf_s("%d", &choice);

			switch (choice) {
                case 1:
                    LoadPlugins(LANG);
                    choosing = false;
                    break;
                case 2:
                    UnloadPlugins(LANG);
                    choosing = false;
                    break;
                case 3:
                    ShowInfo(LANG);
                    choosing = false;
                    break;
                case 4:
                    RunPlugins(LANG);
                    choosing = false;
                    break;
                case 5:
                    system("cls");
                    choosing = false;
                    break;
                case 6:
                    Exit(LANG);
                    choosing = false;
                    menu = false;
                    break;
                default:
                    if (!LANG) {
                        printf("%s\n\n", "Некорректный выбор.");
                    }
                    else {
                        printf("%s\n\n", "Incorrect choose.");
                    }
                    break;
			}
		}
	}
	system("pause>>NUL");

	return 0;
}