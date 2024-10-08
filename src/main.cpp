
/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "page.h"
#include "file.h"
#include "bufferManager.h"
#include <iostream>


void search(File& f);
void iter(File& f, PageDirectory& dir);

int main() {
    std::shared_ptr<Page> p;

    /**
     * @brief 
     *  TODO::table생성 시 파일 및 첫번재 페이지 디렉토리 자동 생성.
     *   지금은 처음 실행 시 
     *   아래 주석 해제 후 실행
     *   주석 후 빌드
     *   다시 실행
     */
    File f("student.bin");
    PageDirectory directory(0);
    f.WritePageDirectoryToFile(directory);
    
    std::shared_ptr<PageDirectory> dir = f.GetPageDir(); 

    std::ofstream outfile("output.txt");  // 파일 열기
    if (outfile.is_open()){
        for (int i=0;i<800;i++){
            p = std::make_shared<Page>();
            f.AddPageToDirectory(*dir,*p);
            //레코드 삽입 및 직렬화
            const char* record1 = "jjang";
            const char* record2 = "1458%%$";
            p.get()->InsertRecord(record1, std::strlen(record1) + 1);
            p.get()->InsertRecord(record2, std::strlen(record2) + 1);
            f.WritePageToFile(*dir,*p);
            f.WritePageDirectoryToFile(*dir);
            outfile<<"page: "<<i<<std::endl;
        }
    }

    outfile.close();
    
   
  
    
    p = std::make_shared<Page>();
    f.AddPageToDirectory(*dir,*p);
    
    // 70개의 페이지 생성 후 디스크에 저장
    for(int i=0; i<70; i++){
        p = std::make_shared<Page>();
        p->SetFilename("student.bin");
        f.AddPageToDirectory(*dir,*p);

        std::string record1="record_"+std::to_string(i) + "_1";
        std::string record2="record_"+std::to_string(i)+"_2";
        p.get()->InsertRecord(record1.c_str(),record1.size()+1);
        p.get()->InsertRecord(record2.c_str(),record1.size()+1);
        f.WritePageToFile(*dir,*p);
        f.WritePageDirectoryToFile(*dir);
    }
    
    std::cout<<"write Page Directory to file "<<std::endl;
    BufferManager *bm=new BufferManager("student.bin");

    // bm->PrintAllPageFromDisk(dir);
    for(int i=1;i<60;i++){
        bm->GetPageFromDisk(dir,i)->PrintRecord();
    }
    
    std::cout<<"DISK READ DONE" <<std::endl;

    for(int i=0;i<10;i++){
        bm->GetPageFromBufferPool("student.bin",i+20)->PrintRecord(); //student테이블의 20~29번째 페이지 출력
    }

    //페이지디렉토리가 가리키는 페이지의 레코드 전부출력
    // search(f);
    return 0;
}

void search(File& f){
    std::shared_ptr<PageDirectory> dir = f.GetPageDir(); 
    while(dir->GetNext()){
        iter(f, *dir);
        size_t offset = dir->GetNext();
        dir = f.GetPageDir(offset);
    }
    iter(f, *dir);
}
void iter(File& f, PageDirectory& dir){
    const auto& entries = dir.GetEntries();
    for(int i=0; i<dir.GetSize(); i++){
        //std::cout<<"page인덱스: "<<i<<std::endl;
        std::shared_ptr<Page> p2 = f.GetPage(dir,i);
        //p2->PrintRecord();
    }
}