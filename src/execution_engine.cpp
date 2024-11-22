#include "execution_engine.h"

#include <iomanip>
#include <string>

/*=======================================ExecutionEngine================================================ */
void ExecutionEngine::Insert(SQLInsert &st)
{
    unsigned long values_size = st.values().size();
    std::string file_name(cm_->path() + db_name_ + "/" + st.tb_name() + ".bin");
    Table *tbl = cm_->GetDB(db_name_)->GetTable(file_name);

    if (tbl == NULL)
    {
        throw TableNotExistException();
    }

    int content_len = 0;
    std::vector<TKey> tkey_values;
    for (int i = 0; i < values_size; i++)
    {
        int value_type = st.values()[i].data_type;
        std::string value = st.values()[i].value;
        int length = tbl->ats()[i].length();
        content_len += length;

        TKey tmp(value_type, length);
        tmp.ReadValue(value.c_str());
        tkey_values.push_back(tmp);
    }

    char *content = new char[content_len + 1];
    char *content_ptr = content;
    for (std::vector<TKey>::iterator iter = tkey_values.begin(); iter != tkey_values.end(); ++iter)
    {
        std::memcpy(content_ptr, iter->key(), iter->length());
        content_ptr += iter->length();
    }

    std::shared_ptr<Page> bPage = bm_->GetEnoughSpacePage(tbl->GetFile(), content_len);

    if (!bPage)
    {
        File file(tbl->GetFile());
        std::shared_ptr<PageDirectory> dir = file.GetPageDir(0);
        std::shared_ptr<Page> page = file.GetEnoughSpacePage(content_len);

        if (page == nullptr)
        {
            std::shared_ptr<Page> new_page = std::make_shared<Page>(tbl->GetFile(), dir->GetIdx());
            new_page->SetFilename(tbl->GetFile());
            dir = file.AddPageToDirectory(*dir, *new_page);
            new_page->InsertRecord(content, content_len);
            file.WritePageToFile(*dir, *new_page);
        }
        else
        {
            page->InsertRecord(content, content_len);
            file.WritePageToFile(*dir, *page);
        }
        file.WritePageDirToFile(*dir);
        cm_->WriteArchiveFile();
    }
    else
    {
        bPage->SetFilename(tbl->GetFile());
        bm_->WriteBlock(bPage, content, content_len);
    }
}

