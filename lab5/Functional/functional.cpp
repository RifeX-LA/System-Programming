#include "plugins_manager.h"

void PrintLocalized(const char* strLang1, const char* strLang2, bool lang) {
    if (!lang) {
        printf("%s", strLang1);
    }
    else {
        printf("%s", strLang2);
    }
}

void LoadPlugins(bool lang) {
	pluginsCount = 0;
	WIN32_FIND_DATA file;
	HANDLE hFile = FindFirstFile("Plugins\\*.dll", &file);

	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (pluginsCount == MAXPLUGINSALLOWED) {
			break;
		}
		char buf[256] = "Plugins\\";
		strcat_s(buf, 256, file.cFileName);
		Plugins[pluginsCount] = LoadLibrary(buf);
		++pluginsCount;
	} while (FindNextFile(hFile, &file));

	FindClose(hFile);
	if (!lang) {
        printf("Загружено плагинов: %d \n\n", pluginsCount);
    }
	else {
        printf("Plugins loaded: %d \n\n", pluginsCount);
    }
}

void UnloadPlugins(bool lang) {
	if (pluginsCount == 0) {
        PrintLocalized("Плагины не были загружены\n\n", "There're no loaded plugins\n\n", lang);
		return;
	}

	do {
		FreeLibrary(Plugins[pluginsCount - 1]);
		Plugins[pluginsCount - 1] = NULL;
		--pluginsCount;
	} while (pluginsCount != 0);

    PrintLocalized("Все плагины были выгружены\n\n", "All plugins were unloaded\n\n", lang);
}

void ShowInfo(bool lang) {
	if (pluginsCount == 0) {
        PrintLocalized("Плагины не были загружены\n\n", "There're no loaded plugins\n\n", lang);
		return;
	}

	DWORD bufdword = 0;
	char buffer[512];
	PluginInfo PI;
	for (int i = 0; i < pluginsCount; ++i) {
		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetDescription");
		PI(buffer, 512, &bufdword);

		if (!lang) {
            printf("Описание плагина%d: %s\n", i + 1, buffer);
        }
		else {
            printf("Plugin%d description: %s\n", i + 1, buffer);
        }

		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetAuthor");
		PI(buffer, 512, &bufdword);
		if (!lang) {
            printf("Автор: %s\n\n", buffer);
        }
		else {
            printf("Author: %s\n\n", buffer);
        }
	}
}

void RunPlugins(bool lang) {
	if (pluginsCount == 0) {
        PrintLocalized("Плагины не были загружены\n\n", "There're no loaded plugins\n\n", lang);
		return;
	}

	PluginExecute PE;
	for (int i = 0; i < pluginsCount; ++i) {
		PE = (PluginExecute)GetProcAddress(Plugins[i], "Execute");
		PE();
	}
}


void Exit(bool lang) {
	if (pluginsCount != 0) {
		do {
			FreeLibrary(Plugins[pluginsCount - 1]);
			Plugins[pluginsCount - 1] = NULL;
			--pluginsCount;
		} while (pluginsCount != 0);

        PrintLocalized("Все плагины были выгружены\n\n", "All plugins were unloaded\n\n", lang);
	}
    PrintLocalized("Программа завершена...\n\n", "Program finished...\n\n", lang);
}