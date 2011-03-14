int main(int argc, char** argv) {
    int curr  = -1;
    char* str = argv[1];

    if (argc != 2) goto error;

    e0:
         curr++;

             if(str[curr] == 'a') goto e1;
        else if(str[curr] == 'c') goto e0;
        else goto error;

    e1:
         curr++;

             if(str[curr] == 'a') goto e1;
        else if(str[curr] == 'b') goto e2;
        else if(str[curr] == 'c') goto e0;
        else goto error;

    e2:
         curr++;

        if (str[curr] == '\0') goto success;

             if(str[curr] == 'a') goto e2;
        else if(str[curr] == 'b') goto e2;
        else if(str[curr] == 'c') goto e2;
        else goto error;

    error:
        return -1;

    success:
        return 0;
}

