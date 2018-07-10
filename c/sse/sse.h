//
// Created by munawar on 9/7/18.
//

#ifndef SSE_C_SSE_H
#define SSE_C_SSE_H

int getConnection(char *);
int createDB(unsigned char*, int);
int createTable(int [], int, ...);
int countRows(unsigned char [], int, unsigned char [], int);
int insert(int [], int, ...);
void select_query(unsigned char [], int, unsigned char [], int, int [], int, ...);
void select_query_with_condition(unsigned char [], int , unsigned char [], int, char *, int [], int, ...);
void set(int, unsigned char [], int);
int hasNext();
unsigned char *get(char [], int);
int delete(unsigned char [], int, unsigned char [], int, char *, unsigned char [], int, unsigned char [], int);
int update(unsigned char [], int, unsigned char [], int, unsigned char [], int, unsigned char [], int);
int setValue(unsigned char [], int, unsigned char [], int, unsigned char [], int, unsigned char [], int, char *, unsigned char [], int, unsigned char [], int);
int drop(int [], int , ...);
int closeSSEDB();

#endif //SSE_C_SSE_H
