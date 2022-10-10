#include <stdio.h>
#include <string.h>

#include <json-c/json_object.h>
#include <json-c/json_util.h>

const char* FIR_FILE_NM = "cat.json";

void flush_input()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

struct PersonPhone
{
    char type[20];
    char phone[11];
};

struct PersonAddress
{
    char street [100];
    char city   [50];
    char state  [50];
    int  isHuge;
};

struct Person 
{
    char f_name[50];
    char l_name[50];
    int  age;
    int  isMarried;
    struct PersonAddress address;
    struct PersonPhone phones[2];
};

void fill_in_(char* to, int len, const char paramPrint[50])
{
    int c;
    printf("%s", paramPrint);
    int i = 0;
    while ((c = getchar()) != '\n' && i != len - 1)
        to[i++] = c;
    if (i < len)
        to[i] = '\0';
    else
        to[i - 1] = '\0';
}

struct Person fill_the_person()
{
    printf("Let's fill the info about you!\n");
    struct Person person;
    fill_in_(person.f_name, 50, "Firstname >");
    putchar('\n');
    fill_in_(person.l_name, 50, "Lastname >");
    printf("\nAge >");
    scanf("%d", &person.age);
    flush_input();
    printf("\nMarried (NO if <= 0, YES if > 0) >");
    scanf("%d", &person.isMarried);
    flush_input();
    // address
    fill_in_(person.address.state, 50, "State >");
    putchar('\n');
    fill_in_(person.address.city, 50, "City >");
    putchar('\n');
    fill_in_(person.address.street, 50, "Street >");
    printf("\nIs your city huge? (NO if <= 0, YES if > 0) >");
    scanf("%d", &person.address.isHuge);
    flush_input();
    printf("\nFill the phones:\n");
    int phones_count = 2;
    struct PersonPhone* phone = person.phones;
    while (phones_count--)
    {
        fill_in_(phone->type, 20, "Type >");
        putchar('\n');
        fill_in_(phone->phone, 11, "Phone >");
        putchar('\n');
        phone = phone + 1;
    }

   return person;
}

json_object* work_with_json(struct Person* person)
{
    json_object* person_object = json_object_new_object();
    json_object* person_name =json_object_new_string(person->f_name);
    json_object* person_fam =json_object_new_string(person->l_name);
    json_object* person_age =json_object_new_int(person->age);
    json_object* person_mar =json_object_new_boolean(person->isMarried);
    json_object* person_addr = json_object_new_object();
    json_object* person_state =json_object_new_string(person->address.state);
    json_object* person_city =json_object_new_string(person->address.city);
    json_object* person_street =json_object_new_string(person->address.street);
    json_object* person_huge =json_object_new_boolean(person->address.isHuge);
    json_object_object_add(person_addr, "State", person_state);
    json_object_object_add(person_addr, "City", person_city);
    json_object_object_add(person_addr, "Street", person_street);
    json_object_object_add(person_addr, "Huge", person_huge);
    struct PersonPhone* phone_p = person->phones;
    int p_count = 2;
    json_object *phones = json_object_new_array();
    while (p_count--)
    {
        json_object * phone = json_object_new_object();
        json_object * t = json_object_new_string(phone_p->type);
        json_object * ph = json_object_new_string(phone_p->phone);
        json_object_array_add(phones, t);
        json_object_array_add(phones, ph);
        phone_p = phone_p + 1;
    }
    json_object_object_add(person_object, "Name", person_name);
    json_object_object_add(person_object, "Last Name", person_fam);
    json_object_object_add(person_object, "Age", person_age);
    json_object_object_add(person_object, "isMarried", person_mar);
    json_object_object_add(person_object, "Address", person_addr);
    json_object_object_add(person_object, "Phones", phones);

    return person_object;
}

int main(int argc, char *argv[])
{
    struct Person person = fill_the_person();
    json_object* person_object = work_with_json(&person);

    const char file_name[] = "person.json";
    json_object_to_file(file_name, person_object);
    printf("\nFile was successfully written\n");

    printf("\nData from file '%s':\n", file_name);
    FILE* file = fopen(file_name, "r");
    int c;
    while ((c = fgetc(file)) != EOF)
        putchar(c);
    fclose(file);
    putchar('\n');

    char close;
    printf("\n\nDelete file '%s'? (y, n)\n", file_name);
    while (close != 'y' && close != 'n')
        scanf("%c", &close);
    if (close == 'y')
        remove(file_name);

    return 0;
}