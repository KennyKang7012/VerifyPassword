#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_ENTRIES 5
#define MAX_LEN 256
#define INI_FILE L".\\credentials.ini"
#define DEFAULT_VALUE L"N/A"

// ��l��INI���
void initialize_ini_file() {
    wchar_t section[MAX_LEN];
    for (int i = 0; i < MAX_ENTRIES; i++) {
        swprintf(section, MAX_LEN, L"Entry%d", i + 1);
        if (!WritePrivateProfileStringW(section, L"id", DEFAULT_VALUE, INI_FILE)) {
            wprintf(L"Error writing id for section %s\n", section);
        }
        if (!WritePrivateProfileStringW(section, L"password", DEFAULT_VALUE, INI_FILE)) {
            wprintf(L"Error writing password for section %s\n", section);
        }
    }
}

// �ˬdINI���O�_�s�b�A�Y���s�b�h��l��
void check_and_initialize_ini_file() {
    FILE* file = _wfopen(INI_FILE, L"r");
    if (file == NULL) {
        initialize_ini_file();
    }
    else {
        fclose(file);
    }
}

// �x�s�S�w��m��ID�M�K�X��INI���
void save_credentials_at(int index, const wchar_t* password) {
    wchar_t section[MAX_LEN];
    swprintf(section, MAX_LEN, L"Entry%d", index + 1);
    wchar_t id[MAX_LEN];
    swprintf(id, MAX_LEN, L"%d", index + 1);

    if (!WritePrivateProfileStringW(section, L"id", id, INI_FILE)) {
        wprintf(L"Error writing id for section %s\n", section);
    }
    if (!WritePrivateProfileStringW(section, L"password", password, INI_FILE)) {
        wprintf(L"Error writing password for section %s\n", section);
    }
}

// �qINI���Ū���S�w��m��ID�M�K�X
void load_credentials_at(int index, wchar_t* id, wchar_t* password) {
    wchar_t section[MAX_LEN];
    swprintf(section, MAX_LEN, L"Entry%d", index + 1);

    GetPrivateProfileStringW(section, L"id", DEFAULT_VALUE, id, MAX_LEN, INI_FILE);
    GetPrivateProfileStringW(section, L"password", DEFAULT_VALUE, password, MAX_LEN, INI_FILE);
}

// �ˬd�w�]�w��ID�MPassword�A�æ^��Index
void check_set_credentials(int* set_indices, int* count) {
    wchar_t section[MAX_LEN];
    wchar_t id[MAX_LEN];
    wchar_t password[MAX_LEN];
    int index = 0;

    for (int i = 0; i < MAX_ENTRIES; i++) {
        swprintf(section, MAX_LEN, L"Entry%d", i + 1);
        GetPrivateProfileStringW(section, L"id", DEFAULT_VALUE, id, MAX_LEN, INI_FILE);
        GetPrivateProfileStringW(section, L"password", DEFAULT_VALUE, password, MAX_LEN, INI_FILE);

        if (wcscmp(id, DEFAULT_VALUE) != 0 || wcscmp(password, DEFAULT_VALUE) != 0) {
            set_indices[index++] = i;
        }
    }

    *count = index;
}

// ���ұK�X�æ^�Ǥ��쪺Index
int verify_password(const wchar_t* password) {
    wchar_t section[MAX_LEN];
    wchar_t stored_password[MAX_LEN];

    for (int i = 0; i < MAX_ENTRIES; i++) {
        swprintf(section, MAX_LEN, L"Entry%d", i + 1);
        GetPrivateProfileStringW(section, L"password", DEFAULT_VALUE, stored_password, MAX_LEN, INI_FILE);

        if (wcscmp(stored_password, password) == 0) {
            return i + 1;
        }
    }

    return -1; // �Y�L�k��令�\�A�^��-1
}