void ExecutionEngine::Select(SQLSelect &st)
{
    std::string file_name(cm_->path() + db_name_ + "/" + st.tb_name() + ".bin");
    Table *tbl = cm_->GetDB(db_name_)->GetTable(file_name);

    for (int i = 0; i < tbl->GetAttributeNum(); ++i)
    {
        std::cout << std::setw(9) << std::left << tbl->ats()[i].attr_name();
    }
    std::cout << std::endl;

    File file(tbl->GetFile());
    std::shared_ptr<PageDirectory> dir = file.GetPageDir(0);
    std::vector<std::vector<TKey>> tkey_values;
    bm_->SetFile(file_name);
    do
    {
        for (int i = 0; i < dir->GetSize(); i++)
        {
            std::shared_ptr<Page> page = bm_->GetPageFromBufferPool(tbl->GetFile(), i);
            if (!page)
            {
                page = bm_->GetPageFromDisk(*dir, i);
            }

            std::vector<char> data = page->GetData();

            bm_->PromotePage(page); // LRU 알고리즘에 의하여 페이지 프로모션

            int offset = page->GetSlotOffset();
            for (int j = HEADER_SIZE; j < offset; j += sizeof(Slot))
            {
                std::vector<TKey> tkey_value = ParseRecord(tbl, data, j);

                if (EvaluateConditions(tkey_value, tbl, st.wheres()))
                {
                    tkey_values.push_back(tkey_value);
                }
            }
        }
    } while (dir->GetNext());

    for (const auto &row : tkey_values)
    {
        for (const auto &key : row)
        {
            std::cout << key;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
}

bool ExecutionEngine::EvaluateConditions(const std::vector<TKey> &record, Table *tbl, const std::vector<SQLWhere> &wheres)
{
    for (const auto &where : wheres)
    {
        bool condition_met = false;
        int attr_index = -1;

        for (int i = 0; i < tbl->GetAttributeNum(); ++i)
        {
            if (tbl->ats()[i].attr_name() == where.key)
            {
                attr_index = i;
                break;
            }
        }
        if (attr_index == -1)
            continue;

        const TKey &value = record[attr_index];
        TKey where_value(tbl->ats()[attr_index].data_type(), tbl->ats()[attr_index].length());
        where_value.ReadValue(where.value);

        switch (where.sign_type)
        {
        case SIGN_EQ:
            condition_met = (value == where_value);
            break;
        case SIGN_NE:
            condition_met = (value != where_value);
            break;
        case SIGN_LT:
            condition_met = (value < where_value);
            break;
        case SIGN_LE:
            condition_met = (value <= where_value);
            break;
        case SIGN_GT:
            condition_met = (value > where_value);
            break;
        case SIGN_GE:
            condition_met = (value >= where_value);
            break;
        default:
            break;
        }
        if (!condition_met)
            return false;
    }
    return true;
}

std::vector<TKey> ExecutionEngine::ParseRecord(Table *tbl, std::vector<char> &data, int offset)
{
    std::vector<TKey> keys;
    Slot slot;
    std::memcpy(&slot, &data[offset], sizeof(Slot));
    char *record = new char[slot.GetLength()];
    std::memcpy(record, &data[slot.GetOffset()], slot.GetLength());

    int idx = 0;
    for (int i = 0; i < tbl->GetAttributeNum(); i++)
    {
        int value_type = tbl->ats()[i].data_type();
        int length = tbl->ats()[i].length();
        TKey tmp(value_type, length);
        std::memcpy(tmp.key(), &record[idx], length);
        idx += length;
        keys.push_back(tmp);
    }

    delete[] record;
    return keys;
}

void ExecutionEngine::AddTestRecord(SQLTestRecord &st)
{
    unsigned long values_size = st.values().size();
    std::string file_name(cm_->path() + db_name_ + "/" + st.tb_name() + ".bin");
    Table *tbl = cm_->GetDB(db_name_)->GetTable(file_name);

    int request_page_size = std::stoi(st.values()[0].value);
    std::cout<<"Request Page Size: "<<request_page_size<<std::endl;
    if (tbl == NULL)
    {
        throw TableNotExistException();
    }
    int content_len = 0;
    std::vector<TKey> tkey_values;
    for (int i = 1; i < values_size; i++)
    {
        int value_type = st.values()[i].data_type;
        std::string value = st.values()[i].value;
        int length = tbl->ats()[i - 1].length();
        content_len += length;

        TKey tmp(value_type, length);
        tmp.ReadValue(value.c_str());
        tkey_values.push_back(tmp);
    }

    char *content = new char[content_len + 1];
    char *content_ptr = content;
    for (std::vector<TKey>::iterator iter = tkey_values.begin();
         iter != tkey_values.end(); ++iter)
    {
        std::memcpy(content_ptr, iter->key(), iter->length());
        content_ptr += iter->length();
    }

    // Test Page
    for (int j = 0; j < request_page_size; j++)
    {
        std::shared_ptr<Page> bPage = bm_->GetEnoughSpacePage(tbl->GetFile(), content_len);

        if (!bPage)
        {
            File file(tbl->GetFile());
            std::shared_ptr<PageDirectory> dir = file.GetPageDir(0);
            std::shared_ptr<Page> page = file.GetEnoughSpacePage(content_len);

            if (page == nullptr)
            {
                std::shared_ptr<Page> new_page = std::make_shared<Page>(tbl->GetFile(), dir->GetIdx());
                new_page->SetFilename(tbl->GetFile());
                dir = file.AddPageToDirectory(*dir, *new_page);
                new_page->InsertRecord(content, content_len);
                file.WritePageToFile(*dir, *new_page);
            }
            else
            {
                page->InsertRecord(content, content_len);
                file.WritePageToFile(*dir, *page);
            }
            file.WritePageDirToFile(*dir);
            cm_->WriteArchiveFile();
        }
        else
        {
            bPage->SetFilename(tbl->GetFile());
            bm_->WriteBlock(bPage, content, content_len);
        }
    }
}