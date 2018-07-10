#include <stdio.h>
#include <memory.h>
#include "sse/sse.h"
#include "crypto/encryption.h"
#include "crypto/decryption.h"

int main() {
    
    //connect to SSE-DB
    //SSE-DB works on port 2912
    int status = getConnection("192.168.50.189");

    if (status == 1){
        printf("SSE-DB connected\n");
    } else {
        printf("SSE-DB connection failed\n");
    }

    //create database
    unsigned char *db_name = (unsigned char *)"employee";

    unsigned char *enc_db_name[16];

    memset(enc_db_name, '\0', sizeof(enc_db_name));

    int enc_db_name_length = encrypt(db_name, strlen(db_name), (unsigned char *)enc_db_name);

    status = createDB(enc_db_name, enc_db_name_length);

    printf("database creation status: %d\n",status);

    //table create
    unsigned char *tb_name = (unsigned char *)"details";
    unsigned char *col_name_1 = (unsigned char *)"name";
    unsigned char *col_name_2 = (unsigned char *)"age";

    unsigned char *enc_tb_name[16];
    unsigned char *enc_col_name_1[16];
    unsigned char *enc_col_name_2[16];

    memset(enc_tb_name, '\0', sizeof(enc_tb_name));
    memset(enc_col_name_1, '\0', sizeof(enc_col_name_1));
    memset(enc_col_name_2, '\0', sizeof(enc_col_name_2));

    int enc_tb_name_length = encrypt(tb_name, strlen(tb_name), (unsigned char *)enc_tb_name);
    int enc_col_name_1_length = encrypt(col_name_1, strlen(col_name_1), (unsigned char *)enc_col_name_1);
    int enc_col_name_2_length = encrypt(col_name_2, strlen(col_name_2), (unsigned char *)enc_col_name_2);

    int a[] = {enc_db_name_length, enc_tb_name_length, enc_col_name_1_length, enc_col_name_2_length};

    status = createTable(a, 4, enc_db_name, enc_tb_name, enc_col_name_1, enc_col_name_2);


    int rows = countRows(enc_db_name, enc_db_name_length, enc_tb_name, enc_tb_name_length);

    printf("ROWS=> %d\n", rows);

    unsigned char *col_name_1_val = (unsigned char *)"bob";
    unsigned char *col_name_2_val = (unsigned char *)"30";

    unsigned char *enc_col_name_1_val[16];
    unsigned char *enc_col_name_2_val[16];

    memset(enc_col_name_1_val, '\0', sizeof(enc_col_name_1_val));
    memset(enc_col_name_2_val, '\0', sizeof(enc_col_name_2_val));

    int enc_col_name_1_val_length = encrypt(col_name_1_val, strlen(col_name_1_val), (unsigned char *)enc_col_name_1_val);
    int enc_col_name_2_val_length = encrypt(col_name_2_val, strlen(col_name_2_val), (unsigned char *)enc_col_name_2_val);

    int lengths[] = {enc_db_name_length, enc_tb_name_length, enc_col_name_1_length, enc_col_name_1_val_length, enc_col_name_2_length, enc_col_name_2_val_length};

    status = insert(lengths, 6, enc_db_name, enc_tb_name, enc_col_name_1, enc_col_name_1_val, enc_col_name_2, enc_col_name_2_val);

    //insert another tuple
    unsigned char *col_name_1_val_2 = (unsigned char *)"alice";
    unsigned char *col_name_2_val_2 = (unsigned char *)"26";

    unsigned char *enc_col_name_1_val_2[16];
    unsigned char *enc_col_name_2_val_2[16];

    memset(enc_col_name_1_val_2, '\0', sizeof(enc_col_name_1_val_2));
    memset(enc_col_name_2_val_2, '\0', sizeof(enc_col_name_2_val_2));

    int enc_col_name_1_val_length_2 = encrypt(col_name_1_val_2, strlen(col_name_1_val_2), (unsigned char *)enc_col_name_1_val_2);
    int enc_col_name_2_val_length_2 = encrypt(col_name_2_val_2, strlen(col_name_2_val_2), (unsigned char *)enc_col_name_2_val_2);

    int lengths_2[] = {enc_db_name_length, enc_tb_name_length, enc_col_name_1_length, enc_col_name_1_val_length_2, enc_col_name_2_length, enc_col_name_2_val_length_2};

    status = insert(lengths_2, 6, enc_db_name, enc_tb_name, enc_col_name_1, enc_col_name_1_val_2, enc_col_name_2, enc_col_name_2_val_2);


    rows = countRows(enc_db_name, enc_db_name_length, enc_tb_name, enc_tb_name_length);

    printf("ROWS=> %d\n", rows);

    int b[] = {enc_col_name_1_length, enc_col_name_2_length};

    //select query
    //select_query(enc_db_name, enc_db_name_length, enc_tb_name, enc_tb_name_length, b, 2, enc_col_name_1, enc_col_name_2);

    //select query with condition
    select_query_with_condition(enc_db_name, enc_db_name_length, enc_tb_name, enc_tb_name_length, "where ?=?", b, 2, enc_col_name_1, enc_col_name_2);

    set(1, enc_col_name_1, enc_col_name_1_length);
    set(2, enc_col_name_1_val, enc_col_name_1_val_length);

    while(hasNext()){
        unsigned  char *f_val = get(enc_col_name_1, enc_col_name_1_length);
        unsigned  char *s_val = get(enc_col_name_2, enc_col_name_2_length);


        char decryptedtext[16];
        memset(decryptedtext, '\0', sizeof(decryptedtext));
        int decryptedtext_len = decrypt(f_val, 16, decryptedtext);
        decryptedtext[decryptedtext_len] = '\0';

        printf("Decrypted Name: %s\n", decryptedtext);


        memset(decryptedtext, '\0', sizeof(decryptedtext));
        decryptedtext_len = decrypt(s_val, 16, decryptedtext);
        decryptedtext[decryptedtext_len] = '\0';

        printf("Decrypted Age: %s\n", decryptedtext);
    }



    //drop table
    int l[] = {enc_db_name_length, enc_tb_name_length};
    drop(l, 2, enc_db_name, enc_tb_name);

    //drop database
    drop(l, 1, enc_db_name);

    //close SSE-Db connection
    closeSSEDB();

    return 0;
}
