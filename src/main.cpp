#include "page.h"

int main() {
    Page page(5);  

    const char* record1 = "1231243";
    const char* record2 = "S123xRecord";
    page.InsertRecord(record1, std::strlen(record1) + 1); 
    page.InsertRecord(record2, std::strlen(record2) + 1); 

    page.WriteToFile("page_data.bin"); 

    Page loaded_page(5);               
    loaded_page.ReadFromFile("page_data.bin"); 

    loaded_page.PrintRecord(); 

    return 0;
}