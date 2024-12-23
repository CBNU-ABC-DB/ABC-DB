# ABC-DB
<img src="https://github.com/user-attachments/assets/bcbdbece-bd8d-4cbe-b184-4e69634a0c39" width="500">


### MEMBER
**Lee jingyeong** in @CBNU  
**Nam jaehong** in @CBNU  
**Song binwon** in @CBNU  
**Jang jeonghwan** in @CBNU  

### Prerequisite
- docker

## How to run  

- docker build -t [IMAGE_NAME] /path/to/dockerfile
- docker run -it [IMAGE_IMAGE] --name [CONTAINER_NAME] /bin/sh
 
**IN CONTAINER**
```shell
/app $ make
/app $ ./ABC
```
**Available query**
- SELECT
- INSERT
- CREATE
- SHOW
- DESC

**Custom query** 
- test bufferpool
- test record [record num] [table명] values()

## EXAMPLE
```sql
CREATE DATABASE abc;
USE abc;
CREATE TABLE student(num int,name char(20));
INSERT INTO student VALUES(110,'student1');
SELECT * FROM student;
```

```sql
test bufferpool;
test record 10 student values(1, "gildong");
```

## REFERENCE
MINIDB from Yan Chen[https://github.com/nrthyrk/minidb]

---  
## overview of our abc-db
![overall design](https://github.com/user-attachments/assets/65aaa223-d09b-4b49-8c03-a20a68d0be61)
