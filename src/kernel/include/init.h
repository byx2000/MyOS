#ifndef INIT_H
#define INIT_H

void Init(void);
void InitBSS(void);
void InitGDT(void);
void Init8259A(void);
void InitIDT(void);
void InitTimer(void);
void InitTask(void);

#endif
