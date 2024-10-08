#include "disk_manager.h"

#include<iomanip>
#include <string>
/*=======================================DiskManager================================================ */
void DiskManager::Insert(SQLInsert &st){
    std::string tb_name = st.tb_name();
    unsigned long values_size = st.values().size();

    Table *tbl = cm_->GetDB(db_name_)->GetTable(tb_name);
    
    if (tbl == NULL) {
        throw TableNotExistException();
    }

    int content_len = 0;
    std::vector<TKey> tkey_values;
    for (int i = 0; i < values_size; i++) {
        int value_type = st.values()[i].data_type;
        std::string value = st.values()[i].value;
        int length = tbl->ats()[i].length();
        content_len+=length;

        TKey tmp(value_type, length);
        tmp.ReadValue(value.c_str());
        tkey_values.push_back(tmp);
    }
    
    char* content = new char[content_len+1];
    char* content_ptr = content;
    for (std::vector<TKey>::iterator iter = tkey_values.begin();
         iter != tkey_values.end(); ++iter) {
      std::memcpy(content_ptr, iter->key(), iter->length());
      content_ptr += iter->length();
    }

    File file(tbl->GetFile());
    std::shared_ptr<PageDirectory> dir = file.GetPageDir(0);

    std::shared_ptr<Page> page = file.GetEnoughSpacePage(content_len);

    if(page == nullptr){
        std::cout<<"새로운 page"<<std::endl;
        std::shared_ptr<Page> new_page = std::make_shared<Page>(tbl->GetFile(),dir->GetIdx());
        new_page->SetFilename(tbl->GetFile());
        dir=file.AddPageToDirectory(*dir,*new_page);
        new_page->InsertRecord(content,content_len);
        file.WritePageToFile(*dir,*new_page);
    }else{
        std::cout<<"기존 page"<<std::endl;
        page->InsertRecord(content,content_len);
        file.WritePageToFile(*dir,*page);
    }
    file.WritePageDirToFile(*dir);
    // std::cout<<"[insert dir.size]"<<dir->GetSize()<<std::endl;
    cm_->WriteArchiveFile();
}

void DiskManager::Select(SQLSelect &st){ //select all
    Table *tbl = cm_->GetDB(db_name_)->GetTable(st.tb_name());

    for (int i = 0; i < tbl->GetAttributeNum(); ++i) {
        std::cout << std::setw(9) << std::left << tbl->ats()[i].attr_name();
    }
    std::cout << std::endl;

    File file(tbl->GetFile());
    std::shared_ptr<PageDirectory> dir = file.GetPageDir(0);
    std::vector<std::vector<TKey> > tkey_values;
    bm_->SetFile(&file);
    do{
        for(int i=0;i<dir->GetSize();i++)
        {
            std::cout<<"size::"<<dir->GetSize()<<std::endl;
            std::shared_ptr<Page> page;
            page=bm_->GetPageFromBufferPool(tbl->GetFile(),i);
            if(!page)
                page=bm_->GetPageFromDisk(*dir,i);
            
            // std::cout<<"[SELECT] page index : "<<page->GetPageIdx()<<"\tpage filename : "<<page->GetFilename()<<std::endl;
            std::vector<char> data = page->GetData();
            int offset = page->GetSlotOffset();
            for(int j=HEADER_SIZE;j<offset;j+=sizeof(Slot)){
                std::vector<TKey> tkey_value = ParseRecord(tbl,data,j);
                tkey_values.push_back(tkey_value);
            }
        }
    }while(dir->GetNext());

    for (int i = 0; i < tkey_values.size(); ++i) {
        for (int j = 0; j < tkey_values[i].size(); ++j) {
            std::cout << tkey_values[i][j];
        }
        std::cout<<std::endl;
    }
    std::cout << std::endl;
}

std::vector<TKey> DiskManager::ParseRecord(Table *tbl, std::vector<char> &data, int offset){
    std::vector<TKey> keys;
    Slot slot;
    std::memcpy(&slot, &data[offset], sizeof(Slot));
    char* record = new char[slot.GetLength()];
    std::memcpy(record,&data[slot.GetOffset()],slot.GetLength());
    
    int idx = 0;
    for(int i=0;i<tbl->GetAttributeNum();i++){
        int value_type=tbl->ats()[i].data_type();
        int length = tbl->ats()[i].length();
        TKey tmp(value_type,length);
        memcpy(tmp.key(),&record[idx],length);
        
        idx+=length;
        keys.push_back(tmp);
    }

    return keys;
}