// ��X�ðO���ۦP�K�X��ID
//void find_and_record_same_passwords() {
//    wchar_t passwords[MAX_ENTRIES][MAX_LEN];
//    wchar_t ids_with_same_passwords[MAX_ENTRIES][MAX_LEN * MAX_ENTRIES] = { 0 };
//    int i, j;
//
//    // ��l�ƱK�X�}�C
//    for (i = 0; i < MAX_ENTRIES; i++) {
//        swprintf(passwords[i], MAX_LEN, L"%s", DEFAULT_VALUE);
//    }
//
//    // Ū���Ҧ��K�X
//    for (i = 0; i < MAX_ENTRIES; i++) {
//        wchar_t section[MAX_LEN];
//        swprintf(section, MAX_LEN, L"Entry%d", i + 1);
//        GetPrivateProfileStringW(section, L"password", DEFAULT_VALUE, passwords[i], MAX_LEN, INI_FILE);
//    }
//
//    // ��X�ۦP���K�X�ðO��ID
//    for (i = 0; i < MAX_ENTRIES; i++) {
//        for (j = i + 1; j < MAX_ENTRIES; j++) {
//            if (wcscmp(passwords[i], passwords[j]) == 0 && wcscmp(passwords[i], DEFAULT_VALUE) != 0) {
//                wchar_t section_i[MAX_LEN];
//                wchar_t section_j[MAX_LEN];
//                wchar_t id_i[MAX_LEN];
//                wchar_t id_j[MAX_LEN];
//
//                swprintf(section_i, MAX_LEN, L"Entry%d", i + 1);
//                swprintf(section_j, MAX_LEN, L"Entry%d", j + 1);
//                GetPrivateProfileStringW(section_i, L"id", DEFAULT_VALUE, id_i, MAX_LEN, INI_FILE);
//                GetPrivateProfileStringW(section_j, L"id", DEFAULT_VALUE, id_j, MAX_LEN, INI_FILE);
//
//                if (wcslen(ids_with_same_passwords[i]) == 0) {
//                    swprintf(ids_with_same_passwords[i], MAX_LEN * MAX_ENTRIES, L"%s", id_i);
//                }
//                if (wcslen(ids_with_same_passwords[j]) == 0) {
//                    swprintf(ids_with_same_passwords[j], MAX_LEN * MAX_ENTRIES, L"%s", id_j);
//                }
//                if (wcslen(ids_with_same_passwords[i]) > 0 && wcslen(ids_with_same_passwords[j]) > 0) {
//                    swprintf(ids_with_same_passwords[i], MAX_LEN * MAX_ENTRIES, L"%s, %s", ids_with_same_passwords[i], id_j);
//                    swprintf(ids_with_same_passwords[j], MAX_LEN * MAX_ENTRIES, L"%s, %s", ids_with_same_passwords[j], id_i);
//                }
//            }
//        }
//    }
//
//    // �O���ۦP�K�X��ID
//    for (i = 0; i < MAX_ENTRIES; i++) {
//        if (wcslen(ids_with_same_passwords[i]) > 0) {
//            wchar_t section[MAX_LEN];
//            swprintf(section, MAX_LEN, L"SamePasswords");
//            //swprintf(section, MAX_LEN, L"SamePasswords%d", i + 1);
//            WritePrivateProfileStringW(section, L"IDs", ids_with_same_passwords[i], INI_FILE);
//        }
//    }
//}

// ��X�ðO���ۦP�K�X��ID
void find_and_record_same_passwords() {
    wchar_t passwords[MAX_ENTRIES][MAX_LEN];
    wchar_t ids_with_same_passwords[MAX_LEN * MAX_ENTRIES] = { 0 };
    int i, j;
    int found = 0;

    // ��l�ƱK�X�}�C
    for (i = 0; i < MAX_ENTRIES; i++) {
        swprintf(passwords[i], MAX_LEN, L"%s", DEFAULT_VALUE);
    }

    // Ū���Ҧ��K�X
    for (i = 0; i < MAX_ENTRIES; i++) {
        wchar_t section[MAX_LEN];
        swprintf(section, MAX_LEN, L"Entry%d", i + 1);
        GetPrivateProfileStringW(section, L"password", DEFAULT_VALUE, passwords[i], MAX_LEN, INI_FILE);
    }

    // ��X�ۦP���K�X�ðO��ID
    for (i = 0; i < MAX_ENTRIES; i++) {
        for (j = i + 1; j < MAX_ENTRIES; j++) {
            if (wcscmp(passwords[i], passwords[j]) == 0 && wcscmp(passwords[i], DEFAULT_VALUE) != 0) {
                wchar_t section_i[MAX_LEN];
                wchar_t section_j[MAX_LEN];
                wchar_t id_i[MAX_LEN];
                wchar_t id_j[MAX_LEN];

                swprintf(section_i, MAX_LEN, L"Entry%d", i + 1);
                swprintf(section_j, MAX_LEN, L"Entry%d", j + 1);
                GetPrivateProfileStringW(section_i, L"id", DEFAULT_VALUE, id_i, MAX_LEN, INI_FILE);
                GetPrivateProfileStringW(section_j, L"id", DEFAULT_VALUE, id_j, MAX_LEN, INI_FILE);

                if (wcslen(ids_with_same_passwords) == 0) {
                    swprintf(ids_with_same_passwords, MAX_LEN * MAX_ENTRIES, L"%s, %s", id_i, id_j);
                }
                else {
                    wchar_t temp[MAX_LEN * MAX_ENTRIES];
                    swprintf(temp, MAX_LEN * MAX_ENTRIES, L"%s, %s", ids_with_same_passwords, id_j);
                    wcscpy(ids_with_same_passwords, temp);
                }
                found = 1;
            }
        }
    }

    // �h��
    wchar_t unique_ids[MAX_LEN * MAX_ENTRIES] = { 0 };
    wchar_t* token = wcstok(ids_with_same_passwords, L", ");
    while (token != NULL) {
        if (wcsstr(unique_ids, token) == NULL) {
            if (wcslen(unique_ids) == 0) {
                wcscpy(unique_ids, token);
            }
            else {
                wcscat(unique_ids, L", ");
                wcscat(unique_ids, token);
            }
        }
        token = wcstok(NULL, L", ");
    }

    // �M�Ť��e��SamePasswords�Ϭq
    WritePrivateProfileStringW(L"SamePasswords", NULL, NULL, INI_FILE);

    // �O���ۦP�K�X��ID
    if (found && wcslen(unique_ids) > 0) {
        WritePrivateProfileStringW(L"SamePasswords", L"IDs", unique_ids, INI_FILE);
    }
}

// �^��[SamePasswords]�r�q���e
void get_same_passwords(wchar_t* output, size_t size) {
    GetPrivateProfileStringW(L"SamePasswords", L"IDs", L"", output, size, INI_FILE);
}

// �NIDs�ഫ����ư}�C
void parse_ids(const wchar_t* input, int* ids, int* count) {
    wchar_t buffer[MAX_LEN * MAX_ENTRIES];
    wcscpy(buffer, input);

    wchar_t* token = wcstok(buffer, L", ");
    int index = 0;

    while (token != NULL) {
        ids[index++] = _wtoi(token);
        token = wcstok(NULL, L", ");
    }

    *count = index;
}

int main() {
    check_and_initialize_ini_file();

    int choice;
    wchar_t id[MAX_LEN];
    wchar_t password[MAX_LEN];
    wchar_t same_passwords[MAX_LEN * MAX_ENTRIES];
    int same_password_ids[MAX_ENTRIES];
    int count;
    int index;

    while (1) {
        wprintf(L"Menu:\n");
        wprintf(L"1. Save Password\n");
        wprintf(L"2. Load ID and Password\n");
        wprintf(L"3. Check Set Credentials\n");
        wprintf(L"4. Verify Password\n");
        wprintf(L"5. Find and Record Same Passwords\n");
        wprintf(L"6. Get Same Passwords\n");
        wprintf(L"7. Exit\n");
        wprintf(L"Enter your choice: ");
        wscanf(L"%d", &choice);

        switch (choice) {
        case 1:
            wprintf(L"Enter the index (1-%d): ", MAX_ENTRIES);
            wscanf(L"%d", &index);
            if (index < 1 || index > MAX_ENTRIES) {
                wprintf(L"Invalid index!\n");
                break;
            }
            wprintf(L"Enter the Password: ");
            wscanf(L"%s", password);
            save_credentials_at(index - 1, password);
            wprintf(L"Credentials saved successfully!\n");
            break;
        case 2:
            wprintf(L"Enter the index (1-%d): ", MAX_ENTRIES);
            wscanf(L"%d", &index);
            if (index < 1 || index > MAX_ENTRIES) {
                wprintf(L"Invalid index!\n");
                break;
            }
            load_credentials_at(index - 1, id, password);
            wprintf(L"ID: %s\n", id);
            wprintf(L"Password: %s\n", password);
            break;
        case 3:
        {
            int set_indices[MAX_ENTRIES] = { 0 };
            int count = 0;
            check_set_credentials(set_indices, &count);
            if (count == 0) {
                wprintf(L"No set credentials found.\n");
            }
            else {
                wprintf(L"Set credentials found at indices: ");
                for (int i = 0; i < count; i++) {
                    wprintf(L"%d ", set_indices[i] + 1);
                }
                wprintf(L"\n");
            }
        }
        break;
        case 4:
            wprintf(L"Enter the Password: ");
            wscanf(L"%s", password);
            index = verify_password(password);
            if (index != -1) {
                wprintf(L"Password verified successfully at index %d!\n", index);
            }
            else {
                wprintf(L"Password verification failed!\n");
            }
            break;
        case 5:
            find_and_record_same_passwords();
            wprintf(L"Same passwords have been recorded in the INI file.\n");
            break;
        case 6:
            find_and_record_same_passwords();
            get_same_passwords(same_passwords, sizeof(same_passwords));
            wprintf(L"Same Passwords IDs: %s\n", same_passwords);

            parse_ids(same_passwords, same_password_ids, &count);
            wprintf(L"Same Passwords IDs: ");
            for (int i = 0; i < count; i++) {
                wprintf(L"%d ", same_password_ids[i]);
            }
            wprintf(L"\n");

            break;
        case 7:
            wprintf(L"Exiting...\n");
            exit(EXIT_SUCCESS);
        default:
            wprintf(L"Invalid choice!\n");
        }
    }

    return 0;
}